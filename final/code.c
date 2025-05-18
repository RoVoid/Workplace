#include <stdio.h>
#include <math.h>

#define LOG_FOLDER "../logs/"
#include "../robot/log.h"

double f14(double x) {
    return 2 * (sin(x * x) + 1);
}

double f14_result(double x) {
    return x * x;
}

typedef struct {
    double (*rule)(double (*)(double), double, double);
    int s;
} Quadrature;

double markov(double (*f)(double), double a, double b) {
    double c = (b - a) / (2.0 * sqrt(5.0));
    double m = (a + b) / 2.0;
    return (b - a) / 12.0 * (f(a) + 5.0 * (f(m - c) + f(m + c)) + f(b));
}

Quadrature Markov = { markov, 7 };

typedef struct {
    double start, end;
} Interval;

Interval findInterval(double (*f)(double), Quadrature qr, double eps, double alpha, double a, double* sum, double* qarant, double* srend, unsigned* k) {
    double s = pow(2.0, qr.s - 1) - 1.0, h = 0.1;

    *sum = 0;
    while (*sum < alpha - eps) {
        (*k)++;
        double i1 = qr.rule(f, a, a + h);
        double i2 = qr.rule(f, a, a + h / 2) + qr.rule(f, a + h / 2, a + h);
        double err = fabs((i2 - i1) / s);
        if (*sum + i2 >= alpha - eps) break;
        if (err <= eps) {
            *sum += i2;
            a += h;
            *qarant += err;
            *srend += i2 >= i1 ? err : -err;
        }
        double factor = pow(err / eps, 1.0 / qr.s);
        if (factor > 10.0) factor = 10.0;
        else if (factor < 0.1) factor = 0.1;
        h = h / factor * 0.95;
    }

    *srend = fabs(*srend);

    return (Interval) { a, a + h };
}

int sign(double x) {
    return (x > 0) - (x < 0);
}

double integral(double (*f)(double), Quadrature qr, double eps, double a, double b) {
    double s = pow(2.0, qr.s - 1) - 1.0, sum = 0, h = b - a;

    while (a < b) {
        if (a + h > b) h = b - a;
        double i1 = qr.rule(f, a, a + h);
        double i2 = qr.rule(f, a, a + h / 2) + qr.rule(f, a + h / 2, a + h);
        double err = fabs((i2 - i1) / s);
        if (err <= eps) {
            sum += i2;
            a += h;
        }
        double factor = pow(err / eps, 1.0 / qr.s);
        if (factor > 10.0) factor = 10.0;
        else if (factor < 0.1) factor = 0.1;
        h = h / factor * 0.95;
    }

    return sum;
}

double equation(double (*f)(double), Quadrature qr, double eps, double alpha, double x, double sum, double a) {
    return sum + integral(f, qr, eps, a, x) - alpha;
}

double methodChord(double (*f)(double), double (*f_result)(double), Quadrature qr, double eps, double alpha, double a, double b, double sum, unsigned* k) {
    double fa = sum - alpha, fb = equation(f, qr, eps, alpha, b, sum, a);

    int si = sign(fa);

    double c[2];
    int i = 0;
    c[0] = a;
    c[1] = b;

    while (fabs(c[1] - c[0]) > eps) {
        (*k)++;
        c[i] = (a * fb - b * fa) / (fb - fa);
        double fc = equation(f, qr, eps, alpha, c[i], sum, a);
        if (fabs(fc) < eps) return c[i];

        if (sign(fc) == si) {
            a = c[i];
            fa = fc;
        }
        else {
            b = c[i];
            fb = fc;
        }
        i = !i;
    }

    return c[i];
}

void execute(double (*f)(double), double (*f_result)(double), Quadrature qr, double eps, double alpha, double root) {
    lprintl("----------------------------------------------\n");

    lprintl(" Epsilon:  %.0le\n\n", eps);

    double sum;
    double qarant = 0, srend = 0;
    unsigned k = 0;

    Interval interval = findInterval(f, qr, eps, alpha, 0, &sum, &qarant, &srend, &k);

    lprintl(" Qar.:  %14.10lf  Sren.: %14.10lf  (Integral)\n", qarant, srend);
    lprintl(" K: %u  (Integral)\n", k);

    k = 1;
    double x = methodChord(f, f_result, qr, eps, alpha, interval.start, interval.end, sum, &k);

    lprintl(" K: %u  (Method Chord)\n", k);

    lprintl("  -----    -----    -----    -----    -----\n");

    lprintl(" Root:  %14.10lf  Found: ", root);
    if (x == -1) lprintl("-\n Error: -\n");
    else {
        x = f_result(x);
        lprintl("%14.10lf\n Error: %14.10lf\n", x, fabs(x - root));
    }
}

typedef struct {
    double alpha, root;
} Set;

int main() {
    lopen("final");

    Set sets[] = { {7.849678850620078, 10}, {21.16734179985924, 100}, {1.64574, 0.5}, {0.0002, 0.00000001}, {79.35445578244514, 1524} };
    unsigned Nsets = sizeof(sets) / sizeof(Set);

    double epsilons[] = { 1e-12, 1e-13, 1e-14, 1e-15 };
    unsigned Neps = sizeof(epsilons) / sizeof(double);

    for (unsigned i = 0; i < Nsets; i++) {
        Set set = sets[i];
        for (unsigned j = 0; j < Neps; j++) {
            execute(f14, f14_result, Markov, epsilons[j], set.alpha, set.root);
        }
    }

    lprintl("----------------------------------------------\n");
    lclosel();
    return 0;
}
