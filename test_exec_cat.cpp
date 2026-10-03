// Based on test_exec.cpp in the homework handout.
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
    // YOUR CODE HERE: use execlp and cat to display test_exec_cat.cpp.
	//execlp is replacing this current program with another
	//1st cat is program being searched for
	//2nd cat is the new name of the program
	//test_exec_cat.cpp is the command line argument passed to ls
	//NULL is the closing argument
	//This will print the contents of this current program
	execlp("cat","cat","test_exec_cat.cpp",(char *)NULL);
	
    cerr << "execlp did not run successfully." << endl;
    return 1;
}
