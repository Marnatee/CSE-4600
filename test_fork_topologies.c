// Complete example: draw the actual relationships, not the old misleading labels.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {  // Begins the program.

    // Disables output buffering so fork does not duplicate unprinted output.
    setbuf(stdout, NULL);

    // Stores the process IDs displayed by each example.
    pid_t child_pid, parent_pid;

    // Labels the first process relationship.
    printf("Create 5 sibling processes from the same parent:\n");

    // Repeats five times to create five direct children.
    for (int i = 0; i < 5; i++) {

        // fork() creates a child, and only that child enters this block.
        if (fork() == 0) {

            // Gets the current child's process ID.
            child_pid = getpid();

            // Gets the process ID of the child's parent.
            parent_pid = getppid();

            // Displays the relationship between the child and parent.
            printf("Child Process (ID): %d, parent process (ID): %d\n",
                   child_pid, parent_pid);

            // Ends the child so it cannot continue the loop.
            exit(0);
        }

        // The original parent waits for the current child to finish.
        wait(NULL);
    }

    // Labels the small branching-tree example.
    printf("\nCreate the handout branching tree (not a full binary tree):\n");

    // Creates two direct children of the original parent.
    for (int i = 0; i < 2; i++) {

        // Creates a child, which enters this block when fork() returns 0.
        if (fork() == 0) {

            // Gets this direct child's PID.
            child_pid = getpid();

            // Gets the original parent's PID.
            parent_pid = getppid();

            // Prints the direct child-to-parent relationship.
            printf("1. Child Process (ID): %d, parent process (ID): %d\n",
                   child_pid, parent_pid);

            // Only the first direct child creates another process.
            if (i == 0) {

                // Creates one grandchild of the original process.
                fork();

                // Gets the PID of the process currently executing this line.
                child_pid = getpid();

                // Gets its current parent's PID.
                parent_pid = getppid();

                // Both the first child and its new child print here.
                printf("2. Child Process (ID): %d, parent process (ID): %d\n",
                       child_pid, parent_pid);
            }

            // Waits for the nested child if this process created one.
            wait(NULL);

            // Stops this child from continuing the outer loop.
            exit(0);
        }

        // The original parent waits for each direct child.
        wait(NULL);
    }

    // Labels the six-child star relationship.
    printf("\nStar topology of processes:\n");

    // Creates six children from the same original parent.
    for (int i = 0; i < 6; i++) {

        // Only each newly created child enters this block.
        if (fork() == 0) {

            // Gets the current child's PID.
            child_pid = getpid();

            // Gets the common parent's PID.
            parent_pid = getppid();

            // Displays the relationship between this child and the parent.
            printf("Child Process (ID): %d, parent process (ID): %d\n",
                   child_pid, parent_pid);

            // Prevents the child from continuing the creation loop.
            exit(0);
        }
    }

    // Reaps all six children created in the preceding loop.
    for (int i = 0; i < 6; i++) {

        // Waits for one unfinished child during each iteration.
        wait(NULL);
    }

    // Ends the original parent successfully.
    return 0;
}
