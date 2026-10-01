// Complete example: draw the actual relationships, not the old misleading labels.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void){
    setbuf(stdout, NULL);
    pid_t child_pid, parent_pid;

    // Create 5 sibling processes from the same original parent.
    printf("Create 5 sibling processes from the same parent:\n");

    for (int i = 0; i < 5; i++){

        // fork() creates a child process.
        // The child enters this if statement because fork() returns 0.
        if (fork() == 0) {

            // Get the child's process ID and its parent's process ID.
            child_pid = getpid();
            parent_pid = getppid();

            printf("Child Process (ID): %d, parent process (ID): %d\n",
                   child_pid, parent_pid);

            // Child finishes so it does not create additional processes.
            exit(0);
        }

        // Parent waits for the child before creating the next sibling.
        wait(NULL);
    }


    // Create the branching tree described in the handout.
    printf("\nCreate the handout branching tree (not a full binary tree):\n");

    for (int i = 0; i < 2; i++){

        // Create a child from the original parent.
        if (fork() == 0) {

            child_pid = getpid();
            parent_pid = getppid();

            // Display the relationship between this child and its parent.
            printf("1. Child Process (ID): %d, parent process (ID): %d\n",
                   child_pid, parent_pid);

            // Only the first child creates another child.
            if (i == 0) {

                // Create a nested child (grandchild of the original parent).
                fork();

                child_pid = getpid();
                parent_pid = getppid();

                // Display the nested process relationship.
                printf("2. Child Process (ID): %d, parent process (ID): %d\n",
                       child_pid, parent_pid);
            }

            // Wait for the nested child if this process created one.
            wait(NULL);

            exit(0);
        }

        // Original parent waits for each child to finish.
        wait(NULL);
    }


    // Create a star topology:
    // one parent in the center with 6 child processes.
    printf("\nStar topology of processes:\n");

    for (int i = 0; i < 6; i++){

        // Each fork creates a child directly from the same parent.
        if (fork() == 0) {

            child_pid = getpid();
            parent_pid = getppid();

            printf("Child Process (ID): %d, parent process (ID): %d\n",
                   child_pid, parent_pid);

            // Child exits so it does not create additional children.
            exit(0);
        }
    }

    // Parent waits for all 6 children to finish.
    for (int i = 0; i < 6; i++){
        wait(NULL);
    }

    return 0;
}