/**
* Machine Problem: Malloc
* CS 241 - Fall 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _meta_data {
	void *ptr;
	size_t size;
	int free;
	struct _meta_data *next;
	// struct _meta_data *prev;

} meta_data;
int is_free = 0;
meta_data *head = NULL;

void coalesce(void *same){
	meta_data *a = same + sizeof(meta_data) + ((meta_data *)same)->size;
	meta_data *co = (meta_data *)same;
	if(a && a->size) write(0, "exists\n", 8);
	if((a < head + sizeof(meta_data) + head -> size) && a -> free){
		co -> size += a -> size + sizeof(meta_data);
		if((meta_data *)(same + sizeof(meta_data) + co -> size)){
			((meta_data *)(same + sizeof(meta_data) + co -> size)) -> next = co;
		}
		// write(0, "coalescing\n", 12);
	}
	// if((a = (co -> prev)) && a -> free){
	// 	a -> size += co -> size + sizeof(meta_data);
	// }
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
	size_t space = num * size;
	void *ptr = malloc(space);
	memset(ptr, 0, space);
	return ptr;
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
	meta_data *p = head;
	meta_data *chosen = NULL;
	if (size <= 0) return NULL;
	if (is_free){
		while (p != NULL) { // block splitting needs to be done here
			if (p -> free && p -> size >= size) {
				chosen = p;
				break;
			}
			p = p -> next;
		}

		if (chosen) {
			chosen -> free = 0;
			return chosen->ptr;
		}
	}
	chosen = sbrk(0);
	sbrk(sizeof(meta_data));
	chosen -> ptr = sbrk(0);
	if (sbrk(size) == (void*)-1) return NULL;
	chosen -> size = size;
	chosen -> free = 0;
	chosen -> next = head;
	head = chosen;
	return chosen -> ptr;
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
	if (!ptr) return;
	meta_data *ptr2 = (meta_data*)ptr - 1;
	ptr2 -> free = 1;
	is_free = 1;
	coalesce(ptr2);
	return;
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
	if (!ptr) return malloc(size);
	if (size == 0) free(ptr);
	meta_data *p = (meta_data*) ptr - 1;
	if (p -> size >= size) {
		return ptr;
	}
	else{
		void* out = malloc(size);
		memcpy(out, ptr, p -> size);
		free(ptr);
		return out;
	}
}