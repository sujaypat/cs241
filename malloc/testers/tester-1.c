/**
 * Machine Problem: Malloc
 * CS 241 - Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>

#define NUM_CYCLES 10000000

int main() {
  int i;
  for (i = 0; i < NUM_CYCLES; i++) {
    int *ptr = malloc(sizeof(int));

    if (ptr == NULL) {
      printf("Memory failed to allocate!\n");
      return 1;
    }

    *ptr = 4;
	printf("iteration %d ptr: %p\n", i, ptr);
	printf("metadata: %d\n", sizeof(meta_data));
    free(ptr);
  }

  printf("Memory was allocated, used, and freed!\n");
  return 0;
}
