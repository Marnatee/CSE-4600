// Complete example: explain the messages; do not rewrite the algorithm.
//test_fork.cpp
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main()
{
  pid_t pid;		//process id
  const char *message;
  int n;
  cout << "fork program starting" << endl;
  pid = fork(); // Fork cretes the child process
  switch (pid) {
  case -1:
    cout << "Fork failure!\n"; //fork ffailed and no child was produced
    return 1;
  case 0:
    // pid is 0 in the child process.
    // The child will print its message 5 times.
   message = "This is the child\n";
    n = 5;
    break;
  default:
    // pid is greater than 0 in the parent process.
    // The child will print its message 3 times.
    message = "This is the parent\n";
    n = 3;
    break;
  }
  for (int i = 0; i < n; ++i) { // The procces each print out n times and they 
                                //pause one second between each line
    cout << message;
    sleep (1);
  }

  // Only the parent waits for the child to finish.
  // The child skips this because its pid value from fork() is 0.
  if (pid > 0) wait(NULL);
  return 0; 
}
