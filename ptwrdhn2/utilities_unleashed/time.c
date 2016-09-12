/**
* Lab: Utilities Unleashed
* CS 241 - Fall 2016
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
	char *child_argv[] = NULL;
	for(int i = 1; i < argc; i++){
		child_argv[i - 1] = argv[i];
	}
	struct timespec start, end;
	if(!strcmp(argv[0],"time"))
	clock_gettime(CLOCK_MONOTONIC, &start);
	pid_t p = fork();
	if(p == 0){
		execvp(child_argv[0], child_argv[]);
	}
	int status;
	waitpid(p, status, WUNTRACED);
	clock_gettime(CLOCK_MONOTONIC, &end);
	// fork and get parent time
	// then exec sleep and get time on child
	// compare timestamps
	return 0;
}
