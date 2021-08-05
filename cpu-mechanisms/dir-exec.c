#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sched.h>

int main(int argc, char *argv[]) {
    // int fd = open("q3_output.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    struct timeval tv0;
    struct timeval tv1;
    int TOTAL_ITERATIONS = 100000001;
    char buffer[0];
    double elapsed_time = 0;
    // reading system call time
    // while (TOTAL_ITERATIONS < 100000001) {
    //     elapsed_time = 0;
    //     for (int i = 0; i < TOTAL_ITERATIONS; i++) {
    //         gettimeofday(&tv0, 0);
    //         read(fd, buffer, 0);
    //         gettimeofday(&tv1, 0);
    //         elapsed_time += (tv1.tv_sec-tv0.tv_sec)*1000000 + tv1.tv_usec-tv0.tv_usec;
    //     }
    //     elapsed_time /= TOTAL_ITERATIONS;
    //     printf("Total average time (in micro seconds) elapsed for %d read system calls:  %f\n", TOTAL_ITERATIONS, elapsed_time);
    //     TOTAL_ITERATIONS *= 10;
    // }

    // while (TOTAL_ITERATIONS < 10) { 
    int first_pipefd[2];
    int pipe_id_1 = pipe(first_pipefd);
    int second_pipefd[2];
    int pipe_id_2 = pipe(second_pipefd);
    pid_t pid_A = fork();
    pid_t pid_B = fork();
    char read_buffer[5] = "hello";

    if (pid_A == 0) {
        printf("Starting first process\n");
        // In child one, write to second pipe
        elapsed_time = 0;
        // for (int i = 0; i < TOTAL_ITERATIONS; i++) {
            
        // }
        gettimeofday(&tv0, 0);
        close(first_pipefd[0]);
        long start_time = tv0.tv_sec * 1000000 + tv0.tv_usec;
        write(first_pipefd[1], &start_time, sizeof(long));
        close(first_pipefd[1]);
        long end_time;
        read(second_pipefd[0], &end_time, sizeof(long));
        elapsed_time = end_time - start_time;
        // elapsed_time /= TOTAL_ITERATIONS;
        printf("In first child, start_time: %ld and end_time: %ld\n", start_time, end_time);
        printf("Total average time (in micro seconds) elapsed for context switch for first child process:  %f\n", elapsed_time);
        TOTAL_ITERATIONS += 1;
    }
    else if (pid_B == 0) {
        // In child two, read from second pipe, write to first pipe
        // for (int i = 0; i < TOTAL_ITERATIONS; i++) {
            
        // }
        printf("Starting second process\n");
        gettimeofday(&tv1, 0);
        long end_time = (tv1.tv_sec)*1000000 + tv1.tv_usec;
        close(second_pipefd[0]);
        long start_time;
        read(first_pipefd[0], &start_time, sizeof(long));
        write(second_pipefd[1], &end_time, sizeof(long));
        read(first_pipefd[0], &start_time, 5);
        // elapsed_time /= TOTAL_ITERATIONS;
        elapsed_time = end_time - start_time;
        printf("In second child, start_time: %ld and end_time: %ld\n", start_time, end_time);
        // elapsed_time /= TOTAL_ITERATIONS;
        printf("Total average time (in micro seconds) elapsed for context switch between first child process:  %f\n", elapsed_time);
        TOTAL_ITERATIONS += 1;
    }
    else {
        printf("Came in parent process\n");
        // printf("Total average time (in micro seconds) elapsed in iterations : %d for context swict and executing second child process:  %f\n", TOTAL_ITERATIONS, elapsed_time);
    }
    // }
}