#include <stdio.h>

void printFloatBits(float a) {
    unsigned x = *(unsigned*)&a;
    int e = ((x >> 23) & 0xFF) - 127;
    printf("Exp: %d\n", e);
    for (int i = 31; i >= 0; i--) {
        printf("%u", (x >> i) & 1);
        if (i == 31 || i == 23) printf(" | ");
    }
    printf("\n\n");
}

void printMin() {
    float a = 1.0f, min;
    do {
        min = a;
        a /= 2.0f;
    } while (a > 0.0f);
    printf("Min: %e\n", min);
    printFloatBits(min);
}

void printEpsilon() {
    float epsilon = 1.0f;
    while (1.0f + epsilon > 1.0f) epsilon /= 2.0f;
    epsilon *= 2.0f;
    printf("Epsilon: %e\n", epsilon);
    printFloatBits(epsilon);
}

void printOldStep(float x) { // По книге
    unsigned ux = *(unsigned*)&x;
    ux++;
    float step = *(float*)&ux - x;
    printf("(OLD) Step For %g: %e\n", x, step);
    printFloatBits(step);
}

void printStep(float x) {
    unsigned ux = *(unsigned*)&x;

    unsigned m = ux & 0x7FFFFF;
    unsigned e = (ux >> 23) & 0xFF;

    if (++m & (1 << 23)) {
        m = 0;
        e++;
    }

    ux = (ux & 0x80000000) | (e << 23) | m;
    float step = *(float*)&ux - x;
    printf("Step For %g: %e\n", x, step);
    printFloatBits(step);
}

/*
    x = m * 2^e, 1 <= m < 2
    eps(x) = 2^(e-23)
    eps = 2^(-23)

    eps(x) = 2^(e-23) <= m * 2^(e-23) = x * eps
    x * eps / 2 = m * 2^(e-24) < eps(x)
*/

int main() {
    float x;
    scanf("%g", &x);
    printMin();
    printEpsilon();
    printOldStep(x);
    printStep(x); // 16777216 for 1.8
    return 0;
}
