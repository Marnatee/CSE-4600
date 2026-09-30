// pipe4.cpp from the handout; complete the same parent/child sections.
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Missing input string. Please provide the input string when running the executable" << std::endl;
        return 1; // Exit with error code 1 if the input string is missing
    }
    const char *toConcatenate = "! Welcome to CSE 4600 Operating Systems course"; 
    int fd[2];
    if (pipe(fd) == -1) { // Create a pipe and check for errors
        std::cerr << "Pipe failed" << std::endl; 
        return 1;
    }
    pid_t pid = fork();  // Create a child process using fork and store the result in pid
    if (pid == -1) return 1;
    if (pid == 0) {
        char buffer[256] = ""; // Create a buffer to store the string read from the pipe
        close(fd[1]); // Close the write end
        read(fd[0], buffer, sizeof(buffer)); // Read the name *
        close(fd[0]); // Close the read end
        std::cout << buffer << toConcatenate << std::endl; // Print buffer followed by toConcatenate
    } else {
        close(fd[0]); // Close the read end
        write(fd[1], argv[1], strlen(argv[1]) + 1); // Write argv[1] to the pipe
        close(fd[1]); // Close the write end
        wait(NULL); // Wait for the child process to complete
    }
    return 0;
}
