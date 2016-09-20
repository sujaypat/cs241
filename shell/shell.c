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


void sigint_handler(int sig){
	printf("caught signal SIGINT\n");
	signal(sig, sigint_handler);
}
void sigeof_handler(int sig){
	printf("caught EOF\n");
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
	print_shell_owner("ptwrdhn2");
	signal(SIGINT, sigint_handler);


	char *command = NULL;
	size_t len = 0;
	int done = 0;

	while (!done) {
		print_prompt(getenv("PWD"), getpid());
		getline(&command, &len, stdin);

		// remove newline from the command
		char *nl = strchr(command, '\n');
		if (nl)
		*nl = 0;


		int command_type = command[0];
		switch (command_type) {
			case 'cd':
			chdir(command[1]);
			// handle_display_command(document, command);
			break;
			case 'w':
			// handle_write_command(document, command);
			break;
			case 'a':
			// handle_append_command(document, command);
			break;
			case 'd':
			// handle_delete_command(document, command);
			break;
			case '/':
			// handle_search_command(document, command);
			break;
			case 's':
			// if (strlen(command) == 1) {
			// 	handle_save_command(document, filename);
			// } else {
			// 	invalid_command(command);
			// }
			break;
			case 'q':
			// done = 1;
			// Document_destroy(document);
			break;
			default:
			fprintf(stderr, "Something bad happened! Report to CS241 Staff!\n");
			break;
		}
	}

	// Need to free the buffer that we created.
	if (command) {
		free(command);
	}

	return 0;
}