#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <stdio.h>

//Worker thread
void *thread_func(void *arg) {
	//This pointer refers to the same memory allocated by main thread
    char *message = (char *)arg;
	//Replace message with worker thread's message
    strcpy(message, "You are in thread function!\n");
    return NULL;
}
int main() {
	//Allocating memory for the shared message
    char *message = (char *)malloc(100);
	//Return NULL if memory allocation fails
    if (message == NULL) return 1;
	//Storing main thread original message in the allocated memory
    strcpy(message, "You are in the main function!\n");
	//Declaring variable that holds identifier for worker thread
    pthread_t tid1;
	//Create worker thread
    pthread_create(&tid1, NULL, thread_func, message);
    // YOUR CODE HERE: put pthread_join and printf in the correct order so
    // the thread's updated message is ALWAYS printed.
	//Waits until the worker thread finishes modifying message
	pthread_join(tid1, NULL);
	//Prints message after worker thread has updated it
	printf("%s", message);
    // Free only after the worker has finished using message.
	free(message);
    return 0;
}
