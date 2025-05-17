#include <stdio.h>
#include <math.h>

// #include "robot/log.h"

double function0(double x) {
    return sin(x);
}

double function1(double x) {
    return x - pow(log(x + 1), 2);
}

double function2(double x) {
    return (x - 2) * (x - 2) + 1;
}

double epsilon = 1e-11;

double min_half(double (*f)(double), double a, double b, unsigned* k) {
    double x1, x2 = (a + b) / 2, x3;
    double f1, f2 = f(x2), f3;

    while (fabs(b - a) > epsilon * fabs(a + b) / 2 && fabs(b - a) > epsilon) {
        (*k)++;

        x1 = (a + x2) / 2; x3 = (x2 + b) / 2;
        f1 = f(x1); f3 = f(x3);

        if (f1 < f2 && f1 < f3) { b = x2; x2 = x1; f2 = f1; }
        else if (f2 < f3) { a = x1; b = x3; }
        else { a = x2; x2 = x3; f2 = f3; }
    }

    return x2;
}

int main() {
    double a, b, x;
    unsigned k = 0;

    scanf("%lf%lf", &a, &b);

    x = min_half(function2, a, b, &k);

    printf("X:    %15.10lf\nf(X): %15.10lf\nK: %u\n", x, function2(x), k);

    return 0;
}