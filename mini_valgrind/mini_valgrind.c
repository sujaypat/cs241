/**
* Mini Valgrind Lab
* CS 241 - Fall 2016
*/

#include "mini_valgrind.h"
#include "print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef malloc
#undef realloc
#undef free



/*
* Replace normal malloc, this malloc will also create meta data info
* and insert it to the head of the list.
* You have to malloc enough size to hold both the size of your allocated
* and the meta_data structure.
* In this function, you should only call malloc only once.
*
* @param size
*	Size of the memory block, in bytes.
* @param file
*	Name of the file that uses mini_valgrind to detect memory leak.
* @param line
*	Line in the file that causes memory leak
*
* @return
*	On success, return a pointer to the memory block allocated by
*	the function. Note that this pointer should return the actual
*	address the pointer starts and not the meta_data.
*
*	If the function fails to allocate the requested block of memory,
*	return a null pointer.
*/
void *mini_malloc(size_t size, const char *file, size_t line) {
	// your code here
	if(size == 0) return NULL;
	meta_data *newmem = calloc(1, sizeof(meta_data) + size);
	if(!newmem) return NULL;

	newmem -> size = size;
	newmem -> line_num = line;
	newmem -> file_name = calloc(1, MAX_FILENAME_LENGTH);

	//memset(newmem -> file_name, 0, MAX_FILENAME_LENGTH);
	strcpy(newmem -> file_name, file);
	newmem -> next = NULL;
	insert_meta_data((meta_data *)newmem, size, file, line);
	return (void *)(newmem) + sizeof(meta_data);
}

/*
* Replace normal realloc, this realloc will also first check whether the
* pointer that passed in has memory. If it has memory then resize the memory
* to it. Or if the pointer doesn't have any memory, then call malloc to
* provide memory.
* For total usage calculation, if the new size is larger than the old size,
* the total usage should increase the difference between the old size and the
* new size. If the new size is smeller or equal to the old size, the total
* usage should remain the same.
* You have to realloc enough size to hold both the size of your allocated
* and the meta_data structure.
* In this function, you should only call malloc only once.
*
* @param ptr
*      The pointer require realloc
* @param size
*	Size of the memory block, in bytes.
* @param file
*	Name of the file that uses mini_valgrind to detect memory leak.
* @param line
*	Line in the file that causes memory leak
*
* @return
*	On success, return a pointer to the memory block allocated by
*	the function. Note that this pointer should return the actual
*	address the pointer starts and not the meta_data.
*
*	If the function fails to allocate the requested block of memory,
*	return a null pointer.
*/
void *mini_realloc(void *ptr, size_t size, const char *file, size_t line) {
	// your code here
	return NULL;
}

/*
* Replace normal free, this free will also delete the node in the list.
*
* @param ptr
*	Pointer to a memory block previously allocated. If a null pointer is
*	passed, no action occurs.
*/
void mini_free(void *ptr) {
	if(ptr){
		remove_meta_data(ptr - sizeof(meta_data));
		// free(ptr - sizeof(meta_data));
	}
	// ptr = NULL;
}

/*
* Helper function to insert the malloc ptr node to the list.
* Accumulate total_usage here.
*
* @param md
* 	Pointer to the meta_data
* @param size
*	Size of the memory block, in bytes.
* @param file
*	Name of the file that uses mini_valgrind to detect memory leak.
* @param line
*	Line in the file that causes memory leak
*/
void insert_meta_data(meta_data *md, size_t size, const char *file, size_t line) {
	/* set value for malloc_info*/
	if(!md) return;
	md -> size = size;
	memset(md -> file_name, 0, MAX_FILENAME_LENGTH);
	strcpy(md -> file_name, file);
	md -> line_num = line;

	md -> next = head;
	head = md;
	total_usage += size;
}

/*
* Helper function to remove the free ptr node from the list.
* Add to total_free here.
*
* @param ptr
*	Pointer to a memory block previously allocated.
*/
void remove_meta_data(void *ptr) {

	if(ptr == NULL || head == NULL){
		bad_frees++;
		return;
	}
	meta_data *del = (meta_data *)ptr;
	meta_data *curr = head;
	if(del == head){
		total_free += del -> size;
		free(del);
		return;
	}
	while(curr -> next != NULL){
		if(curr -> next == del){
			curr -> next = del -> next;
			total_free += del -> size;
			free(del);
			return;
		}
		curr = curr -> next;
	}
	/* check if ptr is in the list and delete it from list */
}

/*
* Deletes all nodes from the list. Called when the program exits so make sure
* to not count these blocks as freed.
*/
void destroy() {

	meta_data *p1 = head;
	meta_data *p2 = NULL;
	while(p1 != NULL){
		p2 = p1 -> next;
		free(p1);
		p1 = p2;
	}
}

/*
* Print mini_valgrind leak report.
*/
void print_report() {
	print_leak_info(head, total_usage, total_free, bad_frees);
}
