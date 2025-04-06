#include <stdio.h>
#include <math.h>

#include "robot/log.h"

double f0(double x) { return 1; }
double f1(double x) { return x; }
double f2(double x) { return pow(x, 2); }
double f3(double x) { return pow(x, 3); }
double f4(double x) { return pow(x, 4); }

double df0(double x) { return x; }
double df1(double x) { return pow(x, 2) / 2; }
double df2(double x) { return pow(x, 3) / 3; }
double df3(double x) { return pow(x, 4) / 4; }
double df4(double x) { return pow(x, 5) / 5; }

void integral(double (*f)(double), double (*df)(double), double a, double b) {
    double doubt = ((b - a) / 8.0) * (f(a) + 3 * f((2 * a + b) / 3) + 3 * f((a + 2 * b) / 3) + f(b));
    double exact = df(b) - df(a);
    double error = fabs(1 - doubt / exact);
    lprintl("%12.10lg | %12.10lg | %12.10le\n", doubt, exact, error);
}

int main() {
    double a = 0.0, b = 1.0;
    lopen("kotes");
    lprintl("-------------------------------------------------------\n");
    lprintl(" S = 4   M = 2   A = %lg   B = %lg\n", a, b);
    lprintl("------|--------------|--------------|------------------\n");
    lprintl(" F    | Doubt        | Exact        | Error\n");
    lprintl("------|--------------|--------------|------------------\n");
    lprintl(" 1    | "); integral(f0, df0, a, b);
    lprintl(" x    | "); integral(f1, df1, a, b);
    lprintl(" x^2  | "); integral(f2, df2, a, b);
    lprintl(" x^3  | "); integral(f3, df3, a, b);
    lprintl(" x^4  | "); integral(f4, df4, a, b);
    lprintl("------|--------------|--------------|------------------\n");
    return 0;
}
