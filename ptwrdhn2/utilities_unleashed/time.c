/**
* Lab: Utilities Unleashed
* CS 241 - Fall 2016
*/


int main(int argc, char *argv[]) {
	if(argv[0] == "time")
	time_t start = clock_gettime();
	pid_t p = fork();
	if(p == 0){
		exec
	}
	int status;
	waitpid(p, status, WUNTRACED);
	// fork and get parent time
	// then exec sleep and get time on child
	// compare timestamps
	return 0;
}
