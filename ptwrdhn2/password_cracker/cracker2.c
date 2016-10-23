/**
* Machine Problem: Password Cracker
* CS 241 - Fall 2016
*/

#include "cracker2.h"
#include "format.h"
#include "utils.h"

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <time.h>


size_t quit = 0, total = 0;
double cputime = 0;
size_t tot_ct = 0;
pthread_mutex_t q, tot;
char *res;

typedef struct thread_data{
	char 	*start;
	long	max;
	size_t 	id;
	char 	*target;
	long 	offset;
	char	*user;
} td;

void *start_routine(void *data){
	td *info = (td *)data;
	struct crypt_data cdata;
	cdata.initialized = 0;

	size_t count = 0;


	char *test = NULL;
	int found = 0;
	// printf("%s\n", info -> start);
	char *first_dot = strchr(info -> start, '.');
	setStringPosition(first_dot, info -> offset);
	// printf("%s %zu\n", first_dot, info -> offset);

	v2_print_thread_start(info -> id, info -> user, (info -> offset), info -> start);
	size_t i;
	for(i = 0; i < (size_t)((info -> max)); i++){
		// check if this thread should give up
		pthread_mutex_lock(&q);
		if(quit){
			// printf("id: %zu gave up\n", info -> id);
			pthread_mutex_unlock(&q);
			break;
		}
		pthread_mutex_unlock(&q);
		test = crypt_r(info -> start, "xx", &cdata);
		count++;
		if(!strcmp(info -> target, test)){
			pthread_mutex_lock(&q);
			quit = 1;
			pthread_mutex_unlock(&q);
			found = 1;
			// printf("id: %zu found it\n", info -> id);
			break;
		}
		incrementString(first_dot);
	}
	// printf("id: %zu out of loop\n", info -> id);
	tot_ct += i;
	cputime += getThreadCPUTime();
	if(found){
		v2_print_thread_result(info -> id, i, 0);
		res = info -> start;
	}
	else if (i == (size_t)((info -> max))){
		v2_print_thread_result(info -> id, i, 2);
	}
	else v2_print_thread_result(info -> id, i, 1);

	return 0;
}


int start(size_t thread_count) {

	pthread_mutex_init(&q, 0);
	pthread_mutex_init(&tot, 0);
	int status = 0;
	size_t length = 0;
	char *line = NULL;
	pthread_t **threads = malloc(thread_count * sizeof(pthread_t *));
	td *arguments = malloc(thread_count * sizeof(td));
	struct timespec start, end;

	while((status = getline(&line, &length, stdin)) != -1){

		pthread_mutex_lock(&q);
		quit = 0;
		pthread_mutex_unlock(&q);

		char *line_dup = strdup(line);
		char *nl = strchr(line_dup, '\n');
		if (nl) *nl = 0;

		char *username = strsep(&line_dup, " "); // username
		char *result = strsep(&line_dup, " "); // hashed password
		char *incomplete = line_dup; // part of password given

		size_t num_dots = strlen(incomplete) - getPrefixLength(incomplete);

		// add subrange to struct
		v2_print_start_user(username);
		clock_gettime(CLOCK_MONOTONIC, &start);

		for(size_t i = 0; i < thread_count; i++){
			threads[i] = malloc(sizeof(pthread_t));
			(arguments[i]).target = result;
			(arguments[i]).id = i + 1;
			(arguments[i]).user = username;
			(arguments[i]).start = strdup(incomplete);

			getSubrange(num_dots, thread_count, i + 1, &(arguments[i]).offset, &(arguments[i]).max);
			pthread_create((threads[i]), 0, (void *)start_routine, &(arguments[i]));
		}
		total++;
		for(size_t i = 0; i < thread_count; i++){
			pthread_join(*(threads[i]), 0);
			free(threads[i]);
		}
		clock_gettime(CLOCK_MONOTONIC, &end);
		double time_diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/(1000000000);
		v2_print_summary(username, res, tot_ct, time_diff, cputime, !quit);
		cputime = 0;
		tot_ct = 0;
	} // end while
	free(threads);
	free(arguments);
	return 0;
}