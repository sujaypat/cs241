/**
* Luscious Locks Lab
* CS 241 - Fall 2016
*/
#include "barrier.h"

// The returns are just for errors if you want to check for them.
int barrier_destroy(barrier_t *barrier) {
	int error = 0;
	pthread_mutex_destroy(&(barrier -> mtx));
	pthread_cond_destroy(&(barrier -> cv));
	return error;
}

int barrier_init(barrier_t *barrier, unsigned int num_threads) {
	int error = 0;
	return error;
}

int barrier_wait(barrier_t *barrier) {
	return 0;
}