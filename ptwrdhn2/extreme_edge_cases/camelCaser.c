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
	// char *buf = NULL;
	char ** res = malloc(strlen(input_str) * sizeof(char *));
	// size_t linecap = 0;
	// ssize_t linelen;

	int c = 0;
	int i = 0;
	// int numStrings = 0;
	char *input_beg = (char *)input_str;
	while(input_str){

		if(ispunct(*input_str)){
			res[c] = calloc(i + 1, sizeof(char *));
			strncat(res[c], input_beg, i);

			c++;
			i=-1;
			input_beg = (char *)++input_str;

		}
		input_str++;
		i++;
		// continue;
	}

	for(int a = 0; a < i; a++){
		printf("%s\n",res[a]);
	}

	return res;


}
