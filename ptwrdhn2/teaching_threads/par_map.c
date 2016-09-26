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
	// printf("start: %zu  end: %zu\n", input -> start_index, input -> end_index);
	for(size_t i = input -> start_index; i <= input -> end_index; i++){
		(input -> res)[i] = input -> func((input -> list)[i]);
	}
	return 0;
}

double *par_map(double *list, size_t list_len, mapper map_func, size_t num_threads) {

	double *res = (double *)malloc(list_len * sizeof(double));

	size_t block_size = list_len / num_threads;
	size_t min = list_len > num_threads ? num_threads : list_len;
	size_t index;

	pthread_t ** threads = malloc(num_threads * sizeof(pthread_t *));
	blazeit * arguments = malloc(num_threads * sizeof(blazeit));

	for(index = 0; index < min - 1; index ++){

		if(((index + 1) * block_size - 1) > list_len) break;
		threads[index] = malloc(sizeof(pthread_t));

		(arguments+index) -> func = map_func;
		(arguments+index) -> start_index = index * block_size;
		(arguments+index) -> end_index = ((index + 1) * block_size - 1);

		(arguments+index) -> list = list;
		(arguments+index) -> res = res;
		pthread_create(threads[index], NULL, (void *)routine, (void *)(arguments + index));

	}
	// if(((index + 1) * block_size - 1) != list_len - 1){

	index = min - 1;
	threads[index] = malloc(sizeof(pthread_t));

	(arguments+index) -> func = map_func;
	(arguments+index) -> start_index = index * block_size;
	(arguments+index) -> end_index = list_len - 1;

	(arguments+index) -> list = list;
	(arguments+index) -> res = res;
	pthread_create(threads[index], NULL, (void *)routine, (void *)(arguments + index));
	// }
	for(index = 0; index < min; index ++){
		pthread_join(*(threads[index]), NULL);
		free(threads[index]);
	}
	free(threads);
	free(arguments);
	return res;
}
