#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "robot/log.h"

#define Nx 100000

typedef struct Point {
    double x, f;
} Point;

double linearSpline(Point* points, double x, unsigned n) {
    double t;
    for (unsigned i = 0; i < n - 1; i++) {
        if (x >= points[i].x && x <= points[i + 1].x) {
            t = (x - points[i].x) / (points[i + 1].x - points[i].x);
            return points[i].f + t * (points[i + 1].f - points[i].f);
        }
    }
    return NAN;
}

int main() {
    srand(time(NULL));

    Point points[200];

    unsigned N[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 17, 33, 65, 101 };
    unsigned n = sizeof(N) / sizeof(unsigned);
    double A = 0, b = 1, h;
    double real, doubt, grade, max_error;

    double X[Nx];

    for (unsigned i = 0; i < Nx; i++) X[i] = fmin(fmax(rand() * (b - A) / RAND_MAX - A, A), b);

    lopen("linear_spline");
    lprintl("----|--------------|---------------\n");
    lprintl(" N  | Norm         | Grade\n");
    lprintl("----|--------------|---------------\n");

    for (unsigned i = 0; i < n; i++) {
        double a = A;
        h = (b - a) / N[i];

        for (unsigned j = 0; j < N[i] - 1; j++) {
            points[j] = (Point){ a, sin(a) };
            a += h;
        }
        points[N[i] - 1] = (Point){ b, sin(b) };
        grade = pow(h, 2); // пересмотрел оценку

        max_error = 0;
        for (unsigned j = 0; j < Nx; j++) {
            real = sin(X[j]);
            doubt = linearSpline(points, X[j], N[i]);
            if (max_error < fabs(real - doubt)) max_error = fabs(real - doubt); // норма
        }

        lprintl("%3u | %12lg | %12lg\n", N[i], max_error, grade);
    }

    lclosel();
    return 0;
}
