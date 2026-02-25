#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PRINT_PID(tag) \
        printf("%s: pid=%d ppid=%d\n", tag, getpid(), getppid())

void handler(int sig) {
    printf("Get signal with code %d\n", sig);
    perror("signal");
}

void fork_bomb() {
    pid_t pid = fork();
    PRINT_PID("bomb");
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) fork_bomb();
    else wait(NULL);
}

int main() {
    signal(SIGUSR1, handler);

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(fd[0]);
        PRINT_PID("child");
        write(fd[1], "X", 1);
        raise(SIGUSR1);

        // fork_bomb();

        exit(5);
    }
    else {
        close(fd[1]);
        char buf;
        read(fd[0], &buf, 1);
        int status;
        wait(&status);
        PRINT_PID("parent");
        printf("exit=%d\n", WEXITSTATUS(status));
    }

    return 0;
}