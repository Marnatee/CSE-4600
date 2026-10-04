// The handout's example: child sleeps, then sends SIGALRM to its parent.
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
using namespace std;

// Supplied safety correction: set a flag here; print from main, not the handler.
volatile sig_atomic_t alarm_received = 0;
void alarm_off(int sig) {
    (void)sig;
    alarm_received = 1;
}
int main() {
    int pid;
    cout << "Alarm testing!" << endl;
    (void)signal(SIGALRM, alarm_off);  // Install before the child can send.
    if ((pid = fork()) == 0) {
        sleep(5);
        kill(getppid(), SIGALRM);
        return 0;
    }
    if (pid < 0) return 1;
    cout << "Waiting for alarm to go off!" << endl;
    while (!alarm_received) sleep(1);
    cout << "Alarm has gone off" << endl;
    wait(NULL);
    cout << "Done!" << endl;
    return 0;
}

// The program runs, we first get lines 1 - 13 done. Next is 14 - 24 runs and gives the alarm testing.
// and waiting for alarm prompt, since child is created, we wait 5 second to then get kill get pid. 
// then we go to line 25 and go all the way to 29 and then end the program.