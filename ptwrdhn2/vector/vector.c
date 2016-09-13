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

	vec -> copy_constructor = copy_constructor;
	vec -> destructor = destructor;
	vec -> array = calloc(INITIAL_CAPACITY, sizeof(void *));
	vec -> size = 0;
	vec -> capacity = INITIAL_CAPACITY;

	return vec;
}

void Vector_destroy(Vector *vector) {
	assert(vector);
	// your code here
	for(int i = 0; i < (int)Vector_size(vector); i++){
		if(vector -> array[i]){
			vector -> destructor(vector -> array[i]);
		}
	}
	free(vector -> array);
	free(vector);
	vector = NULL;
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
	assert(new_size >= 0);
	size_t initSize = Vector_size(vector);
	size_t initCap = Vector_capacity(vector);
	// your code here
	if(new_size != initSize && new_size >= 10){
		if(new_size > initSize){
			if(new_size >= initCap){
				while(new_size >= Vector_capacity(vector)){
					vector -> capacity *= 2;
				}
				vector -> array = realloc(vector -> array, Vector_capacity(vector) * sizeof(void *));
			}
			memset(vector -> array + initSize, 0, (new_size - initSize) * sizeof(void *));
		}
		else if(new_size < initSize){
			for(size_t i = new_size; i <= initSize; i++){
				if(vector -> array[i] != NULL){
					vector -> destructor(vector -> array[i]);
				}
				vector -> array[i] = NULL;
			}
			if(4 * new_size <= Vector_capacity(vector)){
				while (Vector_capacity(vector) > 2 * new_size) {
					vector -> capacity /= 2;
				}
			}
			vector -> capacity = Vector_capacity(vector) < 10 ? 10 : Vector_capacity(vector);
		}
	}
	vector -> size = new_size;
}

void Vector_set(Vector *vector, size_t index, void *elem) {
	assert(vector);
	assert(index >= 0);
	assert(index < Vector_size(vector));
	assert(Vector_size(vector));
	// your code here
	if(vector -> array[index]) vector -> destructor(vector -> array[index]);
	vector -> array[index] = elem ? vector -> copy_constructor(elem) : NULL;
}

void *Vector_get(Vector *vector, size_t index) {
	assert(vector);
	assert(index < Vector_size(vector));
	assert(index >= 0);
	// your code here
	return vector -> array[index];
}

void Vector_insert(Vector *vector, size_t index, void *elem) {
	assert(vector);
	// your code here
	if(index > Vector_size(vector)){
		Vector_resize(vector, index + 1);
	}
	else Vector_resize(vector, Vector_size(vector) + 1);

	memmove(vector -> array[index + 1], vector -> array[index], (Vector_size(vector) - index - 1) * sizeof(void *));
	vector -> array[index] = elem ? vector ->copy_constructor(elem) : NULL;
}

void Vector_delete(Vector *vector, size_t index) {
	assert(vector);
	assert(index < Vector_size(vector));
	// your code here, what asserts might you want?

	if(vector -> array[index]) vector -> destructor(vector -> array[index]);

	// memmove(vector -> array[index], vector -> array[index + 1], (Vector_size(vector) - index - 1) * sizeof(void *));
	for(size_t i = index; i < (Vector_size(vector) - index - 1); i++){
		vector -> array[i] = vector -> array[i + 1];
	}
	Vector_resize(vector, Vector_size(vector) - 1);
}

void Vector_append(Vector *vector, void *elem) {
	assert(vector);
	// your code here
	Vector_resize(vector, Vector_size(vector) + 1);
	vector -> array[Vector_size(vector) - 1] = elem ? vector ->copy_constructor(elem) : NULL;
}
