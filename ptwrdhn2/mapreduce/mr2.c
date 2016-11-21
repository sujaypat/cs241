/**
* MapReduce
* CS 241 - Fall 2016
*/

#include "common.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void usage() { print_mr2_usage(); }

int main(int argc, char **argv) {

	if(argc != 7){
		usage();
		exit(1);
	}

	size_t num_mappers = atoi(argv[5]);
	size_t num_reducers = atoi[6];
	pid_t mappers[num_mappers];
	pid_t splitters[num_mappers];
	pid_t reducers[num_reducers];

	// setting up pipes
	int pipes[num_mappers * 2];
	int red[num_reducers * 2];
	int shuff[2];
	pipe(shuff);
	// splitter -> mapper pipes
	for (size_t i = 0; i < num_mappers; i++) {
		pipe2(&(pipes[i * 2]), O_CLOEXEC); // read, write
	}

	for (size_t i = 0; i < num_mappers; i++) {
		// splitters[i] = fork();
		if (splitters[i] < 0) {
			perror("fork failed: ");
		}
		else if(splitters[i] == 0){
			dup2(pipes[(2 * i) + 1], fileno(stdout));
			close(pipes[2 * i]);
			close(shuff[0]);
			close(shuff[1]);

			// tkaing num mappers to string
			char iter[10];
			sprintf(iter, "%zu", i);

			execl("./splitter", "./splitter", argv[1], argv[5], iter, NULL);
			perror("split exec failed: ");
		}
		else{
			// mappers[i] = fork();
			if (mappers[i] < 0) {
				perror("fork failed: ");
			}
			else if(mappers[i] == 0){
				dup2(pipes[2 * i], fileno(stdin));
				dup2(shuff[1], fileno(stdout));
				close(pipes[(2 * i) + 1]);
				close(shuff[0]);
				execl(argv[3], argv[3], NULL);
				perror("map exec failed: ");
			}
			close(pipes[(2 * i) + 1]);
			close(pipes[(2 * i)]);
		}
	}

	// shuffler -> reducer pipes
	for (size_t i = 0; i < num_reducers; i++) {
		pipe2(&(red[i * 2]), O_CLOEXEC); // read, write
	}


	// pid_t shuffler = fork();
	if(shuffler < 0){
		perror("fork failed: ");
	}
	else if(shuffler == 0){

	}
	else{

	}


	for (size_t i = 0; i < num_reducers; i++) {
		// reducers[i] = fork();
		if (reducers[i] < 0) {
			perror("fork failed: ");
		}
		else if(reducers[i] == 0){
			dup2(pipes[(2 * i) + 1], fileno(stdout));
			close(pipes[2 * i]);
			close(red[2 * i]);
			close(red[(2 * i) + 1]);


			execl(argv[4], argv[4], NULL);
			perror("reducer exec failed: ");
		}
		else{
			close(red[(2 * i) + 1]);
			close(red[(2 * i)]);
		}
	}


	for (size_t map = 0; map < num_mappers; map++) {
		int split_return;
		int map_return;
		waitpid(splitters[map], &split_return, 0);
		waitpid(mappers[map], &map_return, 0);
		if(split_return) print_nonzero_exit_status("splitter", split_return);
		if(map_return) print_nonzero_exit_status(argv[3], map_return);
	}
	for(size_t red = 0; red < num_reducers; red++){
		int red_status;
		waitpid(reducers[red], &red_status, 0);
		if(red_status) print_nonzero_exit_status(argv[4], red_status);
	}

	print_num_lines(argv[2]);

	return 0;

}
