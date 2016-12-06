/**
* Machine Problem: Wearables
* CS 241 - Fall 2016
*/
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

#include "utils.h"
#include "vector.h"
#include "wearable.h"

// The wearable server socket, which all wearables connect to.
int wearable_server_fd;

typedef struct {
	pthread_t thread;
	int fd;
	long timestamp;
	// TODO you might want to put more things here
} thread_data;

thread_data **wearable_threads;
int wearable_threads_size = 0;
int sessionEnd = 0;

Vector *timestamps;
pthread_mutex_t vec_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t vec_cv = PTHREAD_COND_INITIALIZER;

void signal_received(int sig) {
	// TODO close server socket, free anything you don't free in main
	if(sig == SIGINT) {
		sessionEnd = 1;
		for(int i = 0; i < wearable_threads_size; i++) {
			shutdown(wearable_threads[i]->fd, 2);
			close(wearable_threads[i]->fd);
		}
		// for(size_t i = 0; i < Vector_size(timestamps); i++) {
		// 	// free data
		// 	free(((timestamp_entry *)Vector_get(timestamps, i))->data);
		// }
		shutdown(wearable_server_fd, 2);
		close(wearable_server_fd);
	}
}
void *wearable_processor_thread(void *args) {
	thread_data *td = (thread_data *)args;
	int socketfd = td->fd;

	// TODO read data from the socket
	char buf[64];
	while(!sessionEnd && recv(socketfd, buf, 64, 0) > 0) {
		timestamp_entry entry;
		long time;
		extract_key(buf, &time, (SampleData **) &(entry.data));
		entry.time = (unsigned long) time;
		pthread_mutex_lock(&vec_mutex);
		td->timestamp = time;
		Vector_append(timestamps, (void *) &entry);
		// free(((SampleData *)entry.data) -> type_);
		pthread_cond_broadcast(&vec_cv);
		pthread_mutex_unlock(&vec_mutex);
	}

	pthread_mutex_lock(&vec_mutex);
	td->timestamp = LONG_MAX;
	pthread_cond_broadcast(&vec_cv);
	pthread_mutex_unlock(&vec_mutex);

	shutdown(socketfd, 2);
	return NULL;
}

bool type1_selector(timestamp_entry *arg) {
	SampleData *data = (SampleData *)arg->data;
	return strcmp(data->type_, TYPE1) == 0;
}

bool type2_selector(timestamp_entry *arg) {
	SampleData *data = (SampleData *)arg->data;
	return strcmp(data->type_, TYPE2) == 0;
}

bool type3_selector(timestamp_entry *arg) {
	SampleData *data = (SampleData *)arg->data;
	return strcmp(data->type_, TYPE3) == 0;
}

void *user_request_thread(void *args) {
	int socketfd = *((int *)args);

	// TODO read data from the socket
	// Requests will be in the form "<timestamp1>:<timestamp2>"
	// Write out statistics for data between those timestamp ranges
	while(!sessionEnd) {
		char buf[64];
		memset(buf, 0, 64);
		unsigned long start = 0, end = 0;

		while(read(socketfd, buf, 64) > 0) {
			sscanf(buf, "%lu:%lu", &start, &end);
			pthread_mutex_lock(&vec_mutex);
			for(int i = 0; i < wearable_threads_size; i++) {
				while((unsigned long)(wearable_threads[i]->timestamp) < end) {
					pthread_cond_wait(&vec_cv, &vec_mutex);
				}
			}
			timestamp_entry *e1 = NULL;
			timestamp_entry *e2 = NULL;
			timestamp_entry *e3 = NULL;
			ssize_t s1 = gather_timestamps(timestamps, start, end, type1_selector, &e1);
			ssize_t s2 = gather_timestamps(timestamps, start, end, type2_selector, &e2);
			ssize_t s3 = gather_timestamps(timestamps, start, end, type3_selector, &e3);

			pthread_mutex_unlock(&vec_mutex);


			if (s1 == -1 || s2 == -1 || s3 == -1) {
				perror(NULL);
				exit(1);
			}
			write_results(socketfd, TYPE1, e1, s1);
			write_results(socketfd, TYPE2, e2, s2);
			write_results(socketfd, TYPE3, e3, s3);
			free(e1);
			e1 = NULL;
			free(e2);
			e2 = NULL;
			free(e3);
			e3 = NULL;
			write(socketfd, "\r\n", 2);
		}
	}

	close(socketfd);
	return NULL;
}

int open_server_socket(const char *port) {
	// TODO given a string with the port value, set up a passive socket file
	// descriptor and return it
	int optval = 1;
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  	setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int s = getaddrinfo(NULL, port, &hints, &result);
	if(s) {
		fprintf(stderr, "%s:%d getaddrinfo: %s\n", __FILE__, __LINE__, gai_strerror(s));
		return -1;
	}

	if(bind(server_socket, result->ai_addr, result->ai_addrlen)) {
		perror("bind()");
		return -1;
	}

	if(listen(server_socket, 30)) {
		perror("listen()");
		return -1;
	}
	free(result);
	return server_socket;
}

int wearable_server(const char *wearable_port, const char *request_port) {

	timestamps = Vector_create(timestamp_entry_copy_constructor, timestamp_entry_destructor);
	// TODO setup signal handler for SIGINT
	signal(SIGINT, signal_received);
	int request_server_fd = open_server_socket(request_port);
	wearable_server_fd = open_server_socket(wearable_port);

	pthread_t request_thread;
	int request_socket = accept(request_server_fd, NULL, NULL);
	pthread_create(&request_thread, NULL, user_request_thread, &request_socket);
	close(request_server_fd);

	wearable_threads = calloc(200, sizeof(thread_data *));

	// TODO accept continous requests on the wearable port
	//sessionEnd from chatroom
	while(!sessionEnd) {
		int client = accept(wearable_server_fd, NULL, NULL);
		if(client == -1) break;
		thread_data * td = malloc(sizeof(thread_data));
		td->fd = client;
		td->timestamp = LONG_MAX;
		pthread_mutex_lock(&vec_mutex);
		wearable_threads[wearable_threads_size++] = td;
		pthread_mutex_unlock(&vec_mutex);
		int retval = pthread_create(&(td->thread), NULL, wearable_processor_thread, td);
		if(retval != 0) {
			fprintf(stderr, "Wearable processor thread creation failed!\n");
		}
	}
	// TODO join all threads we spawned from the wearables.
	// Cleanup anything we've allocated.
	for(int i = 0; i < wearable_threads_size; i++) {
		pthread_join(wearable_threads[i]->thread, NULL);
		free(wearable_threads[i]);
	}
	free(wearable_threads);

	for(size_t i = 0; i < Vector_size(timestamps); i++) {
		// free data
		free(((SampleData *)((timestamp_entry *)Vector_get(timestamps, i))->data) -> type_);
		free(((timestamp_entry *)Vector_get(timestamps, i))->data);
	}
	Vector_destroy(timestamps);

	pthread_join(request_thread, NULL);

	return 0;
}