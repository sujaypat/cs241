/**
 * Machine Problem: Malloc
 * CS 241 - Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  /*
   * Check to ensure that the program is launched with at least one command
   * line option.  Display helpful text if no options are present.
   */
  if (argc == 1) {
    printf("You must supply a program to be invoked to use your replacement "
           "malloc() script.\n");
    printf("...you may use any program, even system programs, such as `ls`.\n");
    printf("\n");
    printf("Example: %s /bin/ls\n", argv[0]);
    return 1;
  }

  /*
   * Set up the environment to pre-load our 'malloc.so' shared library, which
   * will replace the malloc(), calloc(), realloc(), and free() that is defined
   * by standard libc.
   */
  char **env = malloc(2 * sizeof(char *));
  env[0] = malloc(100 * sizeof(char));
  sprintf(env[0], "LD_PRELOAD=./alloc.so");

  env[1] = NULL;

  /*
   * Replace the current running process with the process specified by the
   * command
   * line options.  If exec() fails, we won't even try and recover as there's
   * likely
   * nothing we could really do; however, we do our best to provide useful
   * output
   * with a call to perror().
   */
  execve(argv[1], argv + 1,
         env); /* Note that exec() will not return on success. */
  perror("exec() failed");

  free(env[0]);
  free(env);

  return 2;
}
