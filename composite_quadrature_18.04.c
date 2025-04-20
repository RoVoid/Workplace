#include <stdio.h>
#include <math.h>

#include "robot/log.h"

typedef struct {
    double (*f)(double);
    double (*df)(double);
} Function;

double dsin(double x) { return -cos(x); }
Function Sin = { sin, dsin };

double f1(double x) { return 1.0 / x; }
double df1(double x) { return log(fabs(x)); }
Function F1 = { f1, df1 };

double f2(double x) { return 10.0 / (1.0 + 25.0 * x * x); }
double df2(double x) { return 2.0 * atan(5.0 * x); }
Function F2 = { f2, df2 };

typedef struct {
    double (*f)(double (*fin)(double), double, double);
    int s;
} Quadrature;

double markov(double (*f)(double), double a, double b) {
    double c = (b - a) / (2.0 * sqrt(5.0));
    double mid = (a + b) / 2.0;
    return (b - a) / 12.0 * (f(a) + 5.0 * (f(mid - c) + f(mid + c)) + f(b));
}

Quadrature Markov = { markov, 7 };

void integral(Function func, Quadrature quad, double _a, double b, double eps) {
    double doubt = 0.0, i1, i2, a = _a, s = pow(2.0, quad.s - 1) - 1.0, delta;
    double h = (b - a) / 10.0, qarant = 0.0, srend = 0.0, chi;
    int k = 0;
    while (a < b) {
        if (a + h > b) h = b - a;
        i1 = quad.f(func.f, a, a + h);
        i2 = quad.f(func.f, a, a + h / 2.0) + quad.f(func.f, a + h / 2.0, a + h);
        delta = fabs((i2 - i1) / s);

        if (delta <= eps) {
            a += h;
            doubt += i2;
            qarant += delta;
            srend += i2 >= i1 ? delta : -delta;
            k++;
        }
        //else {
            chi = pow(delta / eps, 1.0 / quad.s);
            if (chi > 10.0) chi = 10.0;
            else if (chi < 0.1) chi = 0.1;
            h = h / chi * 0.95;
        //}
    }

    double exact = func.df(b) - func.df(_a);
    double error = (fabs(exact) < 2.23e-16) ? fabs(doubt) : fabs(1.0 - doubt / exact);

    lprintl("%16.10lg | %16.10lg | %16.10le | %16.10le | %16.10le | %d\n", doubt, exact, error, fabs(srend), qarant, k);
}

void code(double eps, double a, double b) {
    if (eps < 2.23e-16) return;
    lprintl(" Eps = %lg   [%lg; %lg]\n", eps, a, b);
    lprintl("-------------------------------------------------------------------------------------------------------------------------\n");
    lprintl(" Function          | Doubt (Approx)   | Exact            | Error            | Srend             | Qarant           | K\n");
    lprintl("-------------------------------------------------------------------------------------------------------------------------\n");

    lprintl(" sin(x)            | ");
    integral(Sin, Markov, a, b, eps);

    if (a > 0.0 || b < 0.0) {
        lprintl(" 1/x               | ");
        integral(F1, Markov, a, b, eps);
    }

    if (a < 0.0 && b > 0.0) {
        lprintl(" 10/(1+25x^2)      | ");
        integral(F2, Markov, a, b, eps);
    }

    lprintl("-------------------------------------------------------------------------------------------------------------------------\n");
}

int main() {
    double a, b, eps;
    scanf("%lf%lf%lg", &a, &b, &eps);
    if (eps < 2.23e-16) return 1;

    lopen("composite_quadrature_runqe");
    lprintl("-------------------------------------------------------------------------------------------------------------------------\n");
    code(eps, a, b);
    code(1e-7, a, b);
    code(1e-9, a, b);
    code(1e-11, a, b);
    lclosel();
    return 0;
}
