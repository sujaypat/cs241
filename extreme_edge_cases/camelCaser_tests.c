/**
 * Chatroom Lab
 * CS 241 - Fall 2016
 */
#include "camelCaser_tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"

/*
 * Testing function for various implementations of camelCaser.
 *
 * @param  camelCaser   A pointer to the target camelCaser function.
 * @return              Correctness of the program (0 for wrong, 1 for correct).
 */
int test_camelCaser(char **(*camelCaser)(const char *)) {
  // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
  char *input = "ayy lmao";
  char *correct = "ayyLmao";
  int result = 0;
  char **res = (*camelCaser)(input);
  printf("%s\n", res[0]);
  result = !strcmp(correct, res[0]);
  return result;
  // use assert
}
