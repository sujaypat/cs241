/**
* Chatroom Lab
* CS 241 - Fall 2016
*/
#include "camelCaser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char **camel_caser(const char *input_str) {
	if(!input_str) return NULL;
	char ** res = calloc(1, sizeof(char *));

	int c = 0;
	int i = -1;
	// int numStrings = 0;
	char *input_copy = malloc(strlen(input_str) + 1);
	strcpy(input_copy, input_str);
	char *input_beg = (char *)input_str;
	while(*input_copy){
		// printf("%p\n", &input_str);
		if(ispunct(*input_copy)){
			*input_copy = '\0';
			input_copy++;
			res = realloc(res, (c + 1) * sizeof(char *));
			res[c] = calloc(i + 1, sizeof(char *));
			res[c][0] = '\0';
			strncat(res[c], input_beg, i + 1);

			c++;
			i=-1;
			input_beg = (char *)++input_copy;
		}
		input_copy++;
		i++;
	}
	res = realloc(res, (c + 1) * sizeof(char *));
	res[c + 1] = NULL;

	for(int a = 0; a < c; a++){
		char *word;
		word = strtok(res[a], " ");
		tolower(word[0]);
		while((word = strtok(NULL, " ")) != NULL){
			toupper(word[0]);
		}
	}

	return res;
}