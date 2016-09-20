/**
 * Machine Problem: Shell
 * CS 241 - Fall 2016
 */
#include "format.h"
#include "log.h"
#include "shell.h"

#include <signal.h>



void sigint_handler(int sig){
	printf("caught signal: %d\n", signal);
	signal(sig, sigint_handler);
}

int shell(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
  print_shell_owner("ptwrdhn2");
  signal(SIGINT, sigint_handler);

  return 0;
}