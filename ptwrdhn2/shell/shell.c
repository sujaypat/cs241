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
#include <sys/wait.h>

Log *command_log = NULL;
char *cwd;
char buf[PATH_MAX + 1];
int status = 0;

void sigint_handler(int sig){
	signal(sig, sigint_handler);
}

void handle_history_file(char *filename){
	printf("history file %s created\n", filename);
}

void handle_file(char *filename){
	printf("script %s imported\n", filename);
}

void handle_cd(char *command){
	Log_add_command(command_log, command);
	command += 3;
	status = chdir(command);
	if(status == -1) print_no_directory(command);
	cwd = getcwd(buf, PATH_MAX + 1);
}

void handle_spec_history(char *command){
	char *search = strdup(command + 1);
	char *loc;
	int i;
	for(i = Log_size(command_log) - 1; i >= 0; i--){
		if((loc = strstr(Log_get_command(command_log, i), search)) != NULL){
			// printf("%s\n", Log_get_command(command_log, i));
			break;
		}
	}
	if(!i) print_no_history_match();
	else{
		print_command(command);
		handle_ext_command(command);
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
	char *found;
	if((size_t)num < Log_size(command_log) && num >= 0){
		found = malloc(strlen(Log_get_command(command_log, num)));
		if(!found)
		strcpy(found, Log_get_command(command_log, num));
	}
	else{
		print_invalid_index();
		return;
	}
	Log_add_command(command_log, found);

	free(found);
}

void handle_ext_command(char * command){
	Log_add_command(command_log, command);
	int background = 0;
	if(!strncmp(command + strlen(command) - 1, "&", 1)){
		background = 1;
		command[strlen(command) - 1] = '\0';
	}
	size_t tokens = 0;
	char *command_copy = strdup(command);
	char ** child_argv = strsplit(command, " ", &tokens);
	pid_t p = fork();
	int status = 0;
	if(p == 0){
		execvp(child_argv[0], child_argv);
		print_exec_failed(command_copy);
	}
	else if(p > 0){// && !background){
		print_command_executed(p);
		waitpid(p, &status, 0);
	}
	// else if(p > 0 && background){
	//
	// }
	else{
		print_fork_failed();
	}
	if(WIFEXITED(status)){
		if(WEXITSTATUS(status)) print_wait_failed();
	}
	free_args(child_argv);
	free(command_copy);
}

int shell(int argc, char *argv[]) {
	// TODO: This is the entry point for your shell.
	// argc_copy = argc;
	if(argc == 3){
		if(!strcmp(argv[argc - 2], "-h")) handle_history_file(argv[argc - 1]);
		else if(!strcmp(argv[argc - 2], "-f")) handle_file(argv[argc - 1]);
	}
	if(argc != 3 && argc != 1) 	print_usage();

	command_log = Log_create();

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

		if(!strncmp(command, "cd", 2)) handle_cd(command);
		else if(!strncmp(command, "!history", 8)) handle_history();
		else if(!strncmp(command, "#", 1)) handle_num_history(command);
		else if(!strncmp(command, "!", 1)) handle_spec_history(command);
		else handle_ext_command(command);

	}
	if (command) {
		free(command);
	}
	return 0;
}