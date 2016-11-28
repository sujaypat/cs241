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

void signal_received(int sig) {
  // TODO close server socket, free anything you don't free in main
}

void *wearable_processor_thread(void *args) {
  thread_data *td = (thread_data *)args;
  int socketfd = td->fd;

  // TODO read data from the socket

  close(socketfd);
  return NULL;
}

void *user_request_thread(void *args) {
  int socketfd = *((int *)args);

  // TODO read data from the socket
  // Requests will be in the form "<timestamp1>:<timestamp2>"
  // Write out statistics for data between those timestamp ranges

  close(socketfd);
  return NULL;
}

int open_server_socket(const char *port) {
  // TODO given a string with the port value, set up a passive socket file
  // descriptor and return it
  return 0;
}

int wearable_server(const char *wearable_port, const char *request_port) {
  // TODO setup signal handler for SIGINT

  int request_server_fd = open_server_socket(request_port);
  wearable_server_fd = open_server_socket(wearable_port);

  pthread_t request_thread;
  int request_socket = accept(request_server_fd, NULL, NULL);
  pthread_create(&request_thread, NULL, user_request_thread, &request_socket);
  close(request_server_fd);

  // TODO accept continous requests on the wearable port

  // TODO join all threads we spawned from the wearables.

  // Cleanup anything we've allocated.
  pthread_join(request_thread, NULL);

  return 0;
}
