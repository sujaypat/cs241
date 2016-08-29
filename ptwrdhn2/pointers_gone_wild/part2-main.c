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

  int* second = 132;
  second_step(*second);

  int **double[] = new double[1];
  double[0] = 8942;
  double_step(double);

  strange_step();
  return 0;
}
