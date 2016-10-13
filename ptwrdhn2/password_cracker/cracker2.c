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

// size_t numRecovered = 0, numFailed = 0, quit = 0, total = 0;
// pthread_mutex_t ct, tot;
//
// typedef struct thread_data{
// 	queue_t *queue;
// 	size_t	id;
// } td;
//
// void *start_routine(void *data){
// 	td *info = (td *)data;
// 	struct crypt_data cdata;
// 	struct timespec start, end;
// 	int incrementing = 1;
//
// 	cdata.initialized = 0;
//
// 	while(1){
//
// 		char *line = NULL;
// 		line = queue_pull((info -> queue));
// 		if(!line) break;
//
// 		char *username = strsep(&line, " "); // username
// 		char *result = strsep(&line, " "); // hashed password
// 		char *incomplete = line; // part of password given
//
// 		size_t count = 0;
// 		v1_print_thread_start(info -> id, username);
// 		clock_gettime(CLOCK_MONOTONIC, &start);
// 		char *test = NULL;
// 		int found = 0;
// 		size_t num_dots = 0;
// 		char *where_is_dot = strchr(incomplete, '.');
//
// 		for(size_t i = 0; i < strlen(incomplete); i++){
// 			if(incomplete[i] == '.'){
// 				incomplete[i] = 'a';
// 				num_dots++;
// 			}
// 		}
// 		while(incrementing){
// 			test = crypt_r(incomplete, "xx", &cdata);
// 			count++;
// 			if(!strcmp(result, test)){
// 				found = 1;
// 				break;
// 			}
// 			incrementing = incrementString(incomplete);
// 			if(*where_is_dot){
// 				if(*where_is_dot == 'z'){
// 					where_is_dot++;
// 				}
// 			}
// 			else incrementing = 0;
// 		}
//
// 		clock_gettime(CLOCK_MONOTONIC, &end);
// 		double time_diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/(1000000000);
//
// 		if(incrementing && found){
// 			v1_print_thread_result(info -> id, username, incomplete, count, time_diff, 0);
// 			pthread_mutex_lock(&ct);
// 			numRecovered++;
// 			pthread_mutex_unlock(&ct);
// 			pthread_mutex_lock(&tot);
// 			total--;
// 			pthread_mutex_unlock(&tot);
// 		}
// 		else{
// 			v1_print_thread_result(info -> id, username, incomplete, count, time_diff, 1);
// 			pthread_mutex_lock(&ct);
// 			numFailed++;
// 			pthread_mutex_unlock(&ct);
// 			pthread_mutex_lock(&tot);
// 			total--;
// 			pthread_mutex_unlock(&tot);
// 		}
// 		// free(line);
// 		// free(username);
// 		// free(result);
// 	}
// 	return 0;
// }
//
//
// int start(size_t thread_count) {
//
// 	pthread_mutex_init(&ct, 0);
// 	pthread_mutex_init(&tot, 0);
// 	queue_t *jobs = queue_create(-1); //maybe -1 for unbounded?
// 	int status = 0;
// 	size_t length = 0;
// 	char *line = NULL;
// 	pthread_t **threads = malloc(thread_count * sizeof(pthread_t *));
// 	td *arguments = malloc(thread_count * sizeof(td));
//
// 	while((status = getline(&line, &length, stdin)) != -1){
// 		char *line_dup = strdup(line);
// 		char *nl = strchr(line_dup, '\n');
// 		if (nl) *nl = 0;
// 		queue_push(jobs, line_dup);
// 		total++;
// 	}
// 	for(size_t i = 0; i < thread_count; i++){
// 		threads[i] = malloc(sizeof(pthread_t));
// 		(arguments[i]).queue = jobs;
// 		(arguments[i]).id = i;
// 		pthread_create((threads[i]), 0, (void *)start_routine, &(arguments[i]));
// 		queue_push(jobs, NULL);
// 	}
// 	quit = 1;
// 	for(size_t i = 0; i < thread_count; i++){
// 		pthread_join(*(threads[i]), 0);
// 		free(threads[i]);
// 	}
// 	free(threads);
// 	free(arguments);
// 	queue_destroy(jobs);
// 	v1_print_summary(numRecovered, numFailed);
// 	return 0;
// }
