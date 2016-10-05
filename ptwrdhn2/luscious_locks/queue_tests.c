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

void *sender(void *queue) {
    queue_push(queue, "hello, ");
    queue_push(queue, "world");
    queue_push(queue, "how are you?");
	queue_push(queue, "hegfdllo, ");
    queue_push(queue, "worlfsdgd");
    queue_push(queue, "how arefdsg you?");
	queue_push(queue, "hello345, ");
    queue_push(queue, "world5234");
    queue_push(queue, "how are 4533245you?");
    printf("sent data\n");
	return 0;
}

void *recver(void *queue) {
    sleep(1);
    printf("received '%s'\n", queue_pull(queue));
    printf("received '%s'\n", queue_pull(queue));
    printf("received '%s'\n", queue_pull(queue));
	sleep(6);
	printf("received '%s'\n", queue_pull(queue));
	printf("received '%s'\n", queue_pull(queue));
	printf("received '%s'\n", queue_pull(queue));
	sleep(4);
	printf("received '%s'\n", queue_pull(queue));
	printf("received '%s'\n", queue_pull(queue));
	printf("received '%s'\n", queue_pull(queue));

	return 0;
}

int main() {
    queue_t *queue = queue_create(2);

    pthread_t s, r;
    pthread_create(&s, NULL, sender, (void*)queue);
    pthread_create(&r, NULL, recver, (void*)queue);
    pthread_join(s, NULL);
    pthread_join(r, NULL);

	queue_destroy(queue);
    return 0;
}