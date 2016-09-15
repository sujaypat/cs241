/**
 * Mini Valgrind Lab
 * CS 241 - Fall 2016
 */

#include "mini_valgrind.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
  // your tests here using malloc and free

  // Do NOT modify this line
  atexit(print_report);
  return 0;
}
