/**
* Machine Problem: Vector
* CS 241 - Fall 2016
*/

#include "document.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
// test your document here
int main() {
	Document *d = Document_create();
	Document_destroy(d);

	Document *d_file = Document_create_from_file("memes1.txt");
	printf("line 1: %s\n", Document_get_line(d_file, 1));
	// printf("line 2: %s\n", Document_get_line(d_file, 2));
	Document_destroy(d_file);

	Document *d_file10 = Document_create_from_file("memes_emptylines.txt");
	printf("line 1: %s\n", Document_get_line(d_file10, 1));
	printf("line 8: %s\n", Document_get_line(d_file10, 2));
	Document_insert_line(d_file10, 24, "line 24");
	Document_set_line(d_file10, 4, "ldhlkadhvlkadjhvkdjfvsdfjbvjdsfkjsdfkjdshfjdshkhjsfb");
	Document_set_line(d_file10, 11, "ldhlkadhvlkadjhvkdjfvsdfjbvjdsfkjsdf0890890890hfjdshkhjsfb");
	Document_set_line(d_file10, 13, "ldhlkadhvlkadjhvkdjfvsdfjbvjdsfkj8678678jdshfjdshkhjsfb");
	Document_set_line(d_file10, 17, "ldhlkadhvlkadjhvkdjfvsdfjbvjd34534534dfkjdshfjdshkhjsfb");
	Document_set_line(d_file10, 19, "ldhlkadhvlkadjhvkdjfvsdfj435345345fkjsdfkjdshfjdshkhjsfb");
	Document_set_line(d_file10, 23, "ldhlkadhvlkadjhvkdjfvsdfjb6345sfkjsdfkjdshfjdshkhjsfb");
	// Document_delete_line(d_file10, 7);
	Document_write_to_file(d_file10, "blah.txt");
	Document_destroy(d_file10);
	return 0;
}
