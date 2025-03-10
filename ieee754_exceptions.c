#include <stdio.h>

void printFloatBits(float* a) {
    unsigned x = *(unsigned*)a;
    for (int i = 31; i >= 0; i--) {
        printf("%u", (x >> i) & 1);
        if (i == 31 || i == 23) printf(" | ");
    }
    printf("\n\n");
}

int main() {
    float zeroP = 1, zeroN = -1, infP, infN, nan;

    while (zeroP > 0)zeroP /= 2;
    printf("%g\n", zeroP); // 0
    printFloatBits(&zeroP);

    while (zeroN < 0)zeroN /= 2;
    printf("%g\n", zeroN); // -0
    printFloatBits(&zeroN);

    infP = 1 / zeroP;
    printf("%g\n", infP); // inf
    printFloatBits(&infP);

    infN = 1 / zeroN;
    printf("%g\n", infN); // -inf
    printFloatBits(&infN);

    nan = zeroP / zeroP;
    printf("%g\n", nan); // -nan(ind)
    printFloatBits(&nan);

    return 0;
}
