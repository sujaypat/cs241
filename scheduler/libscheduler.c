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

  /* Add whatever other bookkeeping you need into this struct. */
};

struct _core_t {
  int free;   // boolean representing whether the core is in use or not
  job_t *job; // the job that the core is currently running, if any
};

priqueue_t pqueue;
core_t *cores;
scheme_t scheme;

int comparer_fcfs(const void *a, const void *b) { return 0; }

int break_tie(const void *a, const void *b) { return comparer_fcfs(a, b); }

int comparer_ppri(const void *a, const void *b) {
  // Complete as is
  return comparer_pri(a, b);
}

int comparer_pri(const void *a, const void *b) { return 0; }

int comparer_psjf(const void *a, const void *b) { return 0; }

int comparer_rr(const void *a, const void *b) {
  // Casting to void to silence compiler warning
  (void)a;
  (void)b;
  // Picking 1 arbitarily.
  return 1;
}

int comparer_sjf(const void *a, const void *b) { return 0; }

void scheduler_start_up(int numcores, scheme_t s) {
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

int scheduler_new_job(int job_number, int time, int running_time,
                      int priority) {
  return -1;
}

int scheduler_job_finished(int core_id, int job_number, int time) {
  return 9001;
}

int scheduler_quantum_expired(int core_id, int time) { return 9001; }

float scheduler_average_waiting_time() { return 9001; }

float scheduler_average_turnaround_time() { return 9001; }

float scheduler_average_response_time() { return 9001; }

void scheduler_clean_up() {
  /* Do more cleaning up here */
  priqueue_destroy(&pqueue);
}

void scheduler_show_queue() {
  // This function is left entirely to you! Totally optional.
}
