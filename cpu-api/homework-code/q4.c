// https://stackoverflow.com/a/20823413

/**
 * execl(), execle(), execlp(), execv(), execvp(), and execvpe()
 * All variations of exec essentially do the same thing: loading a new program into the current process,
 * and provide it with arguments and environment variables. The differences are in how the program is found,
 * how the arguments are specified, and where the environment comes from.
 * 
 * - The calls with v in the name take an array parameter to specify the argv[] array of the new program.
 *   The end of the arguments is indicated by an array element containing NULL.
 * - The calls with l in the name take the arguments of the new program as a variable-length argument list to the function itself.
 *   The end of the arguments is indicated by a (char *)NULL argument. You should always include the type cast, because NULL is allowed to be an integer constant,
 *   and default argument conversions when calling a variadic function won't convert that to a pointer.
 * - The calls with e in the name take an extra argument (or arguments in the l case) to provide the environment of the new program; otherwise,
 *   the program inherits the current process's environment. This is provided in the same way as the argv array: an array for execve(), separate arguments for execle().
 * - The calls with p in the name search the PATH environment variable to find the program if it doesn't have a directory in it (i.e. it doesn't contain a / character).
 *   Otherwise, the program name is always treated as a path to the executable.
 * **/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child: redirect standard output to a file
        close(STDOUT_FILENO);
        open("./q4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

        // now exec "ls"...
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        char *myargs[3];
        myargs[0] = strdup("/bin/ls");   // program: "ls" (list directories/files)
        myargs[1] = strdup("./");  // argument: folder to list
        myargs[2] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs ls
        printf("this shouldn't print out");
    } else {
        // parent goes down this path (main)
        int rc_wait = wait(NULL);
        printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n",
                rc, rc_wait, (int) getpid());
    }
    return 0;
}