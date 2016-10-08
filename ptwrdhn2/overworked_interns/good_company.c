/**
* Overworked Interns Lab
* CS 241 - Fall 2016
*/
#include "company.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *work_interns(void *p) {

	Company *meme_inc = (Company *)p;
	pthread_mutex_t *left_intern, *right_intern;


	while (running) {
		int comp_num = Company_get_company_number(meme_inc);
		left_intern = Company_get_left_intern(meme_inc);
		right_intern = Company_get_right_intern(meme_inc);

		if(comp_num % 2){
			pthread_mutex_t *temp = left_intern;
			left_intern = right_intern;
			right_intern = temp;
		}
		if(left_intern != right_intern){
			pthread_mutex_lock(left_intern);
			pthread_mutex_lock(right_intern);

			Company_hire_interns(meme_inc);

			pthread_mutex_unlock(right_intern);
			pthread_mutex_unlock(left_intern);
		}
		Company_have_board_meeting(meme_inc);
	}
	return NULL;
}
