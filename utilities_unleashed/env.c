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
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>


extern char **environ;

char * replace_vars(char * input){
	char *res = malloc(512);
	// char *a = malloc(512);
	char *beg;
	beg = calloc(strlen(input), 1);

	char *var = NULL;
	var = calloc(128, 1);

	char *end;
	end = calloc(strlen(input), 1);
	// printf("29 input: %s\n", input);
	beg = strsep(&input, "%%");
	// printf("31 beg: %s\n", beg);
	// printf("32 input: %s\n", input);
	int i = 0;
	while(*input){ //printf("35 input: %s\n", input);
		if(!(isalpha(*input) || isdigit(*input) || *input == '_')){
			printf("36 var before replacement: %s\n", var);
			printf("input: %s\n", input);
			char *endpart = strdup(input);
			var = getenv(var);
			printf("38 var after replacement: %s\n", var);
			res = strcat(beg, var);
			res = strcat(res, endpart);
			// printf("input: %s\n", input);
			printf("40 res so far: %s\n", res);
			// printf("41 input: %s\n", input);
			input = endpart;
			beg = strsep(&input, "%%");
			printf("input: %s\n", input);
			input = strcat(input, beg);
			res = strcat(res, beg);
			printf("input: %s\n", input);
			printf("43 beg: %s\n", beg);
			var = memset(var, 0, 128);
			// input++;
			i = 0;
			continue;
		}
		printf("49 %c\n", *input);
		printf("50 %s\n", input);
		var[i++] = *input;
		input ++;
	}
	// printf("res at the end:    %s\n", res);
	return res;
}

char ** comma_split(char * input){
	char ** res = NULL;
	int i = 0;
	char *token = NULL;
	while ((token = strsep(&input, ",")) != NULL) {
		res = realloc(res, (i + 1) * sizeof(char *));
		res[i] = malloc(sizeof(token));
		res[i] = replace_vars(token);
		i++;
	}
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
			execvpe(argv[2], argv + 2, env_changes);
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


