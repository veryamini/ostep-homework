/**
 * A pipe is a system call that creates a unidirectional communication link between two file descriptors
 * Reference: https://tldp.org/LDP/lpg/node11.html, https://www.geeksforgeeks.org/pipe-system-call/
 * 
 * int pipe(int pipefd[2]);
 * The pipe() function creates a unidirectional data channel for inter-process communication. You pass in an int (Integer) type array pipefd consisting of 2 array element to the function pipe().
 * Then the pipe() function creates two file descriptors in the pipefd array.
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    int pipefd[2];
    int pipe_id = pipe(pipefd);
    if (fork() == 0) {
        close(STDOUT_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        for(int i=0; i < 10; i++) {
            printf("hello");
        }
        return 0;
    }
    else if (0 == fork()) {
        close(STDIN_FILENO);
        dup2(pipefd[0], STDIN_FILENO); /* redirect standard input to pipefd read end */
        close(pipefd[0]);
        close(pipefd[1]);
        char buffer[100];
        
        int result = scanf("%s", buffer);        
        printf("Read buffer: %s\n", buffer);
        return 0;
    }
    close(pipefd[0]);
    close(pipefd[1]);
    int status;
    printf("Waiting on process 1\n");
    wait(&status);
    printf("Waiting on process 2\n");
    wait(&status);
    printf("All children exited\n");
    return 0;
}