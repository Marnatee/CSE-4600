// The two original calculations remain; add the third average thread.
#include <iostream>
#include <pthread.h>
#include <stdio.h>

int N = 10;
//Take numbers 1 - N and sum them
void *numbers_sum(void *arg) {
    (void)arg;
    int sum = 0;
    for (int i = 1; i <= N; i++) sum += i;
    // One printf call keeps each output line together when threads overlap.
    printf("Sum of the numbers from 1 to %d: %d\n", N, sum);
    pthread_exit(0);
    return NULL;
}
//Take numbers 1 - N and give the product
void *numbers_product(void *arg) {
    (void)arg;
    int product = 1;
    for (int i = 1; i <= N; i++) product *= i;
    printf("Product of the numbers from 1 to %d: %d\n", N, product);
    pthread_exit(0);
    return NULL;
}
//Take numbers 1 - N and calculate the average
void *numbers_average(void *arg) {
    (void)arg;
    // YOUR CODE HERE: add 1 through N, then divide by N using floating-point.
	int sum = 0;
	for (int i = 1; i <= N; i++) sum += i;
	//Forcing cast of float so Integer division doesn't happen
	float average = static_cast<float>(sum) / N;
    // Print: Average of the numbers from 1 to 10: 5.5
	printf("Average of the numbers from 1 to %d: %.1f\n", N, average);
	//Terminate thread
	pthread_exit(0);
    return NULL;
}
int main() {
	//Identifiers for the 3 threads
    pthread_t id1, id2, id3;
	//Objects that store the attributes for the created threads
    pthread_attr_t attr1, attr2, attr3;
	//Strings identifying each thread's task
    const char *tnames[3] = {"Sum Thread", "Product Thread", "Average Thread"};
	//Initializing thread objects
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);
	//Create thread#1 and make it execute "numbers_sum" 
    pthread_create(&id1, &attr1, numbers_sum, (void *)tnames[0]);
	//Create thread#2 and make it execute "numbers_product"
    pthread_create(&id2, &attr2, numbers_product, (void *)tnames[1]);
	//Create thread#3 and make it execute "numbers_average"
	pthread_create(&id3, &attr3, numbers_average, (void *)tnames[2]);
    // YOUR CODE HERE: create id3 with numbers_average and attr3.
	//Join the threads
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
	pthread_join(id3, NULL);
    // YOUR CODE HERE: join id3.
	//Release the resources used by each thread
    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);
    pthread_attr_destroy(&attr3);
    return 0;
}
