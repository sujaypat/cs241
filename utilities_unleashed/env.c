/**
* Lab: Utilities Unleashed
* CS 241 - Fall 2016
*/
#include "format.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
extern char** eviron;

int main(int argc, char *argv[]) {
	int env_status;
	if(argc == 1){
		for (char **env = environ; *env; ++env){
			printf("%s\n", *env);
		}
		return 0;
	}
	if(argc < 3){
		print_env_usage();
		exit(1);
	}
	pid_t env_fork = fork();
	if(env_fork == 0){
		// actual code
		// fprintf(stderr, "fuck me sideways\n");
		char *env_args = strdup(argv[1]);
		char *temp = strsep(&env_args, ",");

		while(temp != NULL){
			// fprintf(stderr, "temp: %s\n", temp);
			char *tempcopy = strdup(temp);
			if(strchr(temp, '%') == NULL){
				// fprintf(stderr, "putting temp in env\n");
				putenv(tempcopy);
			}
			else{
				int len = strlen(tempcopy) + 1;
				int curr_index = 0;
				char *res = calloc(len, 1);

				while(*tempcopy != '='){
					res[curr_index] = *tempcopy;
					curr_index++;
					tempcopy++;
				}
				res[curr_index] = *tempcopy;
				tempcopy++;
				curr_index++;

				while(*tempcopy){
					if(*tempcopy == '%'){
						tempcopy++;
						int iter = 0;
						char* env_entry = calloc(len, 1);
						while(isalpha(*tempcopy) || isdigit(*tempcopy) || *tempcopy == '_'){
							env_entry[iter] = *tempcopy;
							iter++;
							tempcopy++;
						}
						len -= (iter + 1);
						char* value = getenv(env_entry);
						if(value){
							len += strlen(value);
						}
						else value = "";

						free(env_entry);
						char * orig = strdup(res);
						// fprintf(stderr, "1len: %d res: %s value: %s\n", len, res, value);
						res = calloc(len + 100, 1);

						strcpy(res, orig);
						free(orig);
						// fprintf(stderr, "2len: %d res: %s value: %s\n", len, res, value);
						strcat(res, value);
						curr_index += strlen(value);
					}
					else{
						res[curr_index] = *tempcopy;
						tempcopy++;
						curr_index++;
					}
				}
				res[curr_index] = 0;
				putenv(res);
			}
			temp = strsep(&env_args, ",");
		}

		free(env_args);

		execvp(argv[2], argv + 2);
		perror("fuck: ");
		print_exec_failed();
	}
	else if(env_fork > 0){
		// parent
		waitpid(env_fork, &env_status, 0);
	}
	else{
		print_fork_failed();
		exit(errno);
	}
	return 0;
}