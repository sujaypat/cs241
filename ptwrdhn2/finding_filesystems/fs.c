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
	// superblock *sblock = fs -> meta;
	// inode *root = fs -> inode_root;
	// data_block *first_data = fs -> data_root;

	printf("%s\n", path);
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