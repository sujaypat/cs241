/**
* Machine Problem: Text Editor
* CS 241 - Fall 2016
*/

#include "document.h"
#include "editor.h"
#include "format.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* You can programatically test your text editor.
*/
int main() {
	// Setting up a docment based on the file named 'filename'.
	char *filename = "test-ref.txt";
	Document *document = Document_create_from_file(filename);

	handle_write_command(document, "w 1 hello");
	handle_write_command(document, "w 69 a;dslkjf;asdljf;asdlk$adsfas$asdfasf4$AF$sfA$$AF$G$GSAGFJBSKGNNFEF$BJSDGIRT*^SFJE $J KG  $   $  $   $   SAJF KJG%% W($U#$#JJEGA)");
	handle_display_command(document, "p");
	Document_destroy(document);
}
