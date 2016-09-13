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
void dumplmaos(Vector * vector){
	size_t i = 0;
	printf("ayyy//");
	for(i = 0; i < Vector_size(vector); i++){
		printf("%s | ", Vector_get(vector, i) );
	}
	printf("//lmao\n" );
}
// Test your vector here
void dicks(Vector * vector){
	printf("%zu/%zu dicks\n",Vector_size(vector), Vector_capacity(vector) );
}

int main() {
	char *val = "same";
	Vector *test = Vector_create(my_copy_ctor, my_destructor);
	if(!test) printf("vector is null!");
	// Vector_resize(test, 100);

	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	printf("%s\n", Vector_get(test, 2));
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	dicks(test);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	printf("%s\n", Vector_get(test, 2));
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	dicks(test);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	printf("%s\n", Vector_get(test, 2));
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_delete(test, 2);
	Vector_append(test, val);
	dicks(test);
	Vector_append(test, val);
	Vector_append(test, val);
	dicks(test);
	dicks(test);
	dumplmaos(test);
	// Vector_set(test, 20, val);
	// Vector_delete(test, Vector_size(test) - 1);
	Vector_destroy(test);
	return 0;
}
