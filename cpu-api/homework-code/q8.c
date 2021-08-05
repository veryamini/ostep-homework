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
#include <errno.h>

int main(int argc, char* argv[]) {
    int pipefd[2];
    int pipe_id = pipe(pipefd);

    if(fork() == 0) {
        // we can close pipefd[0] here as it is open in the 2nd child process or in the parent prcess.
        // At any point of time we will have at least 1 fd[0] and fd[2] open hence we will not get SIG error
        close(pipefd[0]);
        close(STDOUT_FILENO);
        /**
         * dup2 copies all the data and offset details from fd1 to fd2 in dup2(fd1, fd2);
         * This creates a new fd at fd2 index in the file descriptor arrays.
         * We can close fd1 as we won't need to use this at all.
         * Also when a process exits, it closes all the existing open fds in that process.
         * We need to set new fd at STDOUT_FILENO for this process as it outputs everything from this processa t the new fd
         * fd[1] is generally the output for a process, and input for a pipe
         * */
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        // printf is not a blocking call so this process ends seemlessly
        for (int i = 0; i < 10; i++)
            printf("hello");
        return 0;
    }
    else if(fork() == 0) {
        close(pipefd[1]);
        close(STDIN_FILENO);
        // fd[0] is generally the input for a process, and output for a pipe
        dup2(pipefd[0], STDIN_FILENO); /* redirect standard input to pipefd read end */
        close(pipefd[0]);
        char buffer[100];
        scanf("%s", buffer);
        printf("Read from pipe: %s", buffer);
        return 0;
    }
    /**
     * Need to close all pipefds in all processes.
     * 1. If we don't close it in parent, in one of the child processes we use scanf, which is a blocking call.
     * 2. Scanf needs a whitespace or new line or end of file character, which is returned when a pipe is widowed (one of the ends is closed in all processes.)
     * 3. So when the 2nd child process is suspended and the parent runs, if we close the pipefd we generate the end of file character on read, writing on such
     *    a pipe causes the writing process to receive a SIGPIPE signal.
     * */
    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);
    return 0;
}