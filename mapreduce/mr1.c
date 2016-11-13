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
#include <sys/wait.h>
#include <unistd.h>

void usage() { print_mr1_usage(); }

int main(int argc, char **argv) { //

	if(argc != 6){
		usage();
		exit(1);
	}
	int red_status = 0;
	size_t num_mappers = atoi(argv[5]);
	pid_t mappers[num_mappers];
	pid_t splitters[num_mappers];
	// setting up pipes
	int pipes[num_mappers * 2];
	int reduce[2];
	pipe2(reduce, O_CLOEXEC);
	// Create an input pipe for each mapper.
	for (size_t i = 0; i < num_mappers; i++) {
		pipe2(&(pipes[i * 2]), O_CLOEXEC); // read, write
	}

	for (size_t i = 0; i < num_mappers; i++) {
		splitters[i] = fork();
		if (splitters[i] < 0) {
			perror("fork failed: ");
		}
		else if(splitters[i] == 0){
			dup2(pipes[(2 * i) + 1], fileno(stdout));
			// fprintf(stderr, "what the split fuck %zu\n", ((i*2)+1));
			close(pipes[2 * i]);
			close(reduce[0]);
			close(reduce[1]);

			char iter[10];
			sprintf(iter, "%zu", i);
			// fprintf(stderr, "iter: %s\n", iter);

			execl("./splitter", "./splitter", argv[1], argv[5], iter, NULL);
			perror("split exec failed: ");
		}
		else{
			mappers[i] = fork();
			if (mappers[i] < 0) {
				perror("fork failed: ");
			}
			else if(mappers[i] == 0){
				// fprintf(stderr, "what the map fuck %zu\n", ((i*2)));
				dup2(pipes[2 * i], fileno(stdin));
				dup2(reduce[1], fileno(stdout));
				close(pipes[(2 * i) + 1]);
				close(reduce[0]);
				execl(argv[3], argv[3], NULL);
				perror("map exec failed: ");
			}
			close(pipes[(2 * i) + 1]);
			close(pipes[(2 * i)]);
		}
	}
	pid_t red = fork();
	if (red < 0) {
		perror("fork failed: ");
	}
	else if(red == 0){
		int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR);
		// fprintf(stderr, "output descriptor: %d\n", output);
		dup2(reduce[0], fileno(stdin));
		dup2(output, fileno(stdout));

		close(reduce[1]);
		execl(argv[4], argv[4], NULL);
		perror("reducer exec failed: ");
	}
	else {
		close(reduce[1]);
		close(reduce[0]);
	}


	// Wait for the reducer to finish.
	for (size_t map = 0; map < num_mappers; map++) {
		int split_return;
		int map_return;
		waitpid(splitters[map], &split_return, 0);
		waitpid(mappers[map], &map_return, 0);
		if(split_return) print_nonzero_exit_status("splitter", split_return);
		if(map_return) print_nonzero_exit_status(argv[3], map_return);
	}
	waitpid(red, &red_status, 0);
	if(red_status) print_nonzero_exit_status(argv[4], red_status);

	print_num_lines(argv[2]);

	return 0;
}