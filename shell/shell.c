/**
* Machine Problem: Shell
* CS 241 - Fall 2016
*/
#include "format.h"
#include "log.h"
#include "shell.h"

#include <signal.h>
#include <unistd.h>


void sigint_handler(int sig){
	printf("caught signal SIGINT\n");
	signal(sig, sigint_handler);
}

int shell(int argc, char *argv[]) {
	// TODO: This is the entry point for your shell.
	print_shell_owner("ptwrdhn2");
	signal(SIGINT, sigint_handler);
	while(1){
		print_prompt(getenv(PWD), getpid());

		if(EOF) break;
	}




	return 0;
}