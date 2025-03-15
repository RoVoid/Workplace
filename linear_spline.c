#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "robot/log.h"

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
    Point points[100000];

    unsigned N[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 17, 33, 65, 101 };
    unsigned n = sizeof(N) / sizeof(unsigned);
    double A = 0, b = 1, h, x;
    double real, doubt, grade;

    scanf("%lg", &x);

    lopen("linear_spline");
    lprintl("----|--------------|---------------\n");
    lprintl(" X:   %12lg\n", x);
    lprintl("----|--------------|---------------\n");
    lprintl(" N  | Error        | Grade\n");
    lprintl("----|--------------|---------------\n");

    for (unsigned i = 0; i < n; i++) {
        double a = A;
        h = (b - a) / N[i];
        grade = 1.0;

        for (unsigned j = 0; j < N[i] - 1; j++) {
            points[j] = (Point){ a, sin(a) };
            a += h;
            grade /= 2.0 * (j + 1);
        }
        points[N[i] - 1] = (Point){ b, sin(b) };
        grade /= 2.0 * N[i];

        real = sin(x);
        doubt = linearSpline(points, x, N[i]);

        lprintl("%3u | %12lg | %12lg\n", N[i], fabs(real - doubt), grade);
    }

    lclosel();
    return 0;
}
