/**
* Parallel Make
* CS 241 - Fall 2016
*/
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "parmake.h"
#include "vector.h"
#include "queue.h"
#include "rule.h"
#include "parser.h"

int num_threads = 1;
char *path = NULL;
Vector *rules;
Vector *satisfied;
queue_t *jobs;
pthread_mutex_t vec;
size_t num_jobs = 0;

// state: 0-unprocessed, 1-added, 2-completed, 3-failed

void *my_copy_ctor(void *elem) {
	return elem;
}
void my_destructor(void *elem) {
	elem = (rule_t *)elem;
}

int contains(Vector *search, rule_t *in){
	int res = 0;
	for(size_t i = 0; i < Vector_size(search); i++){
		// printf("comparing %s with %s\n", in -> target, ((rule_t *)Vector_get(search, i)) -> target);
		if(!strcmp(in -> target, ((rule_t *)Vector_get(search, i)) -> target)){
			res = 1;
		}
	}
	return res;
}

void print_vector(Vector *in){
	for (size_t i = 0; i < Vector_size(in); i++) {
		fprintf(stderr, "%s ", ((rule_t *)Vector_get(in, i)) -> target);
	}
	fprintf(stderr, "\n");
}

void callback(rule_t *target){
	Vector_append(rules, target);
}

void * start_routine(void *input){
	queue_t *use = (queue_t *)input;
	while(1){
		int flag = 0;
		rule_t *thread_rule = queue_pull(use);
		if(!thread_rule) break;

		for(size_t j = 0; j < Vector_size(thread_rule -> dependencies); j++){
			while(((rule_t *)Vector_get(thread_rule -> dependencies, j)) -> state != 2 && ((rule_t *)Vector_get(thread_rule -> dependencies, j)) -> state != 3){}
			if(((rule_t *)Vector_get(thread_rule -> dependencies, j)) -> state == 3){
				flag = 1;
			}
		}
		if(!flag){
			for(size_t i = 0; i < Vector_size(thread_rule -> commands); i++){
				int out = system(Vector_get(thread_rule -> commands, i));
				if(out != 0){
					thread_rule -> state = 3;
					continue;
				}
			}
			thread_rule -> state = 2;
		}
	}
	return 0;
}



int parmake(int argc, char **argv) {

	pthread_t **threads = malloc(num_threads * sizeof(pthread_t *));

	pthread_mutex_init(&vec, 0);
	// arg parsing
	extern char *optarg;
	int opt = 0;
	if(argc != 1){
		while ((opt = getopt(argc, argv, "j:f:")) != -1) {
			switch (opt) {
				case 'f':
				path = strdup(optarg);
				break;

				case 'j':
				num_threads = atoi(optarg);
				break;

				default:
				break;
			}
		}
	}

	if(!path){
		// if not makefile then Makefile else quit
		if (access("makefile", F_OK) != -1) path = "makefile";
		else if (access("Makefile", F_OK) != -1) path = "Makefile";
		else exit(1);
	}

	rules = Vector_create(&my_copy_ctor, &my_destructor);
	satisfied = Vector_create(&my_copy_ctor, &my_destructor);
	jobs = queue_create(-1, &my_copy_ctor, &my_destructor);

	parser_parse_makefile(path, &argv[optind], callback);


	for (size_t i = 0; i < Vector_size(rules); i++) {
		rule_t *temp = Vector_get(rules, i);
		if(Vector_size(temp -> dependencies) == 0){
			((rule_t *)(Vector_get(rules, i))) -> state = 1;
			// printf("added to queue with 0 dependencies: %s\n", ((rule_t *)(Vector_get(rules, i))) -> target);
			queue_push(jobs, temp);
			num_jobs++;
			Vector_append(satisfied, temp);
			// print_vector(satisfied);
		}
	}
	size_t total_rules = Vector_size(rules);
	int flag = 0; // 0 if found, 1 if not found
	while(total_rules > 0){
		size_t orig_num = total_rules;
		for(size_t i = 0; i < Vector_size(rules); i++){
			flag = 0;
			for(size_t j = 0; j < Vector_size(((rule_t *)Vector_get(rules, i)) -> dependencies); j++){
				// printf("current rule: %s\n", ((rule_t *)Vector_get(rules, i)) -> target);
				// printf("searching for %s in %s\n", ((rule_t *)Vector_get(((rule_t *)Vector_get(rules, i)) -> dependencies, j)) -> target, "satisfied");
				if(!contains(satisfied, Vector_get(((rule_t *)Vector_get(rules, i)) -> dependencies, j))){
					// printf("not found\n");
					flag = 1;
					break;
				}
			}
			if(!flag && !contains(satisfied, ((rule_t *)Vector_get(rules, i)))){
				// printf("adding to queue: %s\n", ((rule_t *)(Vector_get(rules, i))) -> target);
				num_jobs++;
				queue_push(jobs, Vector_get(rules, i));
				Vector_append(satisfied, Vector_get(rules, i));
				// print_vector(satisfied);
				total_rules--;
			}
		}
		if(orig_num == total_rules){
			// cycle detected
			break;
		}
	}

	for(int j = 0; j < num_threads; j++){
		threads[j] = malloc(sizeof(pthread_t));
		pthread_create((threads[j]), 0, (void *)start_routine, jobs);
		queue_push(jobs, NULL);
	}

	for(int k = 0; k < num_threads; k++){
		pthread_join(*(threads[k]), 0);
		free(threads[k]);
	}
	free(threads);

	return 0;
}