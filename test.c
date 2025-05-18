double int_ur(
    double eps,
    double alpha,
    double a,
    double (*f)(double),
    double (*chebyshev_integral)(double a, double b, double (*f)(double)),
    int (*sign)(double)
) {
    int step = 0;
    double res = 0.0, sum = 0.0, x = a, p = x;
    double x1, x2, i1, i2;
    double h = 1e-8, delta, psum, chi, hnew;
    double sr = 0.0, qr = 0.0;
    const double eps2 = 1e-16;

    // Этап 1: Интегрирование до достижения заданной точности
    while (sum < alpha - eps2) {
        hnew = h;

        i1 = chebyshev_integral(x, x + h, f);
        x1 = x + h;
        x2 = x + h / 2;
        i2 = chebyshev_integral(x, x2, f) + chebyshev_integral(x2, x1, f);

        delta = (i1 - i2) / 31.0;

        if (fabs(delta) < eps) {
            step++;
            psum = sum;
            p = x;
            x += h;
            sum += i2;
            sr += delta;
            qr += fabs(delta);
        }

        chi = pow(fabs(delta / eps), 1.0 / 5.0);
        if (chi > 10.0) chi = 10.0;
        if (chi < 0.1) chi = 0.1;

        hnew = 0.95 * h / chi;
        h = hnew;
    }

    // Этап 2: Метод хорд для точного вычисления границы интегрирования
    double pnew = p;
    double fa, fb, fc = 0.0, c[2];
    int i = 0, ifa, ifb, ifc, k, m = 0;

    c[0] = p;
    c[1] = x;

    fa = psum - alpha;
    fb = sum - alpha;

    ifa = sign(fa);
    ifb = sign(fb);

    for (k = 1; fabs(c[1] - c[0]) > eps2; k++) {
        m++;
        i = (k % 2 == 1) ? 0 : 1;

        c[i] = (p * fb - x * fa) / (fb - fa);
        fc = psum + chebyshev_integral(p, c[i], f) - alpha;
        ifc = sign(fc);

        res = c[i];

        if (fabs(fc) < eps2) {
            res = c[i];
            psum += chebyshev_integral(p, c[i], f);
            break;
        }

        if (ifc * ifa == 1) {
            fa = fc;
            psum += chebyshev_integral(p, c[i], f);
            p = c[i];
        }
        else if (ifb * ifc == 1) {
            x = c[i];
            fb = fc;
        }
    }

    // Этап 3: Подсчет дополнительной оценки ошибки
    i1 = chebyshev_integral(pnew, res, f);
    x1 = pnew + res;
    x2 = pnew + res / 2;
    i2 = chebyshev_integral(pnew, x2, f) + chebyshev_integral(x2, x1, f);
    delta = (i1 - i2) / 31.0;

    sr += delta;
    qr += fabs(delta);
    psum += chebyshev_integral(p, res, f);

    // Этап 4: Вывод результатов
    printf("srednevz %0.16lf\nqarant=%0.16lf\n", sr, qr);
    printf("kolvo xord %d\nkolvo itter integr %d\n", m, step);

    double x_min = (res - qr) / f(res);
    double x_max = (res + qr) / f(res);

    printf("xmin %0.16lf\nxmax %0.16lf\n", x_min, x_max);

    return res;
}
