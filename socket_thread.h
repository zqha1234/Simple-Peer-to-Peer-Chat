#ifndef _SOCKET_THREAD_h
#define  _SOCKET_THREAD_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

struct local_socket{
    int fd;
    struct sockaddr_in local_ska_in;
    unsigned int addr_len;
};

int create_socket();
void thread_cond_mutex_init();
void thread_create();
void wait_for_shutdown();


#endif
