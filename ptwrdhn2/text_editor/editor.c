/**
* Machine Problem: Text Editor
* CS 241 - Fall 2016
*/

#include "document.h"
#include "editor.h"
#include "format.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_filename(int argc, char *argv[]) {
	return argc == 2 ? argv[argc - 1] : NULL;
	// take a look at editor_main.c to see what this is used for
}

void handle_display_command(Document *document, const char *command) {
	if(!Document_size(document)) print_document_empty_error();

	if(!strcmp(command, "p")){
		//print full document if exists
		for(size_t i = 1; i <= Document_size(document); i++){
			print_line(document, i);
		}
	}
	else{
		int line_num = atoi(command + 2);
		for(int j = line_num - 5; j <= line_num + 5; j++){
			if(j > 0 && j <= (int)Document_size(document)){
				print_line(document, j);
			}
		}
	}
}

void handle_write_command(Document *document, const char *command) {
	// TODO implement handle_write_command
	char *input = (char *)(command + 2);
	char *num = strsep(&input, " ");
	int line_num = atoi(num);
	printf("num string: %s\n", num);
	char **res = calloc(1, strlen(input));
	int j = 0;
	int curr = 0;
	for(size_t init = 0; init < strlen(input); init++){
		res[init] = calloc(1, strlen(input));
	}
	for(size_t i = 0; i < strlen(input); i++){
		if(input[i] == '$'){
			j++;
			curr = 0;
			i++;
		}
		res[j][curr++] = input[i];
		// printf("i: %zu\n", i);
		printf("input i: %c\n", input[i]);
	}
	for(size_t print = 0; print <= (size_t)j; print++){
		Document_set_line(document, line_num++, res[print]);
		free(res[print]);
	}
	free(res);
	// free(num);
	free(input);
	res = NULL;
	num = NULL;
	input = NULL;
}

void handle_append_command(Document *document, const char *command) {
	// TODO implement handle_append_command
}

void handle_delete_command(Document *document, const char *command) {
	// TODO implement handle_delete_command
}

void handle_search_command(Document *document, const char *command) {
	// TODO implement handle_search_command
}

void handle_save_command(Document *document, const char *filename) {
	// TODO implement handle_save_command
}
