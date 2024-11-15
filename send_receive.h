#ifndef _SEND_RECEIVE_h
#define  _SEND_RECEIVE_h

#include <stdbool.h>
#include <string.h>
#include <unistd.h>

void *input_function(void* unused);
void *send_function(void* lsock);
void *receive_function(void* lsock);
void *output_function(void* unused);

#endif
