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
#include <sys/types.h>
#include <sys/wait.h>


extern char **environ;

int main(int argc, char *argv[]) {
	int status_env, status_cmd;
	// char *env_args = NULL;

	if(argc == 1){
		for (char **env = environ; *env; ++env){
			printf("%s\n", *env);
		}
	}
	// if parent, fork for env changes, fork for command after
	// 1 arg for env, 2 args for command
	else{
		pid_t env_change = fork();
		if(env_change == 0){
			waitpid(env_change, &status_env, 0);
			pid_t command = fork();
			if(command == 0){
				waitpid(command, &status_cmd, 0);
			}
			else if(command > 0){
				execvp(argv[2], argv + 2);
				print_exec_failed();
			}
			else{
				print_fork_failed();
			}
		}
		else if(env_change > 0){
			execvp(argv[1], argv);
			print_exec_failed();
		}
		else{
			print_fork_failed();
		}
	}


	return 0;
}
