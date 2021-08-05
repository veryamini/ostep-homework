/**
 * The pid parameter specifies the set of child processes for which to wait for in waitpid().
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    int status;
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        // int rc_wait = wait(NULL);
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        for(int i = 0; i < 20; i++) {
            printf("%d\n", i);
        }
    } else {
        // parent goes down this path (original process)
        int rc_wait = waitpid(rc, &status, 0);
        printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
    }
    return 0;
}