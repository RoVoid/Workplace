#include <stdio.h>

// Битовое представление числа: (-1)^S * 1.M * 2^E, S - знак, M - мантисса, E - порядок

void printFloatBits(float* a) {
    unsigned x = *(unsigned*)a;
    // 1 бит для S: 0 {+}, 1 {-}
    // 8 битов для E: E + 127, E от 0 до 127
    // 23 бита для M
    for (int i = 31; i >= 0; i--) {
        printf("%u", (x >> i) & 1);
        if (i == 31 || i == 23) printf(" | ");
    }
    printf("\n\n");
}

void printDoubleBits(double* a) {
    unsigned long long x = *(unsigned long long*)a;
    // 1 бит для S: 0 {+}, 1 {-}
    // 11 битов для E: E + 1023, E от 0 до 1023
    // 52 бита для M
    for (int i = 63; i >= 0; i--) {
        printf("%u", (x >> i) & 1);
        if (i == 63 || i == 52) printf(" | ");
    }
    printf("\n\n");
}

// Если биты E все 1, S == 0 и M == 0 => +∞
// Если биты E все 1, S == 1 и M == 0 => -∞
// Если биты E все 1 и M != 0 => NaN 

int main() {
    float f; double d;
    scanf("%f %lf", &f, &d);

    printf("%%f:  %f\n", f);
    printf("%%e:  %e\n", f);
    printf("%%g:  %g\n", f);
    printFloatBits(&f);

    printf("%%lf:  %lf\n", d);
    printf("%%le:  %le\n", d);
    printf("%%lg:  %lg\n", d);
    printDoubleBits(&d);

    return 0;
}
