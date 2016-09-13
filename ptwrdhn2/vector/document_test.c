/**
 * Machine Problem: Vector
 * CS 241 - Fall 2016
 */

#include "document.h"
// test your document here
int main() {
	Document *d = Document_create();
	Document_destroy(d);

	Document *d_file = Document_create_from_file("memes1.txt");
	printf("line 1: %s\n", Document_get_line(d_file, 1));
	printf("line 2: %s\n", Document_get_line(d_file, 2));
	Document_destroy(d_file);

	Document *d_file = Document_create_from_file("memes10.txt");
	printf("line 1: %s\n", Document_get_line(d_file, 1));
	printf("line 8: %s\n", Document_get_line(d_file, 8));
	Document_destroy(d_file);
	return 0;
}
