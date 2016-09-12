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
	assert(new_size >= 0);
	size_t initSize = Vector_size(vector);
	size_t initCap = Vector_capacity(vector);
	// your code here
	if(new_size > initCap && new_size > 10){
		while(new_size > Vector_capacity(vector)){
			vector -> capacity *= 2;
		}
		vector -> array = realloc(vector -> array, new_size * sizeof(void *));
		memset(vector -> array + initSize, 0, (initSize - new_size) * sizeof(void *));
	}
	else if(4 * new_size <= Vector_capacity(vector) || new_size <= 10){
		if(new_size > 10){
			while (Vector_capacity(vector) > 2 * new_size) {
				vector -> capacity /= 2;
			}
		}
		else vector -> capacity = 10;
		for(size_t i = new_size; i <= initSize; i++){
			if(vector -> array[i]){
				vector -> destructor(vector -> array[i]);
			}
		}
		vector -> array = realloc(vector -> array, new_size * sizeof(void *));
	}
	if(Vector_size(vector) > Vector_capacity(vector)) vector -> size = Vector_capacity(vector);
}

void Vector_set(Vector *vector, size_t index, void *elem) {
	assert(vector);
	assert(index >= 0);
	assert(index < Vector_size(vector));
	assert(Vector_size(vector));
	// your code here
	if(index < Vector_capacity(vector)){
		if(vector -> array[index]) vector -> destructor(vector -> array[index]);
		if(elem) vector -> array[index] = vector -> copy_constructor(elem);
		else vector -> array[index] = NULL;
	}
	else{
		Vector_resize(vector, index + 1);
		if(elem) vector -> array[index] = vector -> copy_constructor(elem);
		else vector -> array[index] = NULL;
	}
}

void *Vector_get(Vector *vector, size_t index) {
	assert(vector);
	// your code here
	if(index < Vector_size(vector)){
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
	memcpy(vector -> array[index + 1], vector -> array[index], (Vector_size(vector) - index) * sizeof(void *));
	vector -> array[index] = elem;
}

void Vector_delete(Vector *vector, size_t index) {
	assert(vector);
	// your code here, what asserts might you want?
	if(Vector_size(vector) < index){

	}
	vector -> array[index] = NULL;
	for(int i = 0; i < (int)Vector_size(vector) - 1; i++){
		if(vector -> array[i] == NULL){

		}
	}
}

void Vector_append(Vector *vector, void *elem) {
	assert(vector);
	// your code here
	if(Vector_size(vector) >= Vector_capacity(vector) - 1){
		Vector_resize(vector, Vector_size(vector) + 1);
	}
	vector -> array[Vector_size(vector) - 1] = vector -> copy_constructor(elem);
}
