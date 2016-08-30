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


  int second = 132;
  second_step(&second);


  int **arr = malloc(sizeof(int*));
  arr[0] = malloc(sizeof(int));
  *arr[0] = 8942;
  double_step(arr);
  free(arr[0]);
  free(arr);


  char *strange = malloc(16);
  for(int i = 0; i < 16; i++){
	  *(strange + i) = '\0';
  }
  *(strange + 5) = (char)15;
  strange_step(strange);
  free(strange);


  char empty[4] = {0,0,0,0};
  empty_step((void*) empty);


  empty[3] = 'u';
  two_step(empty, empty);


  three_step((char*)420, (char*)422, (char*)424);


  empty[1] = 57;
  empty[2] = 65;
  empty[3] = 73;
  step_step_step(empty, empty, empty);


  char odd = 1;
  it_may_be_odd(&odd, (int)odd);


  char* tok = "CS241";
  tok_step(tok);


  empty[0] = 1;
  empty[1] = 2;
  the_end(empty, empty);
  return 0;
}
