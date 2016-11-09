/**
* Finding Filesystems
* CS 241 - Fall 2016
*/
#include "format.h"
#include "fs.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

void fs_ls(file_system *fs, char *path) {
	// Arrrrrgh Matey
	inode *res = NULL;
	dirent dir;
	if(!(res = get_inode(fs, path))){
		print_no_file_or_directory();
		return;
	}
	if(is_file(res)){
		char *file = NULL;
		parent_directory(fs, path, &file);
		print_file(file);
	}
	else if(is_directory(res)){
		int num_entries = res -> size / 256;
		for(size_t i = 0; i < 11; i++){
			for (size_t j = 0; j < 64; j++) {
				char *same = (char *)(((void *)(&(fs -> data_root[res -> direct_nodes[i]]))) + 256 * j);
				make_dirent_from_string(same, &dir);

				inode *file_or_dir = &(fs -> inode_root[dir.inode_num]);
				if(is_file(file_or_dir)) print_file(dir.name);
				else if(is_directory(file_or_dir)) print_directory(dir.name);
				else{
					print_no_file_or_directory();
					return;
				}
				// printf("num_entries: %d\n", num_entries);
				num_entries--;
				if(num_entries == 0) break;
			}
			if(num_entries == 0) break;
		}
		res = &(fs -> inode_root[res -> single_indirect]);
		// num_entries = res -> size / 256;
		for(size_t i = 0; i < 11; i++){
			for (size_t j = 0; j < 64; j++) {
				char *same = (char *)(((void *)(&(fs -> data_root[res -> direct_nodes[i]]))) + 256 * j);
				make_dirent_from_string(same, &dir);

				inode *file_or_dir = &(fs -> inode_root[dir.inode_num]);
				if(is_file(file_or_dir)) print_file(dir.name);
				else if(is_directory(file_or_dir)) print_directory(dir.name);
				else{
					print_no_file_or_directory();
					return;
				}
				// printf("num_entries: %d\n", num_entries);
				num_entries--;
				if(num_entries == 0) return;
			}
		}
	}
}

void fs_cat(file_system *fs, char *path) {
	// Shiver me triggers
	inode *res = NULL;
	if(!(res = get_inode(fs, path))){
		print_no_file_or_directory();
		return;
	}

	size_t count = (size_t)((res -> size + sizeof(data_block) - 1) / sizeof(data_block));
	size_t max = (count > 11) ? 11 : count;
	for (size_t i = 0; i < max; i++) {
		data_block data = fs -> data_root[res -> direct_nodes[i]];
		write(fileno(stdout), &data, 16*KILOBYTE);
	}
	if(count > max){
		for(size_t j = max; j < count; j++){
			data_block indir_data = fs -> data_root[fs -> inode_root[res -> single_indirect].direct_nodes[j - 11]];
			write(fileno(stdout), &indir_data, 16*KILOBYTE);
		}
	}
}