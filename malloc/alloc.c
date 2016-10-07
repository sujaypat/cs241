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
	struct _meta_data *next_free;
} meta_data;

static meta_data *last_free = NULL;
static meta_data *first_free = NULL;

// void coalesce(void *same){
// 	meta_data *co = (meta_data *)same;
// 	meta_data *a = NULL;
// 	if((a = (co -> next)) && a -> is_free){
// 		co -> size += a -> size + sizeof(meta_data);
// 		co -> next = a -> next;
// 	}
// 	if((a = (co -> prev)) && a -> is_free){
// 		a -> size += co -> size + sizeof(meta_data);
// 	}
// }

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

/*
void insert_meta_data(meta_data *this, size_t size, meta_data *prev_free, meta_data *next_free){
this -> size = size;
this -> prev_free = prev_free;
this -> next_free = next_free;
}

void *first_fit(size_t size_needed){
meta_data *found = NULL;
meta_data *curr = first_free;
while(curr != NULL){
if(curr -> size >= size_needed){
if(curr -> size <= (size_needed + sizeof(meta_data))){
if(curr -> prev_free){
curr -> prev_free -> next_free = curr -> next_free;
}
if(curr -> next_free){
curr -> next_free -> prev_free = curr -> prev_free;
}
found = curr;
break;
}
else{
size_t original = curr -> size;
found = (meta_data *)(curr + sizeof(meta_data) + size_needed);
insert_meta_data(found, (original - size_needed - sizeof(meta_data)), curr -> prev_free, curr -> next_free);
curr -> next_free = found;
curr -> size = size_needed;
return curr;
}
}
curr = curr -> next_free;
}
return found;
}
*/

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
void *malloc(size_t size){
	if(size == 0) return NULL;
	meta_data *temp = first_free;

	if(first_free != NULL) {
		if(first_free -> size >= size) {
			first_free = first_free -> next_free;


			return (void*)temp+sizeof(meta_data);
		}
		else
		{
			while(temp -> next_free != NULL)
			{
				if(temp -> next_free -> size >= size) {
					meta_data *touse = temp -> next_free;
					temp -> next_free = touse -> next_free;

					return (void*)touse+sizeof(meta_data);
				}
				temp = temp -> next_free;
			}
		}
	}


	void *p = sbrk(0);
	p = sbrk(size+sizeof(meta_data));

	if(p == (void*)-1) return NULL;

	meta_data *last_free = p;
	meta_data *newmem = p;

	newmem -> size = size;
	newmem -> next_free = NULL;

	return (void*)newmem+sizeof(meta_data);

}



void free(void *ptr) {
	if (!ptr) return;

	meta_data *tofree = ptr-sizeof(meta_data);

	tofree -> next_free = first_free;
	first_free = tofree;

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
	if(p -> size >= size) return ptr;

	if(p -> size < size){
		free(ptr);

		void* newptr = malloc(size);
		memcpy(newptr, ptr, p -> size);

		return newptr;
	}
	return ptr;
}
