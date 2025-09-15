#include <stdio.h>
#include <math.h>

#include "robot/log.h"

double dsin(double x) { return -cos(x); }

double f1(double x) { return 1 / x; }
double df1(double x) { return log(fabs(x)); }

double f2(double x) { return 10 / (1 + 25 * x * x); }
double df2(double x) { return 2 * atan(5 * x); }

void integral(double (*f)(double), double (*df)(double), double _a, double b, unsigned n) {
    double doubt = 0, h = (b - _a) / (n - 1.0);
    for (unsigned i = 0; i < n - 1; i++) {
        double a = _a + h * i;
        double b = a + h;
        double c = (b - a) / 2.0 / sqrt(5.0);
        doubt += (b - a) / 12.0 * (f(a) + 5 * (f((a + b) / 2 - c) + f((a + b) / 2 + c)) + f(b));
    }

    double exact = df(b) - df(_a);
    double error = exact < 2.23e-16 ? fabs(doubt) : fabs(1 - doubt / exact);

    lprintl("%16.10lg | %16.10lg | %16.10le\n", doubt, exact, error);
}

int main() {
    unsigned n; double a, b;
    scanf("%u%lf%lf", &n, &a, &b);
    if (n < 2 || n > 1000000) return 1;

    lopen("composite_quadrature");
    lprintl("-------------------------------------------------------------------------\n");
    lprintl(" N = %u   [%lg; %lg]\n", n, a, b);
    lprintl("----------------|------------------|------------------|------------------\n");
    lprintl(" F              | Doubt (Approx)   | Exact            | Error\n");
    lprintl("----------------|------------------|------------------|------------------\n");
    lprintl("           sin  | "); integral(sin, dsin, a, b, n);
    if (a > 0 || b < 0) { lprintl("           1/x  | "); integral(f1, df1, a, b, n); }
    if (a < 0 && b > 0) { lprintl(" 10/(1+25*x^2)  | "); integral(f2, df2, a, b, n); }
    lprintl("----------------|------------------|------------------|------------------\n");
    lclosel();
    return 0;
}
