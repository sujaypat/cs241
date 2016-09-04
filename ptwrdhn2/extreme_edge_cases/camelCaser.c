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

	int numStrings = 0;
	int i = -1;

	char *input_copy = malloc(strlen(input_str) + 1);
	strcpy(input_copy, input_str);
	char *input_beg = (char *)input_copy;

	while(*input_copy){
		if(ispunct(*input_copy)){
			*input_copy = '\0';
			if(i == -1){
				i = 0;
			}
			// input_copy++;
			res = realloc(res, (numStrings + 1) * sizeof(char *));
			res[numStrings] = calloc(i + 1, sizeof(char *));
			res[numStrings][0] = '\0';
			strncat(res[numStrings], input_beg, i + 1);

			numStrings++;
			i = -1;
			input_beg = (char *)(input_copy + 1);
		}
		input_copy++;
		i++;
	}
	res = realloc(res, (numStrings + 1) * sizeof(char *));
	res[numStrings] = NULL;
	char isSpaceChars[]=" \t\r\n\v\f";
	for(int a = 0; a < numStrings; a++){
		char *newres = malloc(strlen(res[a]) + 1);
		char *word = NULL;
		word = strtok(res[a], isSpaceChars);

		if(word){
			// lowercase entire word
			for(int w1 = 0; w1 < (int)strlen(word); w1++){
				word[w1] = tolower(word[w1]);
			}
		}

		newres[0] = '\0';
		if(!word) continue;
		strcat(newres, word);
		while((word = strtok(NULL, " ")) != NULL){
			// lowercase the word
			for(int w = 0; w < (int)strlen(word); w++){
				word[w] = tolower(word[w]);
			}
			// uppercase the first isalpha() letter in word
			for(int w2 = 0; w2 < (int)strlen(word); w2++){
				if(isalpha(word[w2])){
					word[w2] = toupper(word[w2]);
					break;
				}
			}
			strcat(newres, word);
		}
		res[a] = newres;
	}
	return res;
}