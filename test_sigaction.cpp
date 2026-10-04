// The sigaction extension requested after test_signal.cpp in the handout.
#include <signal.h>
#include <unistd.h>
#include <iostream>
using namespace std;

// Flag the handler sets and main() reads.
// the compiler must re-read it each loop, since a handler can change it.
// sig_atomic_t reads/writes can't be torn when running or no errors 
volatile sig_atomic_t quit_requested = 0;

// Runs asynchronously when a registered signal arrives, interrupting main().
void func(int sig) {
    if (sig == SIGINT) {   // Ctrl+C or copys, if statment (conditional)
        // Only signal-safe calls are allowed. write() qualifies;
        // cout does not (it uses buffers/locks that could be mid-update in main).
        const char text[] = "Received SIGINT; program continues.\n";
        write(STDOUT_FILENO, text, sizeof(text) - 1);  // -1 drops the '\0'since it is not neededd
    }
    // Ctrl+\ : just set the flag; let main() do the actual shutdown.
    if (sig == SIGQUIT) quit_requested = 1;
}

int main() {
    struct sigaction action;
    action.sa_handler = func;       // function to call on signal
    sigemptyset(&action.sa_mask);   // block no extra signals during the handler
    action.sa_flags = 0;            // no special behavior flags

    // Register the handler for both signals
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGQUIT, &action, nullptr);

    cout << "Ready: Ctrl+C continues; Ctrl+backslash quits." << endl;

    // Sleep until the flag flips. A signal wakes sleep() early, so the
    // flag gets rechecked right away. 
    while (!quit_requested) sleep(1);

    cout << "Program finished." << endl;  // safe: we're back in normal code
    return 0;
}
