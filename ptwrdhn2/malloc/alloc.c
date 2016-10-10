/**
* Machine Problem: Malloc
* CS 241 - Fall 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct _meta_data {
	size_t size;
	struct _meta_data* next;
} meta_data;

meta_data *head = NULL;
meta_data *tail = NULL;



void coalesce(void *same){
	meta_data *a = same + sizeof(meta_data) + ((meta_data *)same) -> size;
	meta_data *co = (meta_data *)same;
	// if(a && a->size) write(0, "exists\n", 8);
	if(((void *)a < ((void *)head + sizeof(meta_data) + head -> size)) && a -> free){
		co -> size += a -> size + sizeof(meta_data);
		if(a == head){
			head = co;
		}
		if(((meta_data *)(same + sizeof(meta_data) + co -> size)) -> size){
			((meta_data *)(same + sizeof(meta_data) + co -> size)) -> next = co;
		}
	}
	// if(co -> next && co -> next -> free){
	// 	a = co -> next;
	// 	a -> size += sizeof(meta_data) + co -> size;
	// 	if((co + sizeof(meta_data) + co -> size) < (head + sizeof(meta_data) + head -> size)){
	// 		((meta_data *)(co + sizeof(meta_data) + co -> size)) -> next = a;
	// 	}
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

	if(size == 0) return NULL;
	meta_data *temp = head;

	if(head != NULL){
		if(head->size >= size){
			head = head->next;
			return (void*)temp+sizeof(meta_data);
		}
		else{
			while(temp->next != NULL){
				if(temp->next->size >= size){
					meta_data *touse = temp->next;
					temp->next = touse->next;

					return (void*)touse+sizeof(meta_data);
				}
				temp = temp->next;
			}
		}
	}


	void *p = sbrk(0);
	p = sbrk(size+sizeof(meta_data));

	if(p == (void*)-1) return NULL;

	*tail = p;
	meta_data *cell = p;

	cell->size = size;
	cell->next = NULL;

	return (void*)cell+sizeof(meta_data);
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
	/*
	if (!ptr) return;
	meta_data *ptr2 = (meta_data*)ptr - 1;
	ptr2 -> free = 1;
	is_free = 1;
	coalesce(ptr2);
	return;
	*/
	if (!ptr) return;

	meta_data *tofree = ptr-sizeof(meta_data);
	tofree->next = head;
	head = tofree;
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
	if (!size){
		free(ptr);
		return NULL;
	}

	meta_data *p = ptr-sizeof(meta_data);

	if(p->size >= size) return ptr;

	if(p->size < size) {
		free(ptr);

		void* newp = malloc(size);
		memcpy(newp, ptr, p->size);

		return newp;
	}
	return ptr;

	/*
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
*/
}