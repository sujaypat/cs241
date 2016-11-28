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
#include <errno.h>

void usage() { print_mr2_usage(); }

int main(int argc, char **argv) {

	if(argc != 7){
		usage();
		exit(1);
	}

	size_t num_mappers = atoi(argv[5]);
	size_t num_reducers = atoi(argv[6]);
	pid_t mappers[num_mappers];
	pid_t splitters[num_mappers];
	pid_t reducers[num_reducers];

	// setting up pipes
	int pipes[num_mappers * 2];
	//int red[num_reducers * 2];
	char *fifos[num_reducers + 2];
	int shuff[2];
	pipe(shuff);
	// splitter -> mapper pipes
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
			close(pipes[2 * i]);
			close(shuff[0]);
			close(shuff[1]);

			// itoa-ing num mappers to string
			char iter[10];
			sprintf(iter, "%zu", i);
			// fprintf(stderr, "%s %s %s %s\n", "./splitter", argv[1], argv[5], iter);
			execl("./splitter", "./splitter", argv[1], argv[5], iter, NULL);
			perror("split exec failed: ");
		}
		else{
			mappers[i] = fork();
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
	//fprintf(stderr, "mapper:I am %d\n", getpid());
	

	// shuffler -> reducer pipes
	fifos[0] = "./shuffler";
	fifos[num_reducers + 1] = NULL;
	for (size_t i = 0; i < num_reducers; i++) {
		//pipe2(&(red[i * 2]), O_CLOEXEC); // read, write
		char beg[10];
		sprintf(beg, "./fifo_%zu", i);
		// printf("%s\n", beg);
		mkfifo(beg, S_IRWXU);
		fifos[i + 1] = strdup(beg);
	}

	pid_t shuffler = fork();
	if(shuffler < 0){
		perror("fork failed: ");
	}
	else if(shuffler == 0){
		dup2(shuff[0], fileno(stdin));
		close(shuff[1]);
		//fprintf(stderr, "about to exec shuffler\n");
		/*for(size_t i = 1; i < num_reducers + 1; i++){
			fprintf(stderr, "fifos: %s\n", fifos[i]);
		}*/
		execv("./shuffler", fifos);
		//fprintf(stderr,"Your mark ass failed to execute the shuffler\n");
	}
	else{
		close(shuff[1]);
		close(shuff[0]);
	}
	//fprintf(stderr, "shuffler:I am %d\n", getpid());
	
	int output = open(argv[2], O_TRUNC | O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	//fprintf(stderr, "errno: %s\n", strerror(errno));

	for (size_t i = 0; i < num_reducers; i++) {
		reducers[i] = fork();
		if (reducers[i] < 0) {
			perror("fork failed: ");
		}
		else if(reducers[i] == 0){
			//fprintf(stderr, "output: %d, argv2: %s\n", output, argv[2]);
			dup2(output, fileno(stdout));
			int curr_fifo = open(fifos[i + 1], O_RDONLY, S_IRUSR);
			//fprintf(stderr,"for the pid %d and file %s, the curr_fifo fd number is %d\n",getpid(), fifos[i+1], curr_fifo);
			dup2(curr_fifo, fileno(stdin));
			//close(pipes[2 * i]);
			//close(red[2 * i]);
			//close(red[(2 * i) + 1]);

			execl(argv[4], argv[4], NULL);
			perror("reducer exec failed: ");
		}
		else{
			//close(red[(2 * i) + 1]);
			//close(red[(2 * i)]);
		}
	}
	//fprintf(stderr, "reducer:I am %d\n", getpid());
	for(size_t red = 0; red < num_reducers; red++){
		//fprintf(stderr, "waiting for reducer: %zu pid %d\n", red, reducers[red]);
		int red_status;
		waitpid(reducers[red], &red_status, 0);
		if(red_status) print_nonzero_exit_status(argv[4], red_status);
		unlink(fifos[red + 1]);
	}
	
	for (size_t map = 0; map < num_mappers; map++) {
		int split_return;
		int map_return;
		waitpid(splitters[map], &split_return, 0);
		waitpid(mappers[map], &map_return, 0);
		if(split_return) print_nonzero_exit_status("splitter", split_return);
		if(map_return) print_nonzero_exit_status(argv[3], map_return);
	}
	int shuf_status;
	waitpid(shuffler, &shuf_status, 0);
	if(shuf_status) print_nonzero_exit_status("shuffler", shuf_status);
	close(output);
	print_num_lines(argv[2]);
	for(size_t i = 1; i < num_reducers + 1; i++){
		free(fifos[i]);
	}
	return 0;
}
