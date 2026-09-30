// Original array, constants, partial_sum function and fork loop retained.
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE 48
#define NUM_PROCESSES 4
int array[SIZE];

int partial_sum(int start, int end) { // Calculate the sum of the array elements from index start to end
    int sum = 0;
    for (int i = start; i < end; i++) sum += array[i];
    return sum;
}

int main() { // Main function to calculate the total sum of the array using multiple processes
    for (int i = 0; i < SIZE; i++) array[i] = i + 1;
    int segment_size = SIZE / NUM_PROCESSES;
    int total_sum = 0;
    int fd[NUM_PROCESSES][2];

    for (int i = 0; i < NUM_PROCESSES; i++) { 
        fd[i][0] = fd[i][1] = -1;  // Safe placeholders until the pipe is created.
        if (pipe(fd[i]) == -1) { // Create a pipe for each process and check for errors
            perror("Pipe failed");
            return 1;
        }
    }
    for (int i = 0; i < NUM_PROCESSES; i++) { // Loop to create child processes for calculating partial sums
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return 1;
        } else if (pid == 0) {
            int start = i * segment_size;
            int end = start + segment_size;
            int partialResult = partial_sum(start, end);
            // Supplied: close this child's unused copies of all pipe ends.
            for (int j = 0; j < NUM_PROCESSES; j++) {
                close(fd[j][0]);
                if (j != i) close(fd[j][1]);
            }
            // YOUR CODE HERE: write partialResult to fd[i][1].
            write(fd[i][1], &partialResult, sizeof(partialResult)); // Write the partial result to the write end of the pipe
            close(fd[i][1]); // Close the write end of the pipe
            return 0;  // Sum travels in the pipe, not the exit status.
        }
    }
    for (int i = 0; i < NUM_PROCESSES; i++) close(fd[i][1]);
    for (int i = 0; i < NUM_PROCESSES; i++) {
        int partialResult = 0; 
        read(fd[i][0], &partialResult, sizeof(partialResult)); // Read one integer from fd[i][0]
        total_sum += partialResult; // Add the partial result to the total sum
        close(fd[i][0]); // Close the read end of the pipe after reading
        wait(NULL); // Wait for the child process to finish
    }
    printf("Total sum: %d\n", total_sum);
    return 0;
}
