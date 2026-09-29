#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void child_process(void) {
    // Seed with pid too, so the two children don't get identical random numbers
    srandom(time(NULL) ^ getpid());

    int iterations = random() % 30 + 1;   // 1 to 30

    for (int i = 0; i < iterations; i++) {
        printf("Child Pid: %d is going to sleep!\n", getpid());
        sleep(random() % 10 + 1);          // 1 to 10 seconds
        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n",
               getpid(), getppid());
    }
    exit(0);
}

int main(void) {
    pid_t pid;
    int status;

    for (int i = 0; i < 2; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) {
            child_process();               // never returns
        }
    }

    // Parent: wait for both children
    for (int i = 0; i < 2; i++) {
        pid = wait(&status);
        printf("Child Pid: %d has completed\n", pid);
    }
    return 0;
}