/**
 * Machine Problem: Malloc
 * CS 241 - Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M (1024L * 1024L)
#define SIZE (128 * M)
#define ITERS 100000

int main() {
  // not sure why we need to do this, but all the other testers perform this
  // initial malloc
  malloc(1);

  int i;
  for (i = 0; i < ITERS; i++) {
    char *a = malloc(SIZE + i);
    if (!a)
      return 1;

    *a = 'c';
    if (*a != 'c')
      return 1;

    int *b = malloc(SIZE + i);
    if (!b)
      return 1;

    *b = 'c';
    if (*b != 'c')
      return 1;

    free(a);
    free(b);

    a = malloc(2 * (SIZE + i));
    if (!a)
      return 1;

    *a = 'c';
    if (*a != 'c')
      return 1;

    free(a);
  }

  printf("Memory was allocated, used, and freed!\n");
  return 0;
}
