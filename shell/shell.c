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
#include <limits.h>

Log *command_log = NULL;
char *cwd;
int status = 0;
void sigint_handler(int sig){
	printf("caught signal SIGINT\n");
	signal(sig, sigint_handler);
}

void handle_history_file(char *filename){
	printf("history file %s created\n", filename);

}

void handle_file(char *filename){
	printf("file %s imported\n", filename);
}

void handle_cd(char *command){
	// puts("cd");
	print_command(command);
	Log_add_command(command_log, command);
	command += 3;
	// printf("%s\n", getenv("PWD"));
	status = chdir(command);
	// printf("%s\n", getenv("PWD"));
	if(status == -1) print_no_directory(command);
}

void handle_spec_history(char *command){
	char *search = strdup(command + 1);
	char *loc;
	for(int i = Log_size(command_log) - 1; i >= 0; i--){
		if((loc = strstr(Log_get_command(command_log, i), search)) != NULL){
			print_history_line(i, Log_get_command(command_log, i));
		}
	}
	free(search);
	search = NULL;
}

void handle_history(){
	for(size_t i = 0; i < Log_size(command_log); i++){
		print_history_line(i, Log_get_command(command_log, i));
	}
}

void handle_num_history(char *command){
	command++;
	char *line = strsep(&command, " ");
	int num = atoi(line);
	if((size_t)num < Log_size(command_log) && num >= 0){
		print_command(Log_get_command(command_log, num));
	}
	else print_invalid_index();


	free(line);
}

int shell(int argc, char *argv[]) {
	// TODO: This is the entry point for your shell.
	if(argc == 3){
		if(!strcmp(argv[argc - 2], "-h")) handle_history_file(argv[argc - 1]);
		else if(!strcmp(argv[argc - 2], "-f")) handle_file(argv[argc - 1]);
	}
	if(argc != 3 && argc != 1) 	print_usage();

	command_log = Log_create();
	char buf[PATH_MAX + 1];
	cwd = getcwd(buf, PATH_MAX + 1);

	signal(SIGINT, sigint_handler);


	char *command = NULL;
	size_t len = 0;
	// int dicks = 0;
	int done = 0;

	while (!done) {
		print_prompt(cwd, getpid());
		int eof = getline(&command, &len, stdin);
		if(eof == -1) break;
		// remove newline from the command
		char *nl = strchr(command, '\n');
		if (nl) *nl = 0;

		// printf("command: %s\n", command);
		if(!strncmp(command, "cd", 2)){
			handle_cd(command);
		}
		else if(!strncmp(command, "!history", 8)){
			handle_history();
		}
		else if(strncmp(command, "#", 1)){
			puts("specfic history");
			// only for returned cmd if found and run
			handle_num_history(command);
			// Log_add_command(command_log, command);
		}
		else if(strncmp(command, "!", 1)){
			puts("repeat");
			handle_spec_history(command);
			// only for returned cmd if found and run
			// Log_add_command(command_log, command);
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