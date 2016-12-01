/**
* Chatroom Lab
* CS 241 - Fall 2016
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include "utils.h"
static const size_t MESSAGE_SIZE_DIGITS = 4;

char *create_message(char *name, char *message) {
	int name_len = strlen(name);
	int msg_len = strlen(message);
	char *msg = calloc(1, msg_len + name_len + 4);
	sprintf(msg, "%s: %s", name, message);

	return msg;
}

ssize_t get_message_size(int socket) {
	int32_t size;
	ssize_t read_bytes = read_all_from_socket(socket, (char *)&size, MESSAGE_SIZE_DIGITS);
	if (read_bytes == 0 || read_bytes == -1) return read_bytes;

	return (ssize_t)ntohl(size);
}

ssize_t write_message_size(size_t size, int socket) {
	ssize_t res;
	ssize_t payload = (ssize_t)htonl(size);

	res = write_all_to_socket(socket, (char *)&(payload), (MESSAGE_SIZE_DIGITS));

	return res;
}

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
	ssize_t res;
	size_t progress = 0;
	while(progress != count){
		res = read(socket, buffer + progress, count - progress);
		if(res != -1) progress += res;
		else{
			if(res == -1 && errno == EINTR) continue;
			if((res == -1 && errno != EINTR)) return -1;
		}
	}
	return res;
}

ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
	ssize_t res;
	size_t progress = 0;
	// fprintf(stderr, "buffer to write back: %s\n", buffer);
	while(progress != count){
		res = write(socket, buffer + progress, count - progress);
		if(res != -1) progress += res;
		else{
			if(res == -1 && errno == EINTR) continue;
			if((res == -1 && errno != EINTR)) return -1;
		}
	}
	return res;
}