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
	char *start = (char *)(command + 2);
	printf("%p\n", &start);
	printf("%s\n", start);
	char *num = strsep(&start, " ");
	int line_num = atoi(num);
	char *end = start;
	char *res;
	int length = 0;

	while(*end){
		if(*end == '$'){
			res = calloc(1, length + 1);
			strncpy(res, start, length);
			if((size_t)line_num > Document_size(document)) Document_insert_line(document, line_num++, res);
			else Document_set_line(document, line_num++, res);
			free(res);
			res = NULL;
			length = 0;
			start = ++end;
			continue;
		}
		end++;
		length++;
	}
	res = calloc(1, length + 1);
	strncpy(res, start, length);
	if((size_t)line_num > Document_size(document)) Document_insert_line(document, line_num++, res);
	else Document_set_line(document, line_num++, res);
	free(res);
	res = NULL;
	num = NULL;
	start = NULL;
	end = NULL;
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
	Document_write_to_file(document, filename);
}
