/**
* Mini Valgrind Lab
* CS 241 - Fall 2016
*/

#include "mini_valgrind.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
	// your tests here using malloc and free
	char * f = "test.c";
	void * p1 = mini_malloc(32, f, 12);
	printf("p1 => %p\n", p1);

	void * p2 = mini_malloc(64, f, 15);
	printf("p2 => %p\n", p2);

	mini_free(p1);

	mini_free(p2);

	mini_free(p1 - 50);

	// Do NOT modify this line
	atexit(print_report);
	return 0;
}
