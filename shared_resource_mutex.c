// Original shared_resource_mutex.c: add just the missing lock/unlock.
#include <stdio.h>
#include <pthread.h>

//300,000,000
#define iterations 300000000
//global variable (both worker threads have access)
long long shared_resource = 0;
//mutex initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Increments or decrements shared_resource
void *inc_dec_resource(void *arg) {
	//Convert pointer to integer pointer
	//One thread retrieves +1 and other retrieves -1
    int resource_value = *(int *)arg;
    // YOUR CODE HERE: lock mutex before this loop.
	pthread_mutex_lock(&mutex);
	//Critical Section
	//shared_resource is being modified
    for (int i = 0; i < iterations; i++) {
        shared_resource += resource_value;
    }
	//End of Critical Section
    // YOUR CODE HERE: unlock mutex after this loop.
	pthread_mutex_unlock(&mutex);
	//Terminate thread
    pthread_exit(NULL);
}

int main(void) {
	//Variables that store two worker thread identifiers
    pthread_t tid1, tid2;
	//1st worker adds 1 each time
    int value1 = 1;
	//Create 1st worker thread and pass it the address of value1
    pthread_create(&tid1, NULL, inc_dec_resource, &value1);
	//2nd worker subtracts 1 each time
    int value2 = -1;
	//Create 2nd worker thread and pass it the address of value2
    pthread_create(&tid2, NULL, inc_dec_resource, &value2);
	//Wait until both worker threads have completed their tasks
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
	//Expected value here is 0 if both worker threads executed equally
    printf("Shared resource value: %lld\n", shared_resource);
    return 0;
}
