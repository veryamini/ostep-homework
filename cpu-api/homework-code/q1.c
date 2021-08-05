// Both the processes seem to have their own memory stack which stores value of x individually and updates it individually.
// The initial value 100 is passed to child process when fork is called.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int x = 100;
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d) with (x:%d)\n", (int) getpid(), (int) x);
        x = 200;
        printf("Value of x after changing in child process x:%d\n", x);
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d) with (x:%d)\n",
               rc, (int) getpid(), x);
        x = 300;
        printf("Value of x after changing in parent process x:%d\n", x);
    }
    return 0;
}