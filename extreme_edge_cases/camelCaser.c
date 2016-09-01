/**
* Chatroom Lab
* CS 241 - Fall 2016
*/
#include "camelCaser.h"
#include <stdlib.h>

char **camel_caser(const char *input_str) { return NULL;
	char *buf = NULL;
	char ** res;
	char* delims = "!&+:?^}\"\',;@_~#(-<[`$).=\\{%%*/>]|";
	// size_t linecap = 0;
	// ssize_t linelen;
	// while((linelen = getline(&buf, &linecap, &input_str)) > 0){
	//
	// }

	int c = 0;
	int i = 0;
	int numStrings = 0;
	while(c != -1){
		// c = fgetc(input_str);
		c = getdelim(&buf, &linecap, delims, input_str);
		strcpy(res[i++], buf);

		// if((isalpha(c) || isdigit(c))){
		// 	strcat(buf, c);
		// }
		// else if((isspace(c) || ispunct(c))){
		// 	*res[numStrings++] = buf;
		// 	buf = NULL;
		// }
		// else{
		// 	// probably NULL
		// }
	}
	for(int a = 0; a < i; a++){
		prinft("%s\n",res[a]);
	}



}
