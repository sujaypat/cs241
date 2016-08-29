/**
 * Machine Problem 0
 * CS 241 - Spring 2016
 */

#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
  // your code here
  first_step(81);


  int* second = malloc(sizeof(int));
  *second = 132;
  second_step(second);
  free(second);


  int **arr = malloc(sizeof(int*));
  arr[0] = malloc(sizeof(int));
  *arr[0] = 8942;
  double_step(arr);
  free(arr[0]);
  free(arr);


  char *strange = malloc(16);
  *(strange + 5) = (char)15;
  // printf("%d", *(int *)(strange + 5));
  strange_step(strange);
  free(strange);

  return 0;
}
