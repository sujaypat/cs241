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
typedef struct args{
	double d;
	mapper func;
} args;
/* You should create a start routine for your threads. */


void *routine(void *ptr){
	args *input = (args *)ptr;
	input -> func(input -> d);
	return 0;
}

double *par_map(double *list, size_t list_len, mapper map_func, size_t num_threads) {

	double *res = (double *)malloc(list_len * sizeof(double));

	pthread_t threads[num_threads];
	args arguments[num_threads];

	for(size_t same = 0; same < list_len; same+= num_threads){

		for(size_t i = 0; i < num_threads && i + same < list_len; i++){
			arguments[i].d = list[same + i];
			arguments[i].func = map_func;
			pthread_create(threads + i, NULL, routine, arguments + i);
		}

		for(size_t j = 0; j < num_threads && j + same < list_len; j++){
			pthread_join(*(threads + j), NULL);
			res[same + j] = arguments[j].d;
		}

	}
	return res;
}
