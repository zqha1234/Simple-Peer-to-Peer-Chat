#include "socket_thread.h"
#include "send_receive.h"

//global variable
pthread_mutex_t mutex1; 
pthread_cond_t cond1;

pthread_mutex_t mutex2; 
pthread_cond_t cond2;

//threads
pthread_t input_thread;
pthread_t output_thread;
pthread_t send_thread;
pthread_t receive_thread;

extern unsigned short g_local_Port;
extern unsigned short g_dest_Port;

struct local_socket local_sk;

//create socket
int create_socket(){
    local_sk.fd = socket(AF_INET, SOCK_DGRAM, 0);
    //check if the socket is created sucessfully
    if(local_sk.fd == -1){
        perror("Socket Creation Error!\n");
        exit(1);
    }
    memset(&(local_sk.local_ska_in), 0, sizeof(local_sk.local_ska_in));
    local_sk.local_ska_in.sin_family = AF_INET;
    local_sk.local_ska_in.sin_addr.s_addr = htonl(INADDR_ANY);
    local_sk.local_ska_in.sin_port = g_local_Port;
    local_sk.addr_len = sizeof(struct sockaddr_in);
    // bind
    int result_bind = bind(local_sk.fd, (struct sockaddr *)&(local_sk.local_ska_in), sizeof(local_sk.local_ska_in));
    if(result_bind == -1){
        perror("Bind Error!");
        exit(1);
    }
    return 0;
}

//initiate the mutex and the condition variable
//mutex1 and cond1 are for input and send, and mutex2 and cond2 are for receive and output
void thread_cond_mutex_init(){
    int result_cond = pthread_cond_init(&cond1, NULL);
    //check if the cond initiate sucessfully
    if (result_cond != 0){
    	perror("Thread_cond initiation error\n");
    	exit(1);
    }
    
    int result_mutex = pthread_mutex_init(&mutex1, NULL);
    //check if the mutex initiate sucessfully
    if (result_mutex != 0){
    	perror("Thread_mutex initiation error\n");
    	exit(1);
    }
    
    result_cond = pthread_cond_init(&cond2, NULL);
    if (result_cond != 0){
    	perror("Thread_cond initiation error\n");
    	exit(1);
    }
    
    result_mutex = pthread_mutex_init(&mutex2, NULL);
    if (result_mutex != 0){
    	perror("Thread_mutex initiation error\n");
    	exit(1);
    }
}

//createa threads
// 4 threads in total - one for input, one for send, one for receive, and one for output
void thread_create(){
    //create input thread
    //pthread_t input_thread;
    int result_thread;
    result_thread = pthread_create(&input_thread, NULL, input_function, NULL);
    //check if the thread is created sucessfully
    if (result_thread != 0){
        perror("Thread Creation Error!\n");
        exit(1);
    }

    //create output thread
    //pthread_t output_thread;
    result_thread = pthread_create(&output_thread, NULL, output_function, NULL);
    if (result_thread != 0){
        perror("Thread Creation Error!\n");
        exit(1);
    }

    //create send thread
    //pthread_t send_thread;
    result_thread = pthread_create(&send_thread, NULL, send_function, &local_sk);
    if (result_thread != 0){
        perror("Thread Creation Error!\n");
        exit(1);
    }

    //create receive thread
    //pthread_t receive_thread;
    result_thread = pthread_create(&receive_thread, NULL, receive_function, &local_sk);
    if (result_thread != 0){
        perror("Thread Creation Error!\n");
        exit(1);
    }
}

//wait for shutting threads down, destorying mutex and cond, and closing socket
void wait_for_shutdown(){
    //wait for shutting threads down
    pthread_join(input_thread, NULL);
    pthread_join(output_thread, NULL);
    pthread_join(receive_thread, NULL);
    pthread_join(send_thread, NULL);
    
    //destory mutex and cond
    pthread_mutex_destroy(&mutex1);
    pthread_cond_destroy(&cond1);    
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&cond2);
    
    //close socket
    close(local_sk.fd);
}

