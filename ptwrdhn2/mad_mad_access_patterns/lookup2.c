/**
* Mad Mad Access Pattern
* CS 241 - Fall 2016
*/
#include "tree.h"
#include "utils.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

/*
Look up a few nodes in the tree and print the info they contain.
This version uses mmap to access the data.

./lookup2 <data_file> <word> [<word> ...]
*/

int main(int argc, char **argv) {
	// argv[1] = filename, argv[2] = keyword, argv[3+] = optional more keywords
	if(argc < 3){
		printArgumentUsage();
		exit(1);
	}
	struct stat statinfo;
	int staterr = stat(argv[1], &statinfo);
	if(staterr == -1){
		openFail(argv[1]);
		exit(1);
	}
	size_t filesize = statinfo.st_size;
	int openfile = open(argv[1], O_RDONLY);
	void *addr = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, openfile, 0);
	if(!addr){
		openFail(argv[1]);
		exit(1);
	}
	if(strncmp((char *)addr, BINTREE_HEADER_STRING, 4) != 0){
		formatFail(argv[1]);
		exit(2);
	}
	for(int i = 2; i < argc; i++){

		BinaryTreeNode *temp = addr + BINTREE_ROOT_NODE_OFFSET;
		int flag = 0;
		while(1){
			int comp = strcmp(temp -> word, argv[i]);
			if(!comp){
				printFound(argv[i], temp -> count, temp -> price);
				flag = 1;
				break;
			}
			else if(comp < 0){
				if(temp -> right_child == 0) break;
				temp = addr + temp -> right_child;
			}
			else{
				if(temp -> left_child == 0) break;
				temp = addr + temp -> left_child;
			}
		}
		if (!flag){
			printNotFound(argv[i]);
		}

	}
	int munmaperr = munmap(addr, filesize);
	if(munmaperr == -1){
		printf("rip\n");
	}
	return 0;
}