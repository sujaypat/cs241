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
	printf("%s\n", command);
	char *num = malloc(4 * sizeof(int));
	size_t line_num = 0;
	int i = 0;
	while(*command){
		if(isspace(*command)){
			line_num = atoi(num);
			break;
		}
		num[i++] = *command;
	}
	printf("%zu\n", line_num);
	// size_t len = strlen(command);
	// size_t spn = strcspn(command, "$");

	// if()
	// while $ keep inserting line
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
