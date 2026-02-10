#include <stdio.h>
#include <unistd.h>

int gVar = 0;

int main() {
    int lVar = 10;

    pid_t p;
    p = fork();
    if (p == -1) {
        fprintf(stderr, "Error\n");
        return 1;
    }

    if (p == 0) {
        gVar++;
        lVar++;
        printf("(Child) ");
    }
    else {
        gVar--;
        lVar--;
        printf("(Main) ");
    }

    printf("Pid: %d\n", getpid());
    printf("Global: %d\n", gVar);
    printf("Local: %d\n\n", lVar);

    return 0;
}
