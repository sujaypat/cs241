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
void my_destructor(void *elem) { free(elem); }

Document *Document_create() {
  Document *document = (Document *)malloc(sizeof(Document));

  // if size isn't zero, the interface to vector changed and we might have NULLs
  // in the vector
  // you code here!
  return NULL;
}

void Document_write_to_file(Document *document, const char *filename) {
  assert(document);
  assert(filename);
  // see the comment in the header file for a description of how to do this!
  // your code here!
}

Document *Document_create_from_file(const char *filename) {
  assert(filename);
  // this function will read a file which is created by Document_write_to_file
  // your code here!
  return NULL;
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

void Document_set_line(Document *document, size_t line_number,
                       const char *str) {
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

void Document_insert_line(Document *document, size_t line_number,
                          const char *str) {
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
