#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    while (getchar() != 'x') printf(rand() % 2 ? "Coin\n" : "Bird\n");
}