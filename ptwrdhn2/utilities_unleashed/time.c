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
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int status;
	struct timespec start, end;

	if(argc < 2) print_time_usage();

	// fork exec wait block with timing
	clock_gettime(CLOCK_MONOTONIC, &start);
	pid_t p = fork();
	if(p > 0){
		waitpid(p, &status, 0);
	}
	else if(p == 0){
		execvp(argv[1], argv + 1);
		print_exec_failed();
		exit(errno);
	}
	else{
		print_fork_failed();
		exit(errno);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	if(WIFEXITED(status)){
		if(!WEXITSTATUS(status))
			exit(WEXITSTATUS(status));
	}
	// calculate time difference
	double time_diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/(1000000000);
	display_results(argv, time_diff);

	return 0;
}
