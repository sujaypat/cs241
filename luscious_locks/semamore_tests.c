/**
* Luscious Locks Lab
* CS 241 - Fall 2016
*/
#include "semamore.h"
#include <stdio.h>
#include <stdlib.h>
Semamore* s;
Semamore* s2;
Semamore* s3;
Semamore* s4;
Semamore* s5;
Semamore* s6;
Semamore* s7;
void* funct(void * arg)
{
	int i = *((int*)arg);
	if(i == 1)
	{
		printf("I AM CHILD %d and here I am\n",i);
		semm_post(s2);
	}
	if(i == 2)
	{
		semm_wait(s2);
		printf("I AM CHILD %d and here I am\n",i);
		semm_post(s3);
	}
	if(i == 3)
	{
		semm_wait(s3);
		printf("I AM CHILD %d and here I am\n",i);
		semm_post(s4);
	}
	if(i == 4)
	{
		semm_wait(s4);
		printf("I AM CHILD %d and here I am\n",i);
		semm_post(s5);
	}
	if(i == 5)
	{
		semm_wait(s5);
		printf("I AM CHILD %d and here I am\n",i);
		semm_post(s6);
	}
	if(i == 6)
	{
		semm_wait(s6);
		printf("I AM CHILD %d and here I am\n",i);

	}
	return NULL;
}
int main(int argc, char **argv) {
	s =  malloc(sizeof(Semamore));
	s2 =  malloc(sizeof(Semamore));
	s3 =  malloc(sizeof(Semamore));
	s4 =  malloc(sizeof(Semamore));
	s5 =  malloc(sizeof(Semamore));
	s6 =  malloc(sizeof(Semamore));
	s7 =  malloc(sizeof(Semamore));
	pthread_t thread,thread2,thread3, thread4, thread5, thread6;
	int i = 1;
	int j = 2;
	int k = 3;
	int l = 4;
	int m = 5;
	int n = 6;
	semm_init(s,0,2);
	semm_init(s2,0,2);
	semm_init(s3,0,2);
	semm_init(s4,0,2);
	semm_init(s5,0,2);
	semm_init(s6,0,2);
	semm_init(s7,0,2);
	int q;
	for(q = 0; q < atoi(argv[1]); q++){
		pthread_create(&thread,NULL,funct,(void*)(&j));
		pthread_create(&thread2,NULL,funct,(void*)(&i));
		pthread_create(&thread6,NULL,funct,(void*)(&k));
		pthread_create(&thread3,NULL,funct,(void*)(&l));
		pthread_create(&thread4,NULL,funct,(void*)(&n));
		pthread_create(&thread5,NULL,funct,(void*)(&m));
		pthread_join(thread,NULL);
		pthread_join(thread2,NULL);
		pthread_join(thread3,NULL);
		pthread_join(thread4,NULL);
		pthread_join(thread5,NULL);
		pthread_join(thread6,NULL);
		printf("\n\n Trian %d complete \n\n",q);
	}
	semm_destroy(s);
	semm_destroy(s2);
	semm_destroy(s3);
	semm_destroy(s4);
	semm_destroy(s5);
	semm_destroy(s6);
	semm_destroy(s7);
	return 0;
}