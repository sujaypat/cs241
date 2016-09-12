/**
* Lab: Utilities Unleashed
* CS 241 - Fall 2016
*/
#include "format.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	char *child_argv[argc];
	int status;
		struct timespec start, end;

	if(argc <= 3) print_time_usage();

	for(int i = 1; i <= argc; i++){
		child_argv[i - 1] = argv[i];
	}

	clock_gettime(CLOCK_MONOTONIC, &start);
	pid_t p = fork();
	if(p > 0){
		waitpid(p, &status, 0);
	}
	else if(p == 0){
		execvp(child_argv[0], child_argv);
		print_exec_failed();
	}
	else{
		print_fork_failed();
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	double time_diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/(1000000000);
	display_results(argv, time_diff);
	
	return 0;
}
