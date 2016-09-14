/**
 * Machine Problem: Text Editor
 * CS 241 - Fall 2016
 */

#include "document.h"
#include "editor.h"
#include "format.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_filename(int argc, char *argv[]) {
  // TODO implement get_filename
  return argv[1];
  // take a look at editor_main.c to see what this is used for
  return NULL;
}

void handle_display_command(Document *document, const char *command) {
  // TODO implement handle_display_command
}

void handle_write_command(Document *document, const char *command) {
  // TODO implement handle_write_command
}

void handle_append_command(Document *document, const char *command) {
  // TODO implement handle_append_command
}

void handle_delete_command(Document *document, const char *command) {
  // TODO implement handle_delete_command
}

void handle_search_command(Document *document, const char *command) {
  // TODO implement handle_search_command
}

void handle_save_command(Document *document, const char *filename) {
  // TODO implement handle_save_command
}
