/**
* Luscious Locks Lab
* CS 241 - Fall 2016
*/
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"
typedef struct queue_node_t {
	struct queue_node_t *next;
	void *data;
} queue_node_t;
struct queue_t {
	queue_node_t *head, *tail;
	int size;
	int maxSize;
	pthread_cond_t cv;
	pthread_mutex_t m;
};
queue_t* queue;
void* pusher(void *p)
{
	int i = (*(int*)p);
	int j =0 ;
	for(j = 0; j < i ;j++)
	{
		int* i = 0;//malloc(sizeof(int));
		i = j;

		queue_push(queue,(void*)(i));
	}
	printf("queue size is now %d\n",queue->size);

	return NULL;
}
void* puller(void* p)
{
	int i = (*(int*)p);
	int n = (i/2) - 1;
	int j =0 ;
	int sum = 0;
	int predicted = 0;
	for(j = 0; j < i ;j++)
	{
		//sum += *((int*)queue_pull(queue));
		sum += *((int*)queue_pull(queue));
		predicted += j;
	}
	printf("queue size is now %d\n",queue->size);
	printf("The sum is %d and it should be %d\n",sum, (n*n + n) );
	return NULL;
}
int main(int argc, char **argv) {
	pthread_t thread1, thread2,thread3;
	int i = 5000;
	int j = 2* i ;
	queue = queue_create(10000);
	pthread_create(&thread1,NULL,pusher,(void*)(&i));
	pthread_create(&thread2,NULL,pusher,(void*)(&i));
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	pthread_create(&thread3,NULL,puller,(void*)(&j));
	pthread_join(thread3,NULL);
	queue_destroy(queue);
	return 0;
}