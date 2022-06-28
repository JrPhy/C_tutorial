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
int a = 2;
pthread_t tid;
pthread_t readerthreads[a];
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
 
    sleep(20);
 
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
int main()
{
    // Initialize variables
    int serverSocket, newSocket, port = 0;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    char buf[4096], ip[16] = "-1";
    char *retPort, *retIP, *p;
 
    socklen_t addr_size;
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);
 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
    int thread_num = 0, thread_total = 1;
    pthread_t tid[thread_total];

    while (1) {
        addr_size = sizeof(serverStorage);
 
        // Extract the first
        // connection in the queue
        newSocket = accept(serverSocket,
                           (struct sockaddr*)&serverStorage,
                           &addr_size);
        int status = 0;
        if (pthread_create(&readerthreads[thread_num++], NULL,
                            reader, &newSocket) != 0)
            // Error in creating thread
        {
            printf("Failed to create thread\n");
            status = -1;
            send(newSocket, &status, sizeof(status), 0);
        }
        else
        {
            printf("create thread sucessfully\n");
            status = 1;
            send(newSocket, &status, sizeof(status), 0);
        }
        if (thread_num >= thread_total) {
            // Update i
            thread_num = 0;
            printf("Server is full, please wait\n");
            while (thread_num < thread_total) {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
                pthread_join(readerthreads[thread_num++], NULL);
            }
 
            // Update i
            thread_num = 0;
        }
    }
 
    return 0;
}
