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

Vector *Vector_create(copy_constructor_type copy_constructor,
                      destructor_type destructor) {
  // your code here
  return NULL;
}

void Vector_destroy(Vector *vector) {
  assert(vector);
  // your code here
}

size_t Vector_size(Vector *vector) {
  assert(vector);
  // your code here
  return 0;
}

size_t Vector_capacity(Vector *vector) {
  assert(vector);
  // your code here
  return 0;
}

void Vector_resize(Vector *vector, size_t new_size) {
  assert(vector);
  // your code here
}

void Vector_set(Vector *vector, size_t index, void *elem) {
  assert(vector);
  // your code here
}

void *Vector_get(Vector *vector, size_t index) {
  assert(vector);
  // your code here
  return NULL;
}

void Vector_insert(Vector *vector, size_t index, void *elem) {
  assert(vector);
  // your code here
}

void Vector_delete(Vector *vector, size_t index) {
  assert(vector);
  // your code here, what asserts might you want?
}

void Vector_append(Vector *vector, void *elem) {
  assert(vector);
  // your code here
}
