#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <stdio.h>

void *thread_func(void *arg) {
    char *message = (char *)arg;
    strcpy(message, "You are in thread function!\n");
    return NULL;
}
int main() {
    char *message = (char *)malloc(100);
    if (message == NULL) return 1;
    strcpy(message, "You are in the main function!\n");
    pthread_t tid1;
    pthread_create(&tid1, NULL, thread_func, message);
    // YOUR CODE HERE: put pthread_join and printf in the correct order so
    // the thread's updated message is ALWAYS printed.
    // Free only after the worker has finished using message.
    return 0;
}
