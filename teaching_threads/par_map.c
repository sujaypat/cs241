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
} args;
/* You should create a start routine for your threads. */

double *par_map(double *list, size_t list_len, mapper map_func, size_t num_threads) {
	double *res = (double *)malloc(list_len * sizeof(double));

	args a;
	int min = list_len < num_threads ? list_len : num_threads;
	pthread_t threads[num_threads];
	for(int i = 0; i < min; i++){
		a -> d = list[i];
		pthread_create(threads + i, NULL, map_func, &a);
	}
	for(int i = 0; i < min; i++){
		pthread_join(*(threads + i), NULL);
	}
	return res;
}
