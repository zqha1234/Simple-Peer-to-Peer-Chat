#include <netdb.h> 
#include "socket_thread.h"
#include "send_receive.h"
#include "list.h"

unsigned short g_local_Port;
unsigned short g_dest_Port;

char host_name [50];
char g_ip [20];

List* pList_send = NULL;
List* pList_rec = NULL;

//through the host name to get ip address
int getIp(const char *hostname) {
    struct addrinfo hints;
    struct addrinfo *res = NULL;
    struct sockaddr_in *addr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    int result = getaddrinfo(hostname, NULL, &hints, &res);
    //check if getaddrinfo get the ip adress
    if (result != 0) {
        perror("getaddrinfo error!");
        exit(1);
    }
    if (res != NULL) {
        addr = (struct sockaddr_in *)(res->ai_addr);
        inet_ntop(res->ai_family, &(addr->sin_addr), g_ip, 16);
        freeaddrinfo(res);
    }
    return 0;
}

// Provide a method on how to free an item
void Item_free(void* item){
    Node* nodeItem = (Node*)item;
    if (nodeItem != NULL){
        free(nodeItem);
        // nodeItem = NULL;
    }
} 

int main(){

    fputs("Please initiate s-talk 's-talk [local port number] [destination hostname] [destination port number]'\n\
    (Example: s-talk 6060 asb9804u-b02 6001):", stdout);
    char buffer[250];
    //input from the keyboard (host name and prot number)
    if (fgets(buffer, sizeof(buffer), stdin) == NULL){
    	fputs("invalid input", stdout);
    	exit(1);
    }
    buffer[strlen(buffer)-1] = 0;
    //checkn if the input is valid
    if(strncmp(buffer, "s-talk", 6) != 0){
        perror("Invalid Input, Input should begin with s-talk!\n");
        exit(1);
    }
    char st[10];
    sscanf(buffer, "%s %hu %s %hu", st, &g_local_Port, host_name, &g_dest_Port);   
    //get the ip address, and stored the ip address in g_ip
    getIp(host_name); 
    //call create_socket to to create socket
    create_socket();   
    //create two linked list to store and manage data
    //pList_send stores input data, and pList_rec stores receive data
    pList_send = List_create();
    pList_rec = List_create();
    //initiate mutex and condition variable
    thread_cond_mutex_init();
    //create thread
    thread_create();  
    //shutdown the thread
    wait_for_shutdown(); 
    //free the two linked lists
    List_free (pList_send, Item_free);
    List_free (pList_rec, Item_free);
    pList_send = NULL;
    pList_rec = NULL;

    return 0;
}


