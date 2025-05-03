#include <stdio.h>
#include <math.h>

double f14(double x) {
    return sin(x * x) + 1;
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

double alpha, eps = 1e-15;

double integral(double (*f)(double), Quadrature qr, double a, double b) {
    double scale = pow(2.0, qr.s - 1) - 1.0;
    double sum = 0, h = 0.1;

    while (a < b) {
        if (a + h > b) h = b - a;
        double i1 = qr.rule(f, a, a + h);
        double i2 = qr.rule(f, a, a + h / 2) + qr.rule(f, a + h / 2, a + h);
        double err = fabs((i2 - i1) / scale);
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

Interval findInterval(double (*f)(double), Quadrature qr, double a) {
    double s = pow(2.0, qr.s - 1) - 1.0;
    double sum = 0, h = 0.1;

    while (sum < alpha - eps) {
        double i1 = qr.rule(f, a, a + h);
        double i2 = qr.rule(f, a, a + h / 2) + qr.rule(f, a + h / 2, a + h);
        double err = fabs((i2 - i1) / s);
        if (sum + i2 >= alpha - eps) return (Interval) { a, a + h };
        if (err <= eps) {
            sum += i2;
            a += h;
        }
        double factor = pow(err / eps, 1.0 / qr.s);
        if (factor > 10.0) factor = 10.0;
        else if (factor < 0.1) factor = 0.1;
        h = h / factor * 0.95;
    }

    return (Interval) { a, a + h };
}

int sign(double x) {
    return (x > 0) - (x < 0);
}

double methodChord(double (*f)(double), double a, double b) {
    double fa = f(a), fb = f(b);
    if (fabs(fa) < eps) return a;
    if (fabs(fb) < eps) return b;

    int si = sign(fa);
    if (si == sign(fb)) return -1;

    double c[2];
    int i = 0;
    c[0] = a;
    c[1] = b;

    while (fabs(c[1] - c[0]) > eps) {
        c[i] = (a * fb - b * fa) / (fb - fa);
        double fc = f(c[i]);
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

double equation(double x) {
    return integral(f14, Markov, 0, x) - alpha;
}

int main() {
    scanf("%lf", &alpha);

    Interval interval = findInterval(f14, Markov, 0);
    double center = (interval.start + interval.end) / 2.0;
    double d = (interval.end - interval.start) * 2.0;

    double x = methodChord(equation, center - d, center + d);

    if (x == -1) printf("No found");
    else printf("x ≈ %.15lf\n", x);

    return 0;
}
