// C program for the Server Side
 
// inet_addr
#include <arpa/inet.h>
 
// For threading, link with lpthread
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
 
// Semaphore variables
sem_t x, y;
pthread_t tid;
pthread_t readerthreads[100];
int readercount = 0;
 
// Reader Function
void* reader(void* param)
{
    // Lock the semaphore
    sem_wait(&x);
    readercount++;
 
    if (readercount == 1)
        sem_wait(&y);
 
    // Unlock the semaphore
    sem_post(&x);
 
    printf("\n%d CTB is inside\n", readercount);
 
    //sleep(20);
 
    // Lock the semaphore
    sem_wait(&x);
    readercount--;
 
    if (readercount == 0) {
        printf("readercount == 0\n");
        sem_post(&y);
    }
 
    // Lock the semaphore
    sem_post(&x);
 
    printf("\n%d CTB is inside\n", readercount);
    pthread_exit(NULL);
}
// Driver Code
int timeout_recv(int fd, char *buf, int len,  int nsec)
{
    struct timeval timeout;
    timeout.tv_sec = nsec;
    timeout.tv_usec = 0;

    printf("timeout_recv called, timeout %d seconds\n", nsec);

    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt error");
        exit(1);
    }

    int n = recv(fd, buf, len, 0);

    return n;
}

int main()
{
    // Initialize variables
    int serverSocket, newSocket, port = 0, i = 0;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;

    socklen_t addr_size;
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
 
    // Bind the socket to the
    // address and port number.
    bind(serverSocket,
         (struct sockaddr*)&serverAddr,
         sizeof(serverAddr));
 
    // Listen on the socket,
    // with 40 max connection
    // requests queued
    if (listen(serverSocket, 128) == 0) printf("Listening\n");
    else printf("Error\n");
 
    // Array for thread
    int thread_num = 0, thread_total = 10, status = 0, nread = -1, kkk = 0;
    pthread_t tid[thread_total];

    while (1) {
        addr_size = sizeof(serverStorage);
        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket,
                           (struct sockaddr*)&serverStorage,
                           &addr_size);
                           
        if (pthread_create(&readerthreads[thread_num++], NULL,
                            reader, &newSocket) != 0)
            // Error in creating thread
        {
            printf("Failed to create thread\n");
            status = -1;
        }
        else
        {
            printf("create thread sucessfully\n");
            status = 1;
        }
        if( send(newSocket, &status, sizeof(status), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        while(recv(newSocket, &kkk, sizeof(kkk), 0) > 0) 
        {
            if(kkk%10000000==0) printf("%d\n", kkk);
        }
        //若數量已滿，client 端直接退出
        //log: 使用者，連上時間，command
        nread = timeout_recv(serverSocket, buf, sizeof(buf), 10);
        if (nread == 0) {
            printf("time out\n");
            shutdown(serverSocket, SHUT_RDWR);
            break;
        }
        if (thread_num >= thread_total) {
            // Update thread_num
            thread_num = 0;
            printf("Server is full, please wait\n");
            while (thread_num < thread_total) {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
                pthread_join(readerthreads[thread_num++], NULL);
            }
            // Update thread_num
            thread_num = 0;
        }
    }
    return 0;
}
