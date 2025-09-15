#include <stdio.h>
#include <math.h>

#define PI 3.14159265359

float _Cos(float x) {
    float sum = 1.0f, xi = 1.0f, eps = 1.e-07; unsigned i = 1;
    x = fabsf(x);
    while (fabsf(xi) > eps) {
        xi *= -x * x / ((2 * i - 1) * (2 * i));
        sum += xi;
        i++;
    }
    return sum;
}

int main() {
    FILE* log = fopen("log0-.txt", "w");
    float x[] = { 1,2,3,4,5,6,7,8,9, 10, 40, 50, -1,-2,-3,-4,-5,-6,-7,-8,-9, -10, -40, -50, -65 }, result, result_real, abs_error, real_error;

    fprintf(log, "X   | Doubt Cos       | Normal Cos      | Absolute        | Relative\n");
    fprintf(log, "----|-----------------|-----------------|-----------------|-----------------\n");

    for (unsigned i = 0; i < sizeof(x) / sizeof(float); i++) {
        result = _Cos(x[i]);
        result_real = cosf(x[i]);

        abs_error = fabsf(result_real - result);
        real_error = abs_error / fabsf(result_real);

        printf("%3g: %e\n", x[i], result);
        fprintf(log, "%-3g | %-15e | %-15e | %-15e | %-15e\n", x[i], result, result_real, abs_error, real_error);
    }

    fclose(log);
    return 0;
}
