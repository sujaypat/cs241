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
	if(!Document_size(document)){
		print_document_empty_error();
		return;
	}
	if(!strcmp(command, "p")){
		//print full document if exists
		for(size_t i = 1; i <= Document_size(document); i++){
			print_line(document, i);
		}
	}
	else{
		char *start = strdup(command + 2);
		char *num = strsep(&start, " ");
		int line_num = atoi(num);
		if((size_t)line_num > Document_size(document) || line_num < 1){
			invalid_line();
			return;
		}
		for(int j = line_num - 5; j <= line_num + 5; j++){
			if(j > 0 && j <= (int)Document_size(document)){
				print_line(document, j);
			}
		}
	}
}

void handle_write_command(Document *document, const char *command) {
	char *start = strdup(command + 2);
	char *num = strsep(&start, " ");
	int line_num = atoi(num);
	if(line_num < 1){
		invalid_line();
		return;
	}
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
	free(num);
	res = NULL;
	num = NULL;
	start = NULL;
	end = NULL;
}

void handle_append_command(Document *document, const char *command) {
	char *start = strdup(command + 2);
	char *num = strsep(&start, " ");
	int line_num = atoi(num);
	if(line_num < 1){
		invalid_line();
		return;
	}
	char *end = start;
	char *res;
	int length = 0;
	int count = 0;
	while(*end){
		if(*end == '$'){
			res = calloc(1, length + 1);
			strncpy(res, start, length);
			if((size_t)line_num > Document_size(document)){
				Document_insert_line(document, line_num++, res);
			}
			else {
				if(count++ == 0){
					char *tmpres = strdup(res);
					strcpy(res, Document_get_line(document, line_num));
					res = realloc(res, strlen(Document_get_line(document, line_num)) + strlen(tmpres) + 1);
					strcat(res, tmpres);
					Document_set_line(document, line_num++, res);
					free(tmpres);
				}
				else{
					Document_insert_line(document, line_num++, res);
				}
			}
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
	if((size_t)line_num > Document_size(document)){
		Document_insert_line(document, line_num++, res);
	}
	else if(count++ == 0){
		char *tmpres = strdup(res);
		res = realloc(res, strlen(Document_get_line(document, line_num)) + strlen(tmpres) + 1);
		strcpy(res, Document_get_line(document, line_num));
		res = strcat(res, tmpres);
		Document_set_line(document, line_num++, res);
		free(tmpres);
	}
	else{
		Document_insert_line(document, line_num++, res);
	}
	free(res);
	free(num);
	res = NULL;
	num = NULL;
	start = NULL;
	end = NULL;
}

void handle_delete_command(Document *document, const char *command) {
	char *start = strdup(command + 2);
	char *num = strsep(&start, " ");
	int line_num = atoi(num);
	if((size_t)line_num > Document_size(document)){
		 invalid_line();
		 return;
	 }
	else Document_delete_line(document, line_num);
	free(start);
	free(num);
}

void handle_search_command(Document *document, const char *command) {
	char *search = strdup(command + 1);
	char *loc;
	for(int i = 1; i < Document_size(document); i++){
		if((loc = strstr(Document_get_line(document, i), search)) != NULL){
			print_search_line(i, Document_get_line(document, i), loc, search);
		}
	}
}

void handle_save_command(Document *document, const char *filename) {
	Document_write_to_file(document, filename);
}