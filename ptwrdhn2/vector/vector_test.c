/**
* Machine Problem: Vector
* CS 241 - Fall 2016
*/

#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
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

// Test your vector here
int main() {
	char *val = "same";
	Vector *test = Vector_create(my_copy_ctor, my_destructor);
	if(!test) printf("fuck you");
	Vector_resize(test, 100);
	Vector_set(test, 20, val);
	Vector_destroy(test);

	return 0;
}
