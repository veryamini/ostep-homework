/**
 * Refer to q4.c where we have used close(STDOUT_FILENO); in child process and called printf.
 * This redirects all the output in child process to the output file. Another example can be seen below.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child: redirect standard output to a file
        close(STDOUT_FILENO);
        open("./q7.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

        printf("hello, I am child (pid:%d)\n", (int) getpid());
        for(int i = 0; i < 20; i++) {
            printf("%d\n", i);
        }
    } else {
        // parent goes down this path (main)
        int rc_wait = wait(NULL);
        printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n",
                rc, rc_wait, (int) getpid());
    }
    return 0;
}