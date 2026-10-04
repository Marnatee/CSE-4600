// Complete example: explain the messages; do not rewrite the algorithm.
//test_fork.cpp
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>  // Defines pid_t for process IDs.
#include <sys/wait.h>   // Provides wait().
#include <unistd.h>     // Provides fork() and sleep().
#include <iostream>     // Provides cout and endl.

using namespace std;  // Allows standard output names without the std:: prefix.

int main() {  // Begins the program.

    // Stores the value returned by fork().
    pid_t pid;

    // Points to the message selected for each process.
    const char *message;

    // Stores how many times each process prints its message.
    int n;

    // Prints and flushes the startup message before fork().
    cout << "fork program starting" << endl;

    // Creates a child process.
    // The child receives 0, while the parent receives the child's PID.
    pid = fork();

    // Selects the correct behavior using the fork() return value.
    switch (pid) {

    // A value of -1 means that fork() failed.
    case -1:

        // Reports that no child was created.
        cout << "Fork failure!\n";

        // Ends the program with an error status.
        return 1;

    // A value of 0 identifies the child process.
    case 0:

        // Selects the child's output message.
        message = "This is the child\n";

        // Makes the child print five times.
        n = 5;

        // Leaves the switch statement.
        break;

    // A positive value identifies the original parent process.
    default:

        // Selects the parent's output message.
        message = "This is the parent\n";

        // Makes the parent print three times.
        n = 3;

        // Leaves the switch statement.
        break;
    }

    // Each process repeats according to its own value of n.
    for (int i = 0; i < n; ++i) {

        // Prints the message selected for this process.
        cout << message;

        // Pauses the process for approximately one second.
        sleep(1);
    }

    // Only the original parent waits for and collects the child.
    if (pid > 0)
        wait(NULL);

    // Ends the process successfully.
    return 0;
}
}
