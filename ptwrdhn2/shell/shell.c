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
int history = 0;
int script = 0;
char *hist_file;
char *script_file;
FILE *f;
int status = 0;
int background = 0;


void sigint_handler(int sig){
	signal(sig, sigint_handler);
}

void cleanup(int signal) {
	while (waitpid((pid_t) (-1), 0, WNOHANG) > 0) {
		printf("cleaning...");
	}
}

void handle_history_file(char *filename){
	hist_file = malloc(1 + strlen(filename));
	hist_file = strcpy(hist_file, filename);
	if(access(hist_file, R_OK) != 1) command_log = Log_create_from_file(filename);
	else print_history_file_error();
	history = 1;
}

void handle_file(char *filename){
	script_file = malloc(1 + strlen(filename));
	script_file = strcpy(script_file, filename);
	if((f = fopen(filename, "rw"))){}
	else print_script_file_error();
	script = 1;
}

void handle_ext_command(char * command){
	Log_add_command(command_log, command);
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
		exit(1);
	}
	else if(p > 0 && !background){
		print_command_executed(p);
		waitpid(p, &status, 0);
	}
	else if(p > 0 && background){
		print_command_executed(p);
	}
	else{
		print_fork_failed();
	}
	if(WIFEXITED(status)){
		if(WEXITSTATUS(status)){
			// print_wait_failed();
		}
	}
	free_args(child_argv);
	free(command_copy);
}

void handle_cd(char *command){
	Log_add_command(command_log, command);
	command += 3;
	status = chdir(command);
	if(status == -1) print_no_directory(command);
	cwd = getcwd(buf, PATH_MAX + 1);
}

void handle_history(){
	for(size_t i = 0; i < Log_size(command_log); i++){
		print_history_line(i, Log_get_command(command_log, i));
	}
}

void handle_spec_history(char *command){
	char *search = strdup(command + 1);
	// char *loc;
	int i;
	int found = 0;
	for(i = (int)Log_size(command_log) - 1; i >= 0; i--){
		// if((loc = strstr(Log_get_command(command_log, i), search)) != NULL){
		if(!strncmp(Log_get_command(command_log, i), search, strlen(search))){
			found = 1;
			break;
		}
	}
	if(!found) print_no_history_match();
	else{
		print_command(Log_get_command(command_log, i));
		if(!strncmp((char *)Log_get_command(command_log, i), "cd", 2)) handle_cd((char *)Log_get_command(command_log, i));
		else handle_ext_command((char *)Log_get_command(command_log, i));
	}
	free(search);
	search = NULL;
}

void handle_num_history(char *command){
	command++;
	char *line = strsep(&command, " ");
	int num = atoi(line);
	char *found;
	if((size_t)num < Log_size(command_log) && num >= 0){
		found = malloc(1 + strlen(Log_get_command(command_log, num)));
		strcpy(found, Log_get_command(command_log, num));
	}
	else{
		print_invalid_index();
		return;
	}
	print_command(found);
	if(!strncmp(found, "cd", 2)) handle_cd(found);
	else handle_ext_command(found);

	free(found);
}


int shell(int argc, char *argv[]) {

	extern char *optarg;
	int opt;
	if(argc != 1){
		while ((opt = getopt(argc, argv, "h:f:")) != -1) {
			switch (opt) {
				case 'h':
				handle_history_file(optarg);
				break;
				case 'f':
				handle_file(optarg);
				break;
				default:
				print_usage();
			}
		}
	}

	if(!history) command_log = Log_create();

	cwd = getcwd(buf, PATH_MAX + 1);
	signal(SIGINT, sigint_handler);

	char *command = NULL;
	size_t len = 0;
	int done = 0;
	int eof;

	while (!done) {

		print_prompt(cwd, getpid());
		if(!script) eof = getline(&command, &len, stdin);
		else eof = getline(&command, &len, f);
		if(eof == -1) break;

		char *nl = strchr(command, '\n');
		if(nl) *nl = 0;

		if(script) print_command(command);
		if(command && len){
			if(!strncmp(command, "cd", 2)) handle_cd(command);
			else if(!strncmp(command, "!history", 8)) handle_history();
			else if(!strncmp(command, "#", 1)) handle_num_history(command);
			else if(!strncmp(command, "!", 1)) handle_spec_history(command);
			else handle_ext_command(command);
		}
	}
	signal(SIGCHLD, cleanup);
	if(command) free(command);
	if(history){
		Log_save(command_log, hist_file);
		free(hist_file);
	}
	if(script) free(script_file);
	if(f) fclose(f);
	Log_destroy(command_log);
	return 0;
}