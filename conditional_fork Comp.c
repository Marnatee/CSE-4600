// Complete example: run, add explanations, and draw the observed tree.
#include <stdio.h>
#include <unistd.h>

int main() {
    // The original process creates its first child.
    int value = fork();

    if (value == 0) {

        // This child creates another child.
        fork();

        // Check the PID of each process created in this branch.
        if (getpid() % 2 == 0) {

            // Only processes with an even PID create another child.
            fork();
        }

    } else {

        // The original parent creates another child.
        fork();
    }

    // Every process that exists at this point prints its PID and parent PID.
    printf("Hello from PID: %d, PPID: %d\n", getpid(), getppid());

    // Keep the processes alive briefly so their output can be observed.
    sleep(2);

    return 0;
}