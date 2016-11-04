/**
* Parallel Make
* CS 241 - Fall 2016
*/
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "parmake.h"
#include "vector.h"
#include "queue.h"
#include "rule.h"
#include "parser.h"

int num_threads = 1;
char *path;
Vector *rules;
queue_t *jobs;
pthread_mutex_t vec;

// This is the constructor function for string element.
void *my_copy_ctor(void *elem) {
	return elem;
}
// This is the destructor function for string element.
void my_destructor(void *elem) {
	elem = (rule_t *)elem;
}

void callback(rule_t *target){
	Vector_append(rules, target);
}

void * start_routine(void *input){
	queue_t *use = (queue_t *)input;
	while(1){
		rule_t *thread_rule = queue_pull(use);
		if(!thread_rule) break;

	}
	return 0;
}

int parmake(int argc, char **argv) {

	pthread_t **threads = malloc(num_threads * sizeof(pthread_t *));

	pthread_mutex_init(&vec, 0);
	// arg parsing
	extern char *optarg;
	int opt = 0;
	if(argc != 1){
		while ((opt = getopt(argc, argv, "j:f:")) != -1) {
			switch (opt) {
				case 'f':
				path = strdup(optarg);
				break;

				case 'j':
				num_threads = atoi(optarg);
				break;

				default:
				break;
			}
		}
	}

	rules = Vector_create(&my_copy_ctor, &my_destructor);
	jobs = queue_create(-1, &my_copy_ctor, &my_destructor);

	parser_parse_makefile(path, &argv[optind], callback);

	for (size_t i = 0; i < Vector_size(rules); i++) {
		rule_t *temp = Vector_get(rules, i);
		if(Vector_size(temp -> dependencies) == 0){
			queue_push(jobs, temp);
		}
	}

	for(int j = 0; j < num_threads; j++){
		threads[j] = malloc(sizeof(pthread_t));
		pthread_create((threads[j]), 0, (void *)start_routine, jobs);
		queue_push(jobs, NULL);
	}

	for(int k = 0; k < num_threads; k++){
		pthread_join(*(threads[k]), 0);
		free(threads[k]);
	}
	free(threads);



	return 0;
}