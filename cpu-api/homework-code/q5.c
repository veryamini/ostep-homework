/**
 * If the calling process has no existing unwaited-for child processes, wait will fail and return immediately -1 with [ECHILD] error
 * If there is no child process of the process in which wait system call is made, there is no effect.
 * Otherwise wait system call suspends execution of its calling proces and waits until all the child processes are
 * finished before running the parent process.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
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
        int rc_wait = wait(NULL);
        printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
    }
    return 0;
}