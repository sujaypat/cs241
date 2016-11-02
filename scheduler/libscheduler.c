/**
* Scheduler Lab
* CS 241 - Fall 2016
*/
#include "libpriqueue.h"
#include "libscheduler.h"

typedef struct _job_t job_t;
typedef struct _core_t core_t;
/**
Stores information making up a job to be scheduled including any statistics.

You may need to define some global variables or a struct to store your job
queue elements.
*/
struct _job_t {
	int id;
	int priority;
	float length;
	float start;
	float arrival;
	float remain;
	/* Add whatever other bookkeeping you need into this struct. */
};

struct _core_t {
	int free;   // boolean representing whether the core is in use or not
	job_t *job; // the job that the core is currently running, if any
};
size_t num_cores;
priqueue_t pqueue;
core_t *cores;
scheme_t scheme;

float waiting = 0;
float turnaround = 0;
float response = 0;
int num_procs = 0;

int comparer_fcfs(const void *a, const void *b) {
	int diff = ((job_t *)a) -> arrival - ((job_t *)b) -> arrival;
	return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

int break_tie(const void *a, const void *b) {
	return comparer_fcfs(a, b);
}

int comparer_ppri(const void *a, const void *b) {
	// Complete as is
	return comparer_pri(a, b);
}

int comparer_pri(const void *a, const void *b) {
	int diff = ((job_t *)a) -> priority - ((job_t *)b) -> priority;
	if (diff == 0) return break_tie(a,b);
	return (diff > 0) ? -1 : ((diff < 0) ? 1 : 0);
}

int comparer_psjf(const void *a, const void *b) {
	int diff = ((job_t *)a) -> remain - ((job_t *)b) -> remain;
	if (diff == 0) return break_tie(a,b);
	return (diff > 0) ? 1 : -1;
}

int comparer_rr(const void *a, const void *b) {
	// Casting to void to silence compiler warning
	(void)a;
	(void)b;
	// Picking 1 arbitarily.
	return 1;
}

int comparer_sjf(const void *a, const void *b) {
	int diff = ((job_t *)a) -> length - ((job_t *)b) -> length;
	if(diff == 0) return break_tie(a, b);
	return (diff > 0) ? 1 : -1;
}

void scheduler_start_up(int numcores, scheme_t s) {
	cores = malloc(numcores * sizeof(core_t));
	num_cores = numcores;
	for(int i = 0; i < numcores; i++){
		cores[i].free = 1;
	}
	scheme = s;
	switch (s) {
		case FCFS:
		priqueue_init(&pqueue, comparer_fcfs);
		break;

		case PRI:
		priqueue_init(&pqueue, comparer_pri);
		break;

		case PPRI:
		priqueue_init(&pqueue, comparer_ppri);
		break;

		case PSJF:
		priqueue_init(&pqueue, comparer_psjf);
		break;

		case RR:
		priqueue_init(&pqueue, comparer_rr);
		break;

		case SJF:
		priqueue_init(&pqueue, comparer_sjf);
		break;

		default:
		printf("Did not recognize scheme\n");
		exit(1);
	}
}

int scheduler_new_job(int job_number, int time, int running_time, int priority) {

	job_t *j = malloc(sizeof(job_t));
	if(!j) return -1;
	j -> id = job_number;
	j -> priority = priority;
	j -> arrival = time;
	j -> length = running_time;
	j -> start = 0;
	j -> remain = j -> length;

	num_procs++;

	int preempt = 2; //0 = ppri, 1 = psjf, 2 = no preempt
	switch (scheme) {
		case PPRI:
		preempt = 0;
		break;

		case PSJF:
		preempt = 1;
		break;

		default:
		break;
	}

	for(size_t i = 0; i < num_cores; i++){
		if(cores[i].free == 1){
			cores[i].free = 0;
			j -> start = time;
			cores[i].job = j;
			return i;
		}
		cores[i].job -> remain -= (time - cores[i].job -> start);
	}
	priqueue_offer(&pqueue, j);
	job_t *new = priqueue_poll(&pqueue);
	if(preempt == 0){
		for (size_t k = 0; k < num_cores; k++) {
			if(comparer_ppri(new, cores[k].job) == -1){
				priqueue_offer(&pqueue, cores[k].job);
				cores[k].free = 0;
				new -> start = time;
				return k;
			}
		}
	}
	if(preempt == 1){
		for (size_t k = 0; k < num_cores; k++) {
			if(comparer_psjf(new, cores[k].job) == -1){
				priqueue_offer(&pqueue, cores[k].job);
				cores[k].free = 0;
				new -> start = time;
				return k;
			}
		}
	}
	priqueue_offer(&pqueue, new);
	return -1;
}

int scheduler_job_finished(int core_id, int job_number, int time) {

	waiting += (time - cores[core_id].job -> arrival) - cores[core_id].job -> length;
	job_t *job = (job_t *)(priqueue_poll(&pqueue));
	if(!job){
		cores[core_id].free = 1;
		return -1;
	}
	job -> start = time;
	response += time - job -> arrival;
	turnaround += time - cores[core_id].job -> arrival;
	return job -> id;
}

int scheduler_quantum_expired(int core_id, int time) {
	core_t temp = cores[core_id];
	priqueue_offer(&pqueue, temp.job);
	job_t *job = (job_t *)(priqueue_poll(&pqueue));
	if(job) return job -> id;
	return -1;
}

float scheduler_average_waiting_time() {
	return waiting / num_procs;
}

float scheduler_average_turnaround_time() {
	return turnaround / num_procs;
}

float scheduler_average_response_time() {
	return response / num_procs;
}

void scheduler_clean_up() {
	/* Do more cleaning up here */
	free(cores);
	priqueue_destroy(&pqueue);
}

void scheduler_show_queue() {
	// This function is left entirely to you! Totally optional.
}
