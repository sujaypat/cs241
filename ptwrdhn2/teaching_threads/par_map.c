/**
* Parallel Map Lab
* CS 241 - Fall 2016
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "map.h"
#include "mappers.h"

/* You should create a struct that will get passed in by reference to your
* start_routine. */
typedef struct blazeit{
	double *list;
	mapper func;
	size_t start_index;
	size_t end_index;
	double *res;
} blazeit;
/* You should create a start routine for your threads. */


void *routine(void *ptr){
	blazeit *input = (blazeit *)ptr;
	for(size_t i = input -> start_index; i < input -> end_index; i++){
		(input -> res)[i] = input -> func((input -> list)[i]);
	}
	return 0;
}

double *par_map(double *list, size_t list_len, mapper map_func, size_t num_threads) {

	double *res = (double *)malloc(list_len * sizeof(double));

	int block_size = list_len / num_threads;

	pthread_t ** threads = malloc(num_threads * sizeof(pthread_t *));
	blazeit ** arguments = malloc(num_threads * sizeof(blazeit));

	for(size_t index = 0; index < num_threads; index ++){

		threads[index] = malloc(sizeof(pthread_t));
		arguments[index] = malloc(sizeof(blazeit));

		arguments[index] -> func = map_func;
		arguments[index] -> start_index = index * block_size;
		arguments[index] -> end_index = (index + 1) * block_size - 1;
		arguments[index] -> list = list;
		arguments[index] -> res = res;
		pthread_create(threads[index], NULL, (void *)routine, (void *)(arguments + index));

	}
	for(size_t index = 0; index < num_threads; index ++){
		pthread_join(*(threads[index]), NULL);
		free(threads[index]);
		free(arguments[index]);
	}
	free(threads);
	free(arguments);
	return res;
}
