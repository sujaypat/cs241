/**
* Machine Problem: Vector
* CS 241 - Fall 2016
*/

#include "document.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

struct _document {
	Vector *vector;
};

// This is the constructor function for string element.
// Use this as copy_constructor callback in vector.
void *my_copy_ctor(void *elem) {
	char *str = elem;
	assert(str);
	return strdup(str);
}

// This is the destructor function for string element.
// Use this as destructor callback in vector.
void my_destructor(void *elem) {
	free(elem);
}

Document *Document_create() {
	Document *document = (Document *)malloc(sizeof(Document));

	// if size isn't zero, the interface to vector changed and we might have NULLs
	// in the vector
	// your code here!
	document -> vector = Vector_create(my_copy_ctor, my_destructor);
	return document;
}

void Document_write_to_file(Document *document, const char *filename) {
	assert(document);
	assert(filename);
	// see the comment in the header file for a description of how to do this!
	// your code here!
	size_t docSize = Document_size(document);
	size_t currLine = 0;
	FILE *f = fopen(filename, "w");
	while(currLine < docSize){
		fprintf(f, "%s\n", Document_get_line(document, currLine));
		currLine++;
	}
}

Document *Document_create_from_file(const char *filename) {
	assert(filename);
	// this function will read a file which is created by Document_write_to_file
	// your code here!
	Document *d = Document_create();
	size_t lineNum = 0;
	FILE *fp = fopen(filename, "r");
	if(!fp) return d;

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	if (fp == NULL) exit(EXIT_FAILURE);
	while ((read = getline(&line, &len, fp)) != -1) {
		Document_set_line(d, lineNum, line);
	}
	return d;

}

void Document_destroy(Document *document) {
	assert(document);
	Vector_destroy(document->vector);
	free(document);
}

size_t Document_size(Document *document) {
	assert(document);
	return Vector_size(document->vector);
}

void Document_set_line(Document *document, size_t line_number, const char *str) {
	assert(document);
	assert(str);
	assert(line_number > 0);
	size_t index = line_number - 1;
	Vector_set(document->vector, index, (void *)str);
}

const char *Document_get_line(Document *document, size_t line_number) {
	assert(document);
	assert(line_number > 0);
	size_t index = line_number - 1;
	return (const char *)Vector_get(document->vector, index);
}

void Document_insert_line(Document *document, size_t line_number, const char *str) {
	assert(document);
	assert(str);
	assert(line_number > 0);
	size_t index = line_number - 1;
	Vector_insert(document->vector, index, (void *)str);
}

void Document_delete_line(Document *document, size_t line_number) {
	assert(document);
	assert(line_number > 0);
	size_t index = line_number - 1;
	Vector_delete(document->vector, index);
}
