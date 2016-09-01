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

		if(!ispunct(*input_str)){
			input_str++;
			i++;
		}
		res[c] = malloc(strlen(input_beg) * sizeof(char));
		for(int b = 0; b < i; b++){
			strcat(res[c], (const char *)(input_beg + b));
		}
		c++;
	}
	for(int a = 0; a < i; a++){
		printf("%s\n",res[a]);
	}

	return res;


}
