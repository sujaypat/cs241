/**
* Lab: Utilities Unleashed
* CS 241 - Fall 2016
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


extern char **environ;

int main(int argc, char *argv[]) {
	char *vars = argv[1];
	char *func = argv[2];
	char *arg = argv[3];
	for (char **env = environ; *env; ++env)
        printf("%s\n", *env);
	return 0;
}
