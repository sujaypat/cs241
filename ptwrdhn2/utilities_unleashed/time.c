/**
* Lab: Utilities Unleashed
* CS 241 - Fall 2016
*/


int main(int argc, char *argv[]) {
	struct timespec start, end;
	if(!strcmp(argv[0],"time"))
	clock_gettime(CLOCK_MONOTONIC, &start);
	pid_t p = fork();
	if(p == 0){
		exec
	}
	int status;
	waitpid(p, status, WUNTRACED);
	clock_gettime(CLOCK_MONOTONIC, &end);
	// fork and get parent time
	// then exec sleep and get time on child
	// compare timestamps
	return 0;
}
