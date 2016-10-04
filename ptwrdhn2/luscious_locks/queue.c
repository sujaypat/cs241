/**
* Luscious Locks Lab
* CS 241 - Fall 2016
*/
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
* Struct representing a node in a queue_t
*/
typedef struct queue_node_t {

	struct queue_node_t *next;
	void *data;
} queue_node_t;

/**
* Struct representing a queue
*/
struct queue_t {

	queue_node_t *head, *tail;
	int size;
	int maxSize;
	pthread_cond_t cv;
	pthread_mutex_t m;
};

/**
*  Given data, place it on the queue.  Can be called by multiple threads.
*  Blocks if the queue is full.
*/
void queue_push(queue_t *queue, void *data) {
	pthread_mutex_lock(queue -> m);
	queue_node_t *new_node = malloc(sizeof(queue_node_t));
	new_node -> data = data;
	new_node -> next = NULL;
	queue -> tail -> next = new_node;
	queue -> tail = new_node;
	pthread_cond_broadcast(cv);
	pthread_mutex_unlock(m);
}

/**
*  Retrieve the data from the front of the queue.  Can be called by multiple
* threads.
*  Blocks if the queue is empty.
*/
void *queue_pull(queue_t *queue) {
	pthread_mutex_lock(queue -> m);
	while(q->head == NULL){
		pthread_cond_wait(cv, m);
	}
	void *val = queue -> head -> data;
	queue_node_t *tmp = queue -> head;
	queue -> head = queue -> head -> next;
	free(tmp);
	pthread_mutex_unlock(m);
	return val;
}

/**
*  Allocates heap memory for a queue_t and initializes it.
*  Returns a pointer to this allocated space.
*/
queue_t *queue_create(int maxSize) {
	queue_t *same = malloc(sizeof(queue_t));
	same -> head = NULL;
	same -> tail = NULL;
	same -> size = 0;
	same -> maxSize = maxSize;
	pthread_cond_init(cv, NULL);
	pthread_mutex_init(m, NULL);
	return same;
}

/**
*  Destroys the queue, freeing any remaining nodes in it.
*/
void queue_destroy(queue_t *queue) {
	while (!isempty(queue)){

		queue -> head = queue -> head -> next;
		free(queue -> head);
	}
}