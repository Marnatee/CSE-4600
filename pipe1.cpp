//pipe1.cpp
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
  FILE *fpi;				// for reading a pipe
 
  char buffer[BUFSIZ+1];		// BUFSIZ defined in <stdio.h>
  
  int chars_read;
  memset (buffer, 0,sizeof(buffer));	// clear buffer
  fpi = popen ("cat pipe1.cpp", "r");	// read the source file through a command pipe
  if (fpi != NULL) {
    // read data from pipe into buffer
    chars_read = fread(buffer, sizeof(char), BUFSIZ, fpi);  // read up to BUFSIZ bytes from fpi into buffer
    if (chars_read > 0) // if we read any data, print it to stdout
     cout << "Output from pipe: " << buffer << endl;
    pclose (fpi); 			// close the pipe
    return 0; 
  }

  return 1;
}
