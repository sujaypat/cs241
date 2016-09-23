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
typedef struct 420blazeit{
	mapper func;
	size_t start_index;
	size_t end_index;
} 420blazeit;
/* You should create a start routine for your threads. */


void *routine(void *ptr){
	420blazeit *input = (420blazeit *)ptr;
	for(int i = input -> start_index; i < input -> end_index; i++){
		input -> func(input -> d);
	}
	return 0;
}

double *par_map(double *list, size_t list_len, mapper map_func, size_t num_threads) {

	double *res = (double *)malloc(list_len * sizeof(double));

	int block_size = list_len / num_threads;

	pthread_t threads[num_threads];
	420blazeit arguments[num_threads];

	for(size_t index = 0; index < list_len; index += block_size){

		arguments[index].func = map_func;
		arguments[index].start_index = index;
		arguments[index].end_index = index + block_size - 1;
		pthread_create(num_threads + index, NULL, routine, arguments + index);

		// for(size_t i = 0; i < num_threads && i + index < list_len; i++){
		// 	arguments[i].d = list[index + i];
		// 	arguments[i].func = map_func;
		// 	pthread_create(threads + i, NULL, routine, arguments + i);
		// }
		//
		// for(size_t index = 0; index < num_threads && index + index < list_len; index++){

		// 	res[index + index] = arguments[index].d;
		// }
	}
	for(size_t index = 0; index < num_threads; index ++){
		pthread_join(*(threads + index), NULL);
	}
	return res;
}
