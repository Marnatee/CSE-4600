// Original pipe3.cpp: send two numbers through the existing pipe and add them.
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Example: ./pipe3 10 5" << endl; // Expecting two numbers
        return 1;
    }
    int numbers[2] = {0, 0};
    numbers[0] = atoi(argv[1]);
    numbers[1] = atoi(argv[2]); // Convert argv[1] and argv[2] to integers with atoi.
    int fd[2];
    if (pipe(fd) == 0) {
        int received[2] = {0, 0}; // Create an array to store the received numbers from the pipe
        write(fd[1], numbers, sizeof(numbers)); // Write numbers to fd[1]
        read(fd[0], received, sizeof(received)); // Read into received from fd[0]
        cout << "Sent the numbers " << numbers[0] << " and " << numbers[1]
             << " to pipe for their summation." << endl;
        cout << "The sum of the two numbers is: " << received[0] + received[1] << endl; // Print the sum of the two numbers
        close(fd[0]);
        close(fd[1]); // Close the pipe ends after use
        return 0;
    }
    return 1;
}
