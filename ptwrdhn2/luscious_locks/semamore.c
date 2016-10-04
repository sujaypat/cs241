/**
* Luscious Locks Lab
* CS 241 - Fall 2016
*/

#include "semamore.h"
#include <stdlib.h>

/**
* Initializes the Semamore. Important: the struct is assumed to have been
* allocated by the user.
* Example:
* 	Semamore *s = malloc(sizeof(Semamore));
* 	semm_init(s, 5, 10);
*
*/
void semm_init(Semamore *s, int value, int max_val) {
	s -> value = value;
	s -> max_val = max_val;
}

/**
*  Should block when the value in the Semamore struct (See semamore.h) is at 0.
*  Otherwise, should decrement the value.
*/
void semm_wait(Semamore *s) {
	while(s -> value == 0){

	}

}

/**
*  Should block when the value in the Semamore struct (See semamore.h) is at
* max_value.
*  Otherwise, should increment the value.
*/
void semm_post(Semamore *s) {

}

/**
* Takes a pointer to a Semamore struct to help cleanup some members of the
* struct.
* The actual Semamore struct must be freed by the user.
*/
void semm_destroy(Semamore *s) {
	free(s -> m);
	free(s -> cv);
}
