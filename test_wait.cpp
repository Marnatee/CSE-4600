// Original test_wait.cpp, with a space for the required grandchild extension.
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main() {
    pid_t pid;
    const char *message;  // const because the messages are string literals.
    int n;
    int exit_code;
    pid_t grandparent_pid = getpid();  // Save before the first fork.

    cout << "fork program starting" << endl;
    pid = fork();
    switch (pid) {
    case -1:
        cout << "Fork failure!" << endl;
        return 1;
    case 0: {
        
        // YOUR CODE HERE: this child creates one grandchild.
        // Grandchild: print its PID, parent PID, and saved grandparent PID;
        // then exit(0). Child: wait for the grandchild before continuing.
        // Suggested label: Grandchild PID: ..., Parent PID: ..., Grandparent PID: ...
        pid_t grandchild_pid = fork();

        if (grandchild_pid == -1) {
        cout << "Grandchild fork failure!" << endl;
        return 1;
        }
        else if (grandchild_pid == 0) {
          cout << "Grandchild PID: " << getpid()
                 << ", Parent PID: " << getppid()
                 << ", Grandparent PID: " << grandparent_pid << endl;
        exit(0);
        }
        else {
            wait(NULL);
        }

        message = "This is the child\n";
        n = 5;
        exit_code = 9;
        break;
    }
    default:
        message = "This is the parent\n";
        n = 3;
        exit_code = 0;
        break;
    }
    for (int i = 0; i < n; ++i) {
        cout << message;
        sleep(1);
    }
    if (pid != 0) {
        int stat_val;
        pid_t child_pid = wait(&stat_val);
        cout << "Child finished: PID = " << child_pid << endl;
        if (WIFEXITED(stat_val))
            cout << "child exited with code " << WEXITSTATUS(stat_val) << endl;
        else
            cout << "child terminated abnormally!" << endl;
    }
    exit(exit_code);
}
