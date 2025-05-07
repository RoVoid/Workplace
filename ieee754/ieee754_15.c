#include <stdio.h>
#include <math.h>

float _exp(float x) {
    float sum = 1.0f, xi, eps = 1.e-07; unsigned i = 2;
    xi = fabsf(x);
    do {
        sum += fabsf(x) * xi;
        xi *= x / i++;
    } while (fabsf(xi) > eps);
    return x >= 0 ? sum : 1 / sum;
}

int main() {
    FILE* log = fopen("log.txt", "w");
    float x[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 40, 50, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -40, -50, -65 }, e, e_real;
    fprintf(log, "X   | Doubt Exp       | Normal Exp      | Absolute        | Relative\n");
    fprintf(log, "----|-----------------|-----------------|-----------------|-----------------\n");
    for (unsigned i = 0; i < sizeof(x) / sizeof(float); i++) {
        e = _exp(x[i]);
        e_real = expf(x[i]);

        float abs_error = fabsf(e_real - e);
        float real_error = abs_error / fabsf(e_real);

        printf("%g: %e\n", x[i], e);
        fprintf(log, "%-3g | %-15e | %-15e | %-15e | %-15e\n", x[i], e, e_real, abs_error, real_error);
    }
    return 0;
}
