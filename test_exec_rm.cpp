// Based on test_exec.cpp in the homework handout.
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
    // Create a DISPOSABLE test.txt in this homework folder before running.
    // YOUR CODE HERE: use execlp and rm to remove only test.txt.
    // Do not change the filename or accept another path from the user.
	//execlp is replacing this current program with another
	//1st rm is program being searched for
	//2nd rm is the new name of the program
	//test.txt is the command line argument passed to ls
	//NULL is the closing argument
	execlp("rm", "rm", "test.txt", (char *)NULL);
	
	//program rm should be running instead right now.
    cerr << "execlp did not run successfully." << endl;
    return 1;
}
