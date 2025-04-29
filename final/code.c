#include <stdio.h>
#include <math.h>

double f14(double u) {
    return sin(u * u) + 1;
}

typedef struct {
    double (*f)(double (*)(double), double, double);
    int s;
} Quadrature;

double markov(double (*f)(double), double a, double b) {
    double c = (b - a) / (2.0 * sqrt(5.0));
    double mid = (a + b) / 2.0;
    return (b - a) / 12.0 * (f(a) + 5.0 * (f(mid - c) + f(mid + c)) + f(b));
}

Quadrature Markov = { markov, 7 };

void integral(double (*func)(double), Quadrature quad, double _a, double b, double eps) {
    double doubt = 0.0, i1, i2, a = _a, s = pow(2.0, quad.s - 1) - 1.0, delta;
    double h = (b - a) / 10.0, qarant = 0.0, srend = 0.0, chi;
    while (a < b) {
        if (a + h > b) h = b - a;
        i1 = quad.f(func, a, a + h);
        i2 = quad.f(func, a, a + h / 2.0) + quad.f(func, a + h / 2.0, a + h);
        delta = fabs((i2 - i1) / s);

        if (delta <= eps) {
            a += h;
            doubt += i2;
        }
        chi = pow(delta / eps, 1.0 / quad.s);
        if (chi > 10.0) chi = 10.0;
        else if (chi < 0.1) chi = 0.1;
        h = h / chi * 0.95;
    }
}

double chordsMethod(double (*func)(double), double a, double b, double epsilon) {
    double fa, fb, fc, c[2];
    int i, aSign, k;
    c[0] = a; c[1] = b; fa = func(a); fb = func(b);

    if (fabs(fa) < epsilon) { return a; }
    if (fabs(fb) < epsilon) { return b; }

    aSign = sign(fa);
    if (aSign == sign(fb)) return -1;

    for (k = 1, i = 0; fabs(c[1] - c[0]) > epsilon; k++, i = !i) {
        c[i] = (a * fb - b * fa) / (fb - fa);
        fc = func(c[i]);

        if (fabs(fc) < epsilon) return c[i];
        if (aSign == sign(fc)) {
            a = c[i];
            fa = fc;
        }
        else {
            b = c[i];
            fb = fc;
        }
    }

    return c[i];
}

int main() {
    double alpha, eps = 1e-15;
    scanf("%lf", &alpha);

    return 0;
}
