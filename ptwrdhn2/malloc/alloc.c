/**
* Machine Problem: Malloc
* CS 241 - Fall 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _meta_data {
	size_t is_free;
	size_t size;
	struct _meta_data *next;
	struct _meta_data *prev;
	struct _meta_data *free_next;
	struct _meta_data *free_prev;
} meta_data;

unsigned long sbrk_loc;
meta_data *head = NULL;
meta_data *first_free = NULL;

void coalesce(void *same){
	meta_data *co = (meta_data *)same;
	meta_data *a = NULL;
	if((a = (co + co -> size + sizeof(meta_data))) -> is_free){
		co -> size += a -> size + sizeof(meta_data);
		co -> next = a -> next;
	}
	if((a = (co -> prev)) -> is_free){
		a -> size += co -> size + sizeof(meta_data);
	}
}

void *first_fit(size_t size_needed){
	void *found = NULL;
	meta_data *curr = head;
	while(curr != NULL){
		write(0, &(curr -> size), sizeof(size_t));
		write(0, " = size\n", strlen(" = size\n") + 1);
		if(curr -> size >= size_needed && curr -> is_free){
			found = curr;
			break;
		}
		curr = curr + sizeof(meta_data) + curr -> size;
	}
	return found;
}

void insert_meta_data(meta_data *this, size_t is_free, size_t size, meta_data *next, meta_data *prev, meta_data *free_next, meta_data *free_prev){
	this -> is_free = is_free;
	this -> size = size;
	this -> next = next;
	this -> prev = prev;
	this -> free_next = free_next;
	this -> free_prev = free_prev;
	if(head) head -> prev = this;
	head = this;
}

/**
* Allocate space for array in memory
*
* Allocates a block of memory for an array of num elements, each of them size
* bytes long, and initializes all its bits to zero. The effective result is
* the allocation of an zero-initialized memory block of (num * size) bytes.
*
* @param num
*    Number of elements to be allocated.
* @param size
*    Size of elements.
*
* @return
*    A pointer to the memory block allocated by the function.
*
*    The type of this pointer is always void*, which can be cast to the
*    desired type of data pointer in order to be dereferenceable.
*
*    If the function failed to allocate the requested block of memory, a
*    NULL pointer is returned.
*
* @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
*/
void *calloc(size_t num, size_t size) {
	void *mem = malloc(num * size);
	if(mem) memset(mem, 0, num * size);
	return mem;
}

/**
* Allocate memory block
*
* Allocates a block of size bytes of memory, returning a pointer to the
* beginning of the block.  The content of the newly allocated block of
* memory is not initialized, remaining with indeterminate values.
*
* @param size
*    Size of the memory block, in bytes.
*
* @return
*    On success, a pointer to the memory block allocated by the function.
*
*    The type of this pointer is always void*, which can be cast to the
*    desired type of data pointer in order to be dereferenceable.
*
*    If the function failed to allocate the requested block of memory,
*    a null pointer is returned.
*
* @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
*/
void *malloc(size_t size) {
	write(0, "malloc called\n", strlen("malloc called\n") + 1);
	if(size == 0) return NULL;
	meta_data *newmem = NULL;
	meta_data *temp = first_free;
	if((newmem = first_fit(size)) == NULL){
		newmem = (meta_data *)sbrk(size + sizeof(meta_data));
		if(newmem == (void *)(-1)) return NULL;
		// newmem -> next = NULL;

		insert_meta_data(newmem, 0, size, head, NULL, temp, NULL);
		write(0, "sbrk size: ", strlen("sbrk size: ") + 1);
		write(0, &(newmem), sizeof(meta_data));
		write(0, "\n", strlen("\n") + 1);
		return (void *)(newmem + sizeof(meta_data));
	}
	temp = newmem -> next;
	while(temp){
		if(temp -> is_free){
			break;
		}
		temp = temp -> next;
	}
	insert_meta_data(newmem, 0, size, head, NULL, temp, NULL);
	write(0, "add size: ", strlen("add size: ") + 1);
	write(0, &(newmem), sizeof(meta_data));
	write(0, "\n", strlen("\n") + 1);
	return (void *)(newmem) + sizeof(meta_data);
}

/**
* Deallocate space in memory
*
* A block of memory previously allocated using a call to malloc(),
* calloc() or realloc() is deallocated, making it available again for
* further allocations.
*
* Notice that this function leaves the value of ptr unchanged, hence
* it still points to the same (now invalid) location, and not to the
* null pointer.
*
* @param ptr
*    Pointer to a memory block previously allocated with malloc(),
*    calloc() or realloc() to be deallocated.  If a null pointer is
*    passed as argument, no action occurs.
*/
void free(void *ptr) {
	write(0, "free called\n", strlen("free called\n") + 1);
	meta_data *to_free = (meta_data *)(ptr - sizeof(meta_data));
	to_free -> is_free = 1;
	if((to_free -> next && to_free -> next -> is_free) || (to_free -> prev && to_free -> prev -> is_free)){
		coalesce(to_free);
	}
}

/**
* Reallocate memory block
*
* The size of the memory block pointed to by the ptr parameter is changed
* to the size bytes, expanding or reducing the amount of memory available
* in the block.
*
* The function may move the memory block to a new location, in which case
* the new location is returned. The content of the memory block is preserved
* up to the lesser of the new and old sizes, even if the block is moved. If
* the new size is larger, the value of the newly allocated portion is
* indeterminate.
*
* In case that ptr is NULL, the function behaves exactly as malloc, assigning
* a new block of size bytes and returning a pointer to the beginning of it.
*
* In case that the size is 0, the memory previously allocated in ptr is
* deallocated as if a call to free was made, and a NULL pointer is returned.
*
* @param ptr
*    Pointer to a memory block previously allocated with malloc(), calloc()
*    or realloc() to be reallocated.
*
*    If this is NULL, a new block is allocated and a pointer to it is
*    returned by the function.
*
* @param size
*    New size for the memory block, in bytes.
*
*    If it is 0 and ptr points to an existing block of memory, the memory
*    block pointed by ptr is deallocated and a NULL pointer is returned.
*
* @return
*    A pointer to the reallocated memory block, which may be either the
*    same as the ptr argument or a new location.
*
*    The type of this pointer is void*, which can be cast to the desired
*    type of data pointer in order to be dereferenceable.
*
*    If the function failed to allocate the requested block of memory,
*    a NULL pointer is returned, and the memory block pointed to by
*    argument ptr is left unchanged.
*
* @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
*/
void *realloc(void *ptr, size_t size) {
	meta_data *curr = (meta_data *)(ptr - sizeof(meta_data));
	if(size == curr -> size) return ptr;
	if(size > curr -> size){
		if(curr -> next -> is_free && size < ((curr -> size) + sizeof(meta_data) + (curr -> next -> size))){
			curr -> size = size;
		}
		else{

		}
	}
	else if(size < curr -> size){

	}
	return NULL;
}
