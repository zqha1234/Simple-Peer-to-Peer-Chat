#include "send_receive.h"
#include "socket_thread.h"
#include "list.h"

int g_flag1 = 0;
int g_flag2 = 0;
//thread_exit_flag
bool exit_flag = false;

extern List* pList_send;
extern List* pList_rec;

extern char host_name [50];
extern char g_ip [20];

extern unsigned short g_local_Port;
extern unsigned short g_dest_Port;

extern pthread_mutex_t mutex1; 
extern pthread_cond_t cond1;

extern pthread_mutex_t mutex2; 
extern pthread_cond_t cond2;

extern pthread_t input_thread;
extern pthread_t output_thread;
extern pthread_t send_thread;
extern pthread_t receive_thread;

//Input from the keyboard. And store the input data in the pList_send
void *input_function(void* unused){   
    char send_msg[256] = {0};
    while (1){
        if (fgets(send_msg, sizeof(send_msg), stdin) == NULL){
            //do nothing, allow empty message, and send empty message
        }
        pthread_mutex_lock(&mutex1); //lock the critical section
        g_flag1 = 1;
        //check if the input is '!', if it is '!', set exit_flag to true
        if( (send_msg[0] == '!') && (strlen(send_msg) == 2)){ //string in c: ['!', '\0']
            exit_flag = true;
        }
        List_append(pList_send, send_msg); //store the input data in the pList_send
        pthread_mutex_unlock(&mutex1); //unlock the critical section
        pthread_cond_signal(&cond1); // signal (wake up) the other thread 
        sleep(1);
    }
    return NULL;
}

//Send the message to the terminal
void *send_function(void* lsock){   
    struct local_socket *l_sk = (struct local_socket *) lsock;
    struct sockaddr_in dst_addr;
    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = g_dest_Port;
    inet_pton(AF_INET, g_ip, &dst_addr.sin_addr.s_addr);
    while(1){
        pthread_mutex_lock(&mutex1); //lock the critical section
        //condition check
        if (g_flag1 == 0){
            pthread_cond_wait(&cond1, &mutex1);
        }
        g_flag1 = 0;
        //send message
        List* temp = List_trim(pList_send);
        char* send_list = (char*)temp->nodeData->data;
        if (send_list != NULL){                    
            int i = sendto(l_sk->fd, send_list, 256, 0, (struct sockaddr *)&dst_addr, sizeof(dst_addr));
            if (i == -1) {
                fputs("The last message didn't send sucessfully", stdout);
            }
        }

        if (temp != NULL){
            //free the node (in the pList_send) after take out the data
            //avoid leaking memory
            free(temp->nodeData);
            free(temp);
        }
        pthread_mutex_unlock(&mutex1); //unlock the critical section       
        //if exit_flag is true, then cancel the threads and exit the program
        if (exit_flag == true){
            pthread_cancel(input_thread);
            pthread_cancel(receive_thread);
            pthread_cancel(output_thread);
    	    return NULL;
    	}
    }
    return NULL;
}


//Receive teh data from the other terminal, and store the data in the pList_rec
void *receive_function(void* lsock){
    struct local_socket *l_sk = (struct local_socket *) lsock;
    struct sockaddr_in addr;
    char rec_msg[256] = {0};
    while(1){
        //receive message
        recvfrom(l_sk->fd, rec_msg, 256, 0, (struct sockaddr *)&addr, &l_sk->addr_len);
        pthread_mutex_lock(&mutex2); //lock the critical section
        g_flag2 = 1;
        //check if the received data is '!', if it is '!', then set exit_flage to true
        if( (rec_msg[0] == '!') && (strlen(rec_msg) == 2)){ //string in c: ['!', '\0']
            exit_flag = true;
        }
        //store the data in the pList_rec
        List_append(pList_rec, rec_msg);
        pthread_mutex_unlock(&mutex2); //unlock the critical section
        pthread_cond_signal(&cond2); //signal (wake up) the other thread
        sleep(1); 
    }
    return NULL;
}

void *output_function(void* unused){
    //output to the screen 
    while (1){
        pthread_mutex_lock(&mutex2); //lock the critical section
        //condition check
        if (g_flag2 == 0){
            pthread_cond_wait(&cond2, &mutex2);
        }
        g_flag2 = 0;
        List* temp = List_trim(pList_rec);
        char* rec_list = (char*)temp->nodeData->data;
        char dest_port[10];
        snprintf(dest_port, sizeof(dest_port), "%hu", g_dest_Port);
        //use fputs to out the data on the screen
        fputs("Message from ", stdout);
        fputs(host_name, stdout);
        fputs(" ", stdout);
        fputs(dest_port, stdout);
        fputs(": ", stdout);
        fputs(rec_list, stdout);
        if (temp != NULL){
            //free the node (in the pList_send) after take out the data
            //avoid leaking memory
            free(temp->nodeData);
            free(temp);
        }
        pthread_mutex_unlock(&mutex2); //unlock the critical section
        //if exit_flag is true, then cancel the threads and exit the program
        if (exit_flag == true){
            pthread_cancel(input_thread);
            pthread_cancel(send_thread);
            pthread_cancel(receive_thread);
    	    return NULL;
    	}
    }
    return NULL;
}

