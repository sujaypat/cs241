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
/*
Look up a few nodes in the tree and print the info they contain.
This version uses fseek() and fread() to access the data.

./lookup1 <data_file> <word> [<word> ...]
*/

int main(int argc, char **argv) {
	// argv[1] = filename, argv[2] = keyword, argv[3+] = optional more keywords
	if(argc < 3){
		printArgumentUsage();
		exit(1);
	}
	FILE *f = fopen(argv[1], "r");
	if(!f){
		openFail(argv[1]);
		exit(2);
	}
	char *first_four = calloc(5, 1);
	fread(first_four, BINTREE_ROOT_NODE_OFFSET, 1, f);
	if(strcmp(BINTREE_HEADER_STRING, first_four)){
		formatFail(argv[1]);
		exit(2);
	}

	for(int i = 2; i < argc; i++){
		fseek(f, BINTREE_ROOT_NODE_OFFSET + sizeof(BinaryTreeNode), SEEK_SET);
		int terminator = 0;
		while(1){
			// first char of word = curr + 16
			char temp = fgetc(f);
			terminator++;
			if(temp == '\0') break;
		}

		BinaryTreeNode *temp = calloc(sizeof(BinaryTreeNode) + terminator, 1);
		fseek(f, BINTREE_ROOT_NODE_OFFSET, SEEK_SET);
		fread(temp, sizeof(BinaryTreeNode) + terminator, 1, f);

		int flag = 0;

		while(1){
			int comp = strcmp(temp -> word, argv[i]);
			if(!comp){
				printFound(argv[i], temp -> count, temp -> price);
				flag = 1;
				free(temp);
				break;
			}
			else if(comp < 0){
				if(temp -> right_child == 0) break;
				fseek(f, temp -> right_child + sizeof(BinaryTreeNode), SEEK_SET);
				int terminator = 0;
				while(1){
					// first char of word = curr + 16
					char temp = fgetc(f);
					terminator++;
					if(temp == '\0') break;
				}
				temp = realloc(temp, sizeof(BinaryTreeNode) + terminator);
				fseek(f, temp -> right_child, SEEK_SET);
				fread(temp, sizeof(BinaryTreeNode) + terminator, 1, f);
			}
			else{
				if(temp -> left_child == 0) break;
				fseek(f, temp -> left_child + sizeof(BinaryTreeNode), SEEK_SET);
				int terminator = 0;
				while(1){
					// first char of word = curr + 16
					char temp = fgetc(f);
					terminator++;
					if(temp == '\0') break;
				}
				temp = realloc(temp, sizeof(BinaryTreeNode) + terminator);
				fseek(f, temp -> left_child, SEEK_SET);
				fread(temp, sizeof(BinaryTreeNode) + terminator, 1, f);
			}
		}
		if (!flag){
			printNotFound(argv[i]);
			if(temp) free(temp);
		}

	}
	free(first_four);
	fclose(f);
	return 0;
}