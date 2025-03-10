#define _USE_MATH_DEFINES // в библиотеке на это стояло условие

#include <stdio.h>
#include <math.h>
#include "robot/log.h"

int nf;

inline int sign(double x) {
    return x > 0 ? 1 : x < 0 ? -1 : 0;
}

inline double abs_error(double x, double real) {
    return fabs(x - real);
}

inline double real_error(double x, double real) {
    return fabs(x / real - 1);
}

inline double execute(double (*func)(double), double x) {
    nf++;
    return func(x);
}

double exer0(double x) {
    return pow(x - 1, 6) - 1;
}

double exer1(double x) {
    return pow(x, 3) + 4 * pow(x, 2) - 10;
}

double exer2(double x) {
    return cos(x) - x;
}

double exer3(double x) {
    return pow(x, 3) + 4 * pow(x, 2) + cos(x - 1) - 6;
}

double exer4(double x) {
    return pow(M_E, pow(x, 2) + 7 * x - 30) - 1;
}

double exer5(double x) {
    return pow(M_E, 1 - x) - 1;
}

double exer6(double x) {
    return atan(x);
}

double exer7(double x) {
    return pow(x, 3) + 4 * pow(x, 2) - 15;
}

double exer8(double x) {
    return pow(x, 2) + pow(x - 1, 5);
}

double exer9(double x) {
    return cos(x) - x * pow(M_E, x);
}

double exer10(double x) {
    return pow(x, 3) + log(1 + x);
}

double exer11(double x) {
    return pow(sin(x), 2) - pow(x, 2) + 1;
}

double exer12(double x) {
    return pow(x, 4) - sin(x);
}

double exer13(double x) {
    return pow(x, 2) + 3 * x - 10;
}

int chordsMethod(double* x, double a, double b, double (*func)(double), double epsilon) {
    double fa, fb, fc, c[2];
    int i = 0, aSign, k;
    c[0] = a; c[1] = b; fa = execute(func, a); fb = execute(func, b);

    if (fabs(fa) < epsilon) { *x = a; return 1; }
    if (fabs(fb) < epsilon) { *x = b; return 1; }

    aSign = sign(fa);
    if (aSign == sign(fb)) return 0;

    for (k = 1; fabs(c[1] - c[0]) > epsilon; k++) {
        i = !i;
        c[i] = (a * fb - b * fa) / (fb - fa);
        fc = execute(func, c[i]);

        if (fabs(fc) < epsilon) {
            *x = c[i];
            return k;
        }
        if (aSign == sign(fc)) {
            a = c[i];
            fa = fc;
        }
        else {
            b = c[i];
            fb = fc;
        }
    }

    *x = c[i];
    return k - 1;
}

inline double derivative(double (*func)(double), double x) {
    const double h = 1e-8;
    return (execute(func, x + h) - execute(func, x - h)) / (2 * h);
}

// x_{n+1} = x_n - (f'(x_n))^(-1) * f(x_n)
int NewtonMethod(double* x, double xStart, double (*func)(double), double epsilon) {
    double fx, dfx, fpre, dfpre;
    int k = 0;

    *x = xStart;

    do {
        fpre = fx;
        dfpre = dfx;

        fx = execute(func, *x);
        dfx = derivative(func, *x);

        if (fabs(dfx) < epsilon || fabs(dfpre - dfx) < epsilon) return k;

        *x -= fx / dfx;
        k++;

    } while (fabs(fx) > epsilon && abs(fpre - fx) > epsilon && k < 25);

    return k;
}

typedef struct {
    double (*func)(double), a, b, x;
} Test;

typedef struct {
    double (*func)(double);
    unsigned size;
    double X[3];
    double x;
} Test1;

int main() {
    if (!lopen("roots")) return 1;
    double temp; int itCount, i, j;

    Test tests0[] = { { exer7, 0, 2, 1.6319808055660636 }, { exer11, 0, 2, 1.4044916482153412 }, { exer13, 0, 2, 2 } };
    Test1 tests1[] = { { exer2, 2, {0, 2}, 0.7390851332151600 }, { exer3, 2, {1.8, 3}, 1 },  { exer9, 1, {2}, 0.5177573636824583 } };

    lprintl("> Метод хорд:\n");
    lprintl("----- ----- ----- ----- ----- ----- -----\n");
    for (i = 0; i < sizeof(tests0) / sizeof(Test); i++) {
        lprintl("  [%lg, %lg]\n", tests0[i].a, tests0[i].b);
        nf = 0;
        itCount = chordsMethod(&temp, tests0[i].a, tests0[i].b, tests0[i].func, 1.e-15);
        if (itCount) {
            lprintl("  X: %.13lf\n  X (Real): %.13lf\n", temp, tests0[i].x);
            lprintl("  Абс. погр.: %.13le\n  Отн. погр.: %.13le\n", abs_error(temp, tests0[i].x), real_error(temp, tests0[i].x));
            lprintl("  Итераций: %d\n  Число обращений к функции: %d\n", itCount, nf);
        }
        else lprintl("  Ошибка для [%lg, %lg]\n", tests0[i].a, tests0[i].b);
        lprintl("----- ----- ----- ----- ----- ----- -----\n");
    }
    lprintl("\n> Метод Ньютона:\n");
    lprintl("----- ----- ----- ----- ----- ----- -----\n");
    for (i = 0; i < sizeof(tests1) / sizeof(Test1); i++) {
        for (j = 0; j < tests1[i].size; j++) {
            lprintl("  %lg\n", tests1[i].X[j]);
            nf = 0;
            itCount = NewtonMethod(&temp, tests1[i].X[j], tests1[i].func, 1.e-15);
            if (itCount) {
                lprintl("  X: %.13lf\n  X (Real): %.13lf\n", temp, tests1[i].x);
                lprintl("  Абс. погр.: %.13le\n  Отн. погр.: %.13le\n", abs_error(temp, tests1[i].x), real_error(temp, tests1[i].x));
                lprintl("  Итераций: %d\n  Число обращений к функции: %d\n", itCount, nf);
            }
            else lprintl("  Ошибка для %lg\n", tests1[i].X[j]);
            lprintl("----- ----- ----- ----- ----- ----- -----\n");
        }
    }

    lclosel();
    return 0;
}