/**
* Map Reduce 0 Lab
* CS 241 - Fall 2016
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

int main(int argc, char **argv) {
	if(argc != 5){
		print_usage();
		exit(1);
	}
	int r_status = 0;
	int m_status = 0;
	// Open the input file.
	int input = open(argv[1], O_RDONLY);
	// descriptors_add(input);
	// Create a pipe to connect the mapper to the reducer.
	int pipes[2]; // 0 is read end, 1 is write end
	pipe(pipes);
	// Open the output file.
	int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IWUSR | S_IRUSR);
	// descriptors_add(output);
	// Start the mapper.
	pid_t map_child = fork();
	if(map_child == 0){
		//run mapper
		dup2(input, fileno(stdin));
		dup2(pipes[1], fileno(stdout));
		close(pipes[0]);
		close(output);
		execl(argv[3], argv[3], NULL);
	}
	// Start the reducer.

	pid_t reduce_child = fork();
	if(reduce_child == 0){
		//run reducer
		dup2(pipes[0], fileno(stdin));
		dup2(output, fileno(stdout));
		close(pipes[1]);
		close(input);
		execl(argv[4], argv[4], NULL);
	}
	// Wait for the reducer to finish.
	close(pipes[0]);
	close(pipes[1]);

	waitpid(map_child, &m_status, 0);
	waitpid(reduce_child, &r_status, 0);
	// Print nonzero subprocess exit codes.
	if(m_status) printf("my_mapper exited with status \n", m_status);
	if(r_status) printf("my_reducer exited with status \n", r_status);
	close(input);
	close(output);
	// Count the number of lines in the output file.
	print_num_lines(argv[2]);

	return 0;
}
