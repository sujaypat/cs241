/**
* Chatroom Lab
* CS 241 - Fall 2016
*/
#include "camelCaser_tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"




void print_output(char **output){
	// printf("Got the following:\n");
	if(output){
		char ** line = output;
		while(*line){
			printf("\"%s\"\n", *line);
			line++;
		}
	} else{
		printf("NULL POINTER!\n");
	}
}

void print_input(char * input){
	printf("testing: \n\t'%s'\n\n", input);
}
/*
* Testing function for various implementations of camelCaser.
*
* @param  camelCaser   A pointer to the target camelCaser function.
* @return              Correctness of the program (0 for wrong, 1 for correct).
*/
int test_camelCaser(char **(*camelCaser)(const char *)) {
	// TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
	char * inputs[] = {
		"",
		" ",
		".,.,.,.,.,.,",
		"\t",
		"\n",
		"ayy    lmao.",
		".jet fuel can't melt steel beams!",
		"Hello,... World.",
		"69test 420numbers.",
		"69 test 420 numbers.",
		"memes \n dank.",
		"same tbh. lmao",
		"AYY LMAO. ME tOO thanks.",
		"The Heisenbug is an incredible creature. Facenovel servers get their power from its indeterminism. Code smell can be ignored with INCREDIBLE use of air freshener. God objects are the new religion.",
		NULL
	};
	char * correct[15][15] = {
		{""},
		{""},
		{"", "","",	"",	"",	"",	"", "","",	"",	"", ""},
		{""},
		{""},
		{"ayyLmao"},
		{"", "jetFuelCan", "tMeltSteelBeams"},
		{"hello", "", "", "", "world"},
		{"69test420Numbers"},
		{"69Test420Numbers"},
		{"memesDank"},
		{"sameTbh"},
		{"ayyLmao",
		"meTooThanks"},
		{"theHeisenbugIsAnIncredibleCreature", "facenovelServersGetTheirPowerFromItsIndeterminism", "codeSmellCanBeIgnoredWithIncredibleUseOfAirFreshener", "godObjectsAreTheNewReligion"},
		{NULL}
	};

	char ** input = inputs;
	int c = 0;
	while(*input){
		int i = 0;
		char **output = (*camelCaser)(*input);
		while(output[i]){
			if(strcmp(output[i], correct[c][i])) return 0;
			i++;
		}
		c++;
		input++;
	}
	return 1;
}