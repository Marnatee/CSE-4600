// pipe1.cpp adapted to receive a command from the command line.
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) { // Check if the arguments are provided
        cerr << "Example: ./pipe1a cat pipe1.cpp" << endl; // Expecting at least one command and its arguments
        return 1;
    }
    char command[1024] = ""; // Creates an empty string
    for (int i = 1; i < argc; i++) {
        // Supplied size check; only trusted short local commands are used
        if (strlen(command) + strlen(argv[i]) + 2 >= sizeof(command)) return 1;
        strcat(command, " "); // Add a space before appending the next argument
        strcat(command, argv[i]); // Append argv[i] to command
    }
    FILE *fpi; // for reading a pipe
    char buffer[BUFSIZ + 1]; 
    int chars_read; // store the number of characters read from the pipe
    memset(buffer, 0, sizeof(buffer)); // Clear the buffer

    fpi = popen(command, "r");  // Open command with popen in read mode
    if (fpi != NULL) {
        chars_read = fread(buffer, sizeof(char), BUFSIZ, fpi); // Read up to BUFSIZ bytes from fpi into buffer
        if (chars_read > 0) cout << "Output from pipe: " << buffer << endl; // If we read any data, print it to stdout
        int status = pclose(fpi); // Close the pipe and get the exit status
        return status == 0 ? 0 : 1; 
    }
    return 1;
}
