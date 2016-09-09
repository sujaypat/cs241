/**
* Machine Problem: Vector
* CS 241 - Fall 2016
*/

/* An automatically-expanding array of strings. */
#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_CAPACITY 10

/*
* Vector structure
* Do not modify the structure
* array: Void pointer to the beginning of an array of void pointers to
* arbitrary data.
* size: The number of elements in the vector. This is the number of actual
* objects held in the vector, which is not necessarily equal to its capacity.
* capacity: The size of the storage space currently allocated for the vector,
* expressed in terms of elements.
* copyt_constructor: the function callback for you to define the way you want
* to copy elements
* destructor:  the function callback for you to define the way you want to
* destroy elements
*/
struct Vector {
	copy_constructor_type copy_constructor;
	destructor_type destructor;

	void **array;
	size_t size;
	size_t capacity;
};

Vector *Vector_create(copy_constructor_type copy_constructor, destructor_type destructor) {
	// your code here
	Vector *vec = malloc(sizeof(Vector));
	//malloc(sizeof(copy_constructor_type) + sizeof(destructor_type) + 10*sizeof(void *) + 2*sizeof(size_t));
	vec -> copy_constructor = copy_constructor;
	vec -> destructor = destructor;
	vec -> array = malloc(INITIAL_CAPACITY * sizeof(void *));
	vec -> size = 0;
	vec -> capacity = INITIAL_CAPACITY;
	return NULL;
}

void Vector_destroy(Vector *vector) {
	assert(vector);
	// your code here
	for(int i = 0; i < (int)vector -> size; i++){
		if(vector -> array[i]){
			free(vector -> array[i]);
		}
	}
	free(vector -> copy_constructor);
	free(vector -> destructor);
	free(vector -> array);
	free(vector);
}

size_t Vector_size(Vector *vector) {
	assert(vector);
	// your code here
	return vector -> size;
}

size_t Vector_capacity(Vector *vector) {
	assert(vector);
	// your code here
	return vector -> capacity;
}

void Vector_resize(Vector *vector, size_t new_size) {
	assert(vector);
	// your code here
	while(new_size > vector -> size){
		vector -> capacity *= 2;
	}
	while (Vector_size(vector) < 10 && Vector_size(vector) < 2 * new_size) {
		vector -> capacity /= 2;
	}
}

void Vector_set(Vector *vector, size_t index, void *elem) {
	assert(vector);
	// your code here
	if(index < Vector_size(vector)){
		vector -> array[index] = elem;
	}
}

void *Vector_get(Vector *vector, size_t index) {
	assert(vector);
	// your code here
	if(index < Vector_size(vector) && vector -> array[index]){
		return vector -> array[index];
	}
	return NULL;
}

void Vector_insert(Vector *vector, size_t index, void *elem) {
	assert(vector);
	// your code here
	if(index > Vector_size(vector)){
		Vector_resize(vector, index);
	}
	vector -> array[index] = elem;
}

void Vector_delete(Vector *vector, size_t index) {
	assert(vector);
	// your code here, what asserts might you want?
	if(Vector_size(vector) < index){

	}
	vector -> array[index] = NULL;
}

void Vector_append(Vector *vector, void *elem) {
	assert(vector);
	// your code here
	if(Vector_size(vector) >= Vector_capacity(vector) - 1){
		Vector_resize(vector, Vector_size(vector) + 1);
	}
	vector -> array[Vector_size(vector) - 1] = elem;
}
