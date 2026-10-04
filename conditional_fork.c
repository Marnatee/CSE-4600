#include <stdio.h>   // Provides printf().
#include <unistd.h>  // Provides fork(), getpid(), getppid(), and sleep().

int main() {  // Begins the program.

    // Creates the first child process.
    // The child receives 0, while the parent receives the child's PID.
    int value = fork();

    // Only the first child enters this branch.
    if (value == 0) {

        // Creates another process from the first child.
        fork();

        // Each process in this branch checks its own PID.
        if (getpid() % 2 == 0) {

            // Only a process with an even PID creates another child.
            fork();
        }

    } else {

        // The original parent creates another direct child.
        fork();
    }

    // Every process created by the program reaches this statement.
    printf("Hello from PID: %d, PPID: %d\n", getpid(), getppid());

    // Keeps each process alive briefly so its output can be observed.
    sleep(2);

    // Ends the process successfully.
    return 0;
}
