#include <stdio.h>
#include <math.h>

#define PI 3.14159265359

float _Cos(float x) {
    float sum = 1.0f, xi = 1.0f, eps = 1.e-07; unsigned i = 1;
    x = fabsf(x);
    x -= (unsigned)(x / (2 * PI)) * (2 * PI);
    while (fabsf(xi) > eps) {
        xi *= -x * x / ((2 * i - 1) * (2 * i));
        sum += xi;
        i++;
    }
    return sum;
}

float _Sin(float x) {
    float sum = 0.0f, xi, eps = 1.e-07; unsigned i = 1;
    x -= (int)(x / (2 * PI)) * (2 * PI);
    xi = x;
    while (fabsf(xi) > eps) {
        sum += xi;
        xi *= -x * x / ((2 * i) * (2 * i + 1));
        i++;
    }
    return sum;
}

typedef struct {
    float first, second;
} pair;

int signf(float a) {
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}

int kvadr(float b, float c, pair* x) {
    float d = b * b - 4 * c;
    if (d < 0) return 0;

    x->first = -(b + signf(b) * sqrtf(d)) / 2;
    x->second = x->first ? c / x->first : 0;
    return 1;
}

int kvadr1(float b, float c, pair* x) {
    float d = b * b - 4 * c;
    if (d < 0) return 0;

    x->first = (-b + sqrtf(d)) / 2;
    x->second = c / x->first;
    return 1;
}

void task0() {
    FILE* log = fopen("log0.txt", "w");
    float x[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 40, 50, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -40, -50, -65 }, result, result_real, abs_error, real_error;

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
}

void task1() {
    FILE* log = fopen("log1.txt", "w");
    pair x[] = { {2, 3}, {1e7f, 1}, {-1, -1}, {-2, -1e7f}, {-263, 500}, {-1, 0}, {2 * -1e7f, 4} }, result, result_real, abs_error, real_error;

    fprintf(log, " Doubt X      | Normal X     | Absolute     | Relative\n");
    fprintf(log, "--------------|--------------|--------------|--------------\n");

    for (unsigned i = 0; i < sizeof(x) / sizeof(pair); i++) {
        result_real = x[i];
        if (!kvadr(-result_real.first - result_real.second, result_real.first * result_real.second, &result)) {
            fprintf(log, "              | %12.5e |              |\n", result_real.first);
            fprintf(log, "              | %12.5e |              |\n", result_real.second);
            fprintf(log, "--------------|--------------|--------------|--------------\n");
            continue;
        }

        abs_error = (pair){ fabsf(result_real.first - result.first), fabsf(result_real.second - result.second) };
        real_error = (pair){ fabsf(1 - result.first / result_real.first), fabsf(1 - result.second / result_real.second) };

        printf("%6g %6g: %10.3e %10.3e\n", x[i].first, x[i].second, result.first, result.second);
        fprintf(log, " %12.5e | %12.5e | %12.5e | %12.5e\n", result.first, result_real.first, abs_error.first, real_error.first);
        fprintf(log, " %12.5e | %12.5e | %12.5e | %12.5e\n", result.second, result_real.second, abs_error.second, real_error.second);
        fprintf(log, "--------------|--------------|--------------|--------------\n");
    }

    fclose(log);
}

int main() {
    task0();
    printf("\n");
    task1();
    return 0;
}
