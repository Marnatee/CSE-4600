// Based on test_exec.cpp in the homework handout.
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
    // YOUR CODE HERE: use execlp and echo to print:
    // Hello from CSUSB CSE 4600 Section 01 Fall 2026
	//execlp is replacing this current program with another
	//1st echo is program being searched for
	//2nd echo is the new name of the program
	//"Hello..." is the command line argument passed to ls
	//NULL is the closing argument
	execlp("echo", "echo", "Hello from CSUSB CSE 4600 Section 01 Fall 2026", (char *)NULL);
	
	//echo should be running right now instead of this current program
    cerr << "execlp did not run successfully." << endl;
    return 1;
}
