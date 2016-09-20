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
	}
	if(argc != 3 && argc != 1) 	print_usage();

	command_log = Log_create();
	print_shell_owner("ptwrdhn2");
	signal(SIGINT, sigint_handler);


	char *command = NULL;
	size_t len = 0;
	size_t tokens = 0;
	int done = 0;
	int status = 0;

	while (!done) {
		print_prompt(getenv("PWD"), getpid());
		int eof = getline(&command, &len, stdin);
		if(eof == -1) break;
		// remove newline from the command
		char *nl = strchr(command, '\n');
		if (nl) *nl = 0;
		char ** args = strsplit(command, " ", &tokens);

		char *command_type = args[0];
		printf("command: %s\n", command_type);
		if(!strcmp(command_type, "cd")){
			puts("cd");
			printf("%s\n", args[1]);
			status = chdir(args[1]);
			Log_add_command(command_log, command);
			if(status == -1) print_no_directory(args[1]);
		}
		else if(!strcmp(command_type, "!history")){
			for(size_t i = 0; i < Log_size(command_log); i++){
				print_history_line(i, Log_get_command(command_log, i));
			}
			puts("history");
		}
		else if(strstr(command_type, "#")){
			puts("specfic history");
			Log_add_command(command_log, command);
		}
		else if(strstr(command_type, "!")){
			puts("repeat");
			Log_add_command(command_log, command);
		}
		else{
			puts("u dun fucked up");
			//fork, exec, wait
		}
	}

	if (command) {
		// free(command);
	}

	return 0;
}