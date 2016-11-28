/**
* MapReduce
* CS 241 - Fall 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"

void usage() {
	fprintf(stderr, "shuffler destination1 destination2 ...\n");
	fprintf(stderr, "where destination1..n are files on the filesystem to which "
	"the shuffler will write its output\n");
}

int main(int argc, char *argv[]) {
	// read from stdin
	// hash the key for the input line
	// send them to the correct output file (output files are given as command
	// line arguments
	FILE *output_files[argc - 1];
	for(int i = 1; i < argc; i++){
		//printf("%s\n", argv[i]);
		output_files[i - 1] = fopen(argv[i], "w");
	}
	size_t N = argc - 1;
	if (argc < 2) {
		usage();
		exit(1);
	}
	char *line = NULL;
	size_t len = 0;
	while(getline(&line, &len, stdin) != EOF){
		//fprintf(stderr, "reading line %s\n", line);
		char *key = NULL;
		char *value = NULL;
		split_key_value(line, &key, &value);
		FILE *outf = output_files[hashKey(key) % N];

		//fprintf(stderr, "%s: %s\n", key, value);

		fprintf(outf, "%s: %s\n", key, value);
		//fprintf(stderr,"i just sent that shit straight to the fifo\n");
		line = NULL;
		len = 0;
		
	}
	//if(line) free(line);
	
//	for(int i = 1; i < argc; i++){
		//printf("%s\n", argv[i]);
//		fclose(output_files[i]);
//	}
	//fprintf(stderr,"exiting the shuffler\n");
	return 0;
}
