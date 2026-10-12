#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\033[?1049h"); // включить alt screen

    for (int i = 0; i < 5; i++) {
        printf("\033[H"); // курсор в начало
        printf("Frame: %d\n", i);
        fflush(stdout);
        sleep(1);
    }

    printf("\033[?1049l"); // вернуть обычный экран
    return 0;
}