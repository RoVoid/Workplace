#include <stdio.h>
#include <math.h>

#include "robot/log.h"

double f0(double x) { return 1; }
double df0(double x) { return x; }

double f1(double x) { return x; }
double df1(double x) { return pow(x, 2) / 2.0; }

double f2(double x) { return pow(x, 2); }
double df2(double x) { return pow(x, 3) / 3.0; }

double f3(double x) { return pow(x, 3); }
double df3(double x) { return pow(x, 4) / 4.0; }

double f4(double x) { return pow(x, 4); }
double df4(double x) { return pow(x, 5) / 5.0; }

double f5(double x) { return pow(x, 5); }
double df5(double x) { return pow(x, 6) / 6.0; }

double f6(double x) { return pow(x, 6); }
double df6(double x) { return pow(x, 7) / 7.0; }

double f7(double x) { return pow(x, 7); }
double df7(double x) { return pow(x, 8) / 8.0; }

void integral(double (*f)(double), double (*df)(double), double a, double b) {
    double h = (b - a) / 2.0 / sqrt(5);
    double doubt = (b - a) / 12.0 * (f(a) + 5 * (f((a + b) / 2 - h) + f((a + b) / 2 + h)) + f(b));
    double exact = df(b) - df(a);
    double error = fabs(1 - doubt / exact);
    lprintl("%12.10lg | %12.10lg | %12.10le\n", doubt, exact, error);
}

int main() {
    double a = 0.0, b = 1.0;
    lopen("markov");
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
    lprintl(" x^5  | "); integral(f5, df5, a, b);
    lprintl(" x^6  | "); integral(f6, df6, a, b);
    lprintl(" x^7  | "); integral(f7, df7, a, b);
    lprintl("------|--------------|--------------|------------------\n");
    lclosel();
    return 0;
}
