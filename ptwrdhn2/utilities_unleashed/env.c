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
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


extern char **environ;

char ** comma_split(char * input){
	char ** res = NULL;
	int i = 0;
	char *token = NULL;
	while ((token = strsep(&input, ",")) != NULL) {
		res = realloc(res, (i + 1) * sizeof(char *));
		res[i] = malloc(sizeof(token));
		res[i] = token;
	}
	return res;
}

char ** replace_vars(char * input){
	char *beg = NULL;
	char *end = NULL;
	beg = strsep(&input, "%%");
		// input++;
	char *res = strcat(beg, end);
	return res;
}

int main(int argc, char *argv[]) {
	int status;

	if(argc == 1 && !strcmp(argv[0], "env")){
		for (char **env = environ; *env; ++env){
			printf("%s\n", *env);
		}
	}

	// if parent, fork for env changes, fork for command after
	// 1 arg for env, 2 args for command
	else{
		pid_t p = fork();
		if(p == 0){
			waitpid(p, &status, 0);
		}
		else if(p > 0){
			char **env_changes = comma_split(argv[1]);
			while(*env_changes){
				printf("%s\n", *env_changes);
				env_changes++;
			}
			char **modifications = replace_vars(env_changes);
			execvpe(argv[2], argv + 2, modifications);
			print_exec_failed();
			exit(errno);
		}
		else{
			print_fork_failed();
			exit(errno);
		}
	}
	return 0;
}


