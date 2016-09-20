/**
* Machine Problem: Shell
* CS 241 - Fall 2016
*/
#include "format.h"
#include "log.h"
#include "shell.h"

#include <signal.h>
#include <unistd.h>
#include <string.h>

Log *command_log = NULL;
void sigint_handler(int sig){
	printf("caught signal SIGINT\n");
	signal(sig, sigint_handler);
}

void handle_history(char *filename){
	printf("history file %s created\n", filename);

}

void handle_file(char *filename){
	printf("file %s imported\n", filename);
}


int shell(int argc, char *argv[]) {
	// TODO: This is the entry point for your shell.
	if(argc == 3){
		if(!strcmp(argv[argc - 2], "-h")) handle_history(argv[argc - 1]);
		else if(!strcmp(argv[argc - 2], "-f")) handle_file(argv[argc - 1]);
		else print_usage();
	}
	command_log = Log_create();
	print_shell_owner("ptwrdhn2");
	signal(SIGINT, sigint_handler);


	char *command = NULL;
	size_t len = 0;
	size_t tokens = 0;
	int done = 0;

	while (!done) {
		print_prompt(getenv("PWD"), getpid());
		int eof = getline(&command, &len, stdin);
		if(eof == -1) break;
		char ** args = strsplit(command, " ", &tokens);

		// remove newline from the command
		char *nl = strchr(command, '\n');
		if (nl) *nl = 0;

		char *command_type = args[0];
		if(!strcmp(command_type, "cd")){
			puts("cd\n");
			chdir(args[1]);
		}
		else if(!strcmp(command_type, "!history")){
			// for(int i = 0; i < )
			puts("history\n");
		}
		else if(strstr(command_type, "#")){
			puts("specfic history\n");
		}
		else if(strstr(command_type, "!")){
			puts("repeat\n");
		}
		else{
			puts("u dun fucked un");
			//fork, exec, wait
		}
	}

	if (command) {
		// free(command);
	}

	return 0;
}