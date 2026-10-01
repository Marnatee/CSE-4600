// The sigaction extension requested after test_signal.cpp in the handout.
#include <signal.h>
#include <unistd.h>
#include <iostream>
using namespace std;

// Supplied handler support. This flag is safe to set from a signal handler.
volatile sig_atomic_t quit_requested = 0;
void func(int sig) {
    if (sig == SIGINT) {
        // write is used here because cout is not safe inside a signal handler.
        const char text[] = "Received SIGINT; program continues.\n";
        write(STDOUT_FILENO, text, sizeof(text) - 1);
    }
    if (sig == SIGQUIT) quit_requested = 1;
}

int main() {
    struct sigaction action;
    action.sa_handler = func;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    // YOUR CODE HERE: call sigaction for SIGINT and SIGQUIT, using &action.
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGQUIT, &action, nullptr);

    cout << "Ready: Ctrl+C continues; Ctrl+backslash quits." << endl;
    while (!quit_requested) sleep(1);  // Supplied: wait without burning CPU.
    cout << "Program finished." << endl;
    return 0;
}
