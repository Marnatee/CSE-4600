// Based on test_exec.cpp in the homework handout.
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
	
	//endl flushes the output buffer
    cout << "Listing files with execlp" << endl;
    // YOUR CODE HERE: use execlp to run ls -l.
	//execlp is replacing this current program with another
	//1st ls is program being searched for
	//2nd ls is the new name of the program
	//-1 is the command line argument passed to ls
	//NULL is the closing argument
	execlp("ls", "ls", "-1", (char *)NULL);
	
	//ls -1 should be running now instead of this program.
    cerr << "execlp did not run successfully." << endl;
    return 1;  // A successful execlp never reaches this line.
}
