/**
 * Scheduler Lab
 * CS 241 - Fall 2016
 */
#pragma once

typedef struct _entry {
  void *value;
  struct _entry *next;
} entry;

/**
  Priqueue Data Structure
*/
typedef struct _priqueue_t {
  int (*comparer)(const void *, const void *);
  int size;
  entry *head;
} priqueue_t;

void priqueue_init(priqueue_t *q, int (*comparer)(const void *, const void *));
int priqueue_offer(priqueue_t *q, void *ptr);
void *priqueue_peek(priqueue_t *q);
void *priqueue_poll(priqueue_t *q);
int priqueue_size(priqueue_t *q);
void priqueue_destroy(priqueue_t *q);
