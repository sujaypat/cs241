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
} blazeit;
/* You should create a start routine for your threads. */


void *routine(void *ptr){
	blazeit *input = (blazeit *)ptr;
	for(size_t i = input -> start_index; i < input -> end_index; i++){
		input -> func(*(input -> list + i));
	}
	return 0;
}

double *par_map(double *list, size_t list_len, mapper map_func, size_t num_threads) {

	double *res = (double *)malloc(list_len * sizeof(double));

	int block_size = list_len / num_threads;

	pthread_t threads[num_threads];
	blazeit arguments[num_threads];

	for(size_t index = 0; index < list_len; index += block_size){
		threads[index] = (pthread_t)malloc(sizeof(pthread_t));
		arguments[index] = (blazeit *)malloc(sizeof(blazeit));
		arguments[index].func = map_func;
		arguments[index].start_index = index;
		arguments[index].end_index = index + block_size - 1;
		arguments[index].list = list;
		pthread_create(&threads[index], NULL, routine, arguments + index);

	}
	for(size_t index = 0; index < num_threads; index ++){
		pthread_join(threads[index], NULL);
	}
	return res;
}
