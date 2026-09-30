// The final handout exercise: replace processes/pipes with threads and a mutex.
#include <stdio.h>
#include <pthread.h>

#define SIZE 48
//4 worker threads
#define NUM_THREADS 4
//global array shared by all threads
int array[SIZE];
//global variable storing result of all threads
int total_sum = 0;
//intialize the mutex
//protects updates to total_sum
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Calculates partial sum of array elements
int partial_sum(int start, int end) {
    int sum = 0;
    for (int i = start; i < end; i++) sum += array[i];
    return sum;
}
//This is used by all 4 threads
void *sum_thread(void *arg) {
	//Retrieve this worker's thread number from the passed pointer
    int i = *(int *)arg;
	//Divides array elements for each worker thread
    int segment_size = SIZE / NUM_THREADS;
	//Calculate index of where this current worker thread starts
    int start = i * segment_size;
	//Calculate index of where this current worker thread ends
    int end = start + segment_size;
	//Calculate this thread's value indepent of the other threads
	//partialResult is local to this current thread, does not need Mutex
    int partialResult = partial_sum(start, end);
    // YOUR CODE HERE: lock mutex, add partialResult to total_sum, then unlock.
	pthread_mutex_lock(&mutex);
	//CRITICAL SECTION start
	total_sum += partialResult;
	//CRITICAL SECTION end
	pthread_mutex_unlock(&mutex);
    return NULL;
}
int main() {
	//initialize array from 1 to SIZE
    for (int i = 0; i < SIZE; i++) array[i] = i + 1;
	//store identifiers for all four worker threads
    pthread_t threads[NUM_THREADS];
	//Store a thread number for each worker thread
    int thread_numbers[NUM_THREADS];
	//Create all 4 worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_numbers[i] = i;  // Each worker gets its own stable index.
        // YOUR CODE HERE: create threads[i], passing &thread_numbers[i].
		pthread_create(&threads[i],NULL,sum_thread,&thread_numbers[i]);
    }
	//Waits for all 4 worker threads to finish before displaying total_sum
    for (int i = 0; i < NUM_THREADS; i++) {
        // YOUR CODE HERE: join threads[i].
		pthread_join(threads[i],NULL);
    }
    printf("Total sum: %d\n", total_sum);
    return 0;
}
