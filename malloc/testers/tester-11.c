/**
 * Machine Problem: Malloc
 * CS 241 - Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>

#define M (1024L * 1024L)
#define SIZE (2L * 1024L * M)
#define ITERS 10000

int main() {
  // not sure why we need to do this, but all the other testers perform this
  // initial malloc
  malloc(1);
  int i;
  for (i = 0; i < ITERS; i++) {
    char *a = malloc(SIZE);
    if (!a)
      return 1;

    *a = 'c';
    if (*a != 'c')
      return 1;

    free(a);
    a = malloc(SIZE / 2);

    char *b = malloc(SIZE / 4);
    if (!a || !b)
      return 1;

    *a = 'c';
    if (*a != 'c')
      return 1;

    *b = 'c';
    if (*b != 'c')
      return 1;

    free(a);
    free(b);
  }

  printf("Memory was allocated, used, and freed!\n");
  return 0;
}
