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

void *sender(void * queue) {
    send(queue, "hello, ");
    send(queue, "world");
    send(queue, "how are you?");
    printf("sent data\n");
}

void *recver(void * queue) {
    sleep(1);
    printf("received '%s'\n", recv(queue));
    printf("received '%s'\n", recv(queue));
    printf("received '%s'\n", recv(queue));
}

int main() {
    mq_t * queue = create();

    pthread_t s, r;
    pthread_create(&s, null, sender, (void*)queue);
    pthread_create(&r, null, recver, (void*)queue);
    pthread_join(s, null);
    pthread_join(r, null);

    return 0;
}


// int main(int argc, char **argv) {
//
// 	printf("Please write tests cases\n");
// 	return 0;
// }
