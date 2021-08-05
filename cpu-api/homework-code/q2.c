// When both the child and parent process are writing to the same file, any one of them writes to the file first the next process follows that

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int fd = open("q2_output.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    printf("Parent function before fork (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        assert(fd > -1);
        int can_child_write = write(fd, "hello\n", 5);
        assert(can_child_write == 5);
        printf("Able to successfully read the file in child process\n");
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int) getpid());
        assert(fd > -1);
        int can_parent_write = write(fd, "goodbye\n", 7);
        assert(can_parent_write == 7);
        printf("Able to successfully read the file in child process\n");
        // wait(NULL);
        close(fd);
    }
    return 0;
}