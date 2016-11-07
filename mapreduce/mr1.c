/**
 * MapReduce
 * CS 241 - Fall 2016
 */

#include "common.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void usage() { print_mr1_usage(); }

int main(int argc, char **argv) {
  // Create an input pipe for each mapper.

  // Create one input pipe for the reducer.

  // Open the output file.

  // Start a splitter process for each mapper.

  // Start all the mapper processes.

  // Start the reducer process.

  // Wait for the reducer to finish.

  // Print nonzero subprocess exit codes.

  // Count the number of lines in the output file.

  return 0;
}
