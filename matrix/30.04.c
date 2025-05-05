#include <stdio.h>
#include <stdlib.h>

#define LOG_FOLDER "../logs"
#include "../robot/log.h"


float max(int i, int j) {
    return i > j ? i : j;
}

float _abs(float x) {
    return x < 0 ? -x : x;
}

float fillingMethod1(int n, int i, int j, char inverse) {
    if (!inverse) return n - max(i, j);
    if (i == j) return i ? 2 : 1;
    if (_abs(i - j) == 1) return -1;
    return 0;
}

float fillingMethod2(int n, int i, int j, char inverse) {
    if (!inverse) return n - max(n - 1 - i, j);
    if (i + j == n - 1) return i ? 2 : 1;
    if (_abs(i + j - n + 1) == 1) return -1;
    return 0;
}

float fillingMethod4(int n, int i, int j, char inverse) {
    if (!inverse) return _abs(i - j);
    if (n <= 2) return 0;
    if (i == j) return j % (n - 1) == 0 ? -(n - 2.0) / (2 * n - 2.0) : -1;
    if (_abs(i - j) == 1) return 0.5;
    if ((i == 0 && j == n - 1) || (i == n - 1 && j == 0)) return 1.0 / (2 * n - 2.0);
    return 0;
}

void code0(int n) {
    float* a = (float*)malloc(n * n * sizeof(float));

    for (int k = 0; k < n * n; k++) {
        a[k] = fillingMethod2(n, k / n, k % n, 0);
        lprintl("%6.3f ", a[k]);
        if (k % n == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("-------");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        lprintl("%6.3f ", fillingMethod2(n, k / n, k % n, 1));
        if (k % n == n - 1) lprintl("\n");
    }

    free(a);
}

void code1(int n) {
    float* a = (float*)malloc(n * n * sizeof(float));

    for (int k = 0; k < n * n; k++) {
        a[k] = fillingMethod2(n, k % n, k / n, 0);
        lprintl("%6.3f ", a[k]);
        if (k % n == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("-------");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        lprintl("%6.3f ", fillingMethod2(n, k % n, k / n, 1));
        if (k % n == n - 1) lprintl("\n");
    }

    free(a);
}

void code2(int n) {
    float** a = (float**)malloc(n * sizeof(float*));
    for (int i = 0; i < n; i++) a[i] = (float*)malloc(n * sizeof(float));

    for (int k = 0; k < n * n; k++) {
        int i = k / n;
        int j = k % n;
        a[i][j] = fillingMethod4(n, i, j, 0);
        lprintl("%6.3f ", a[i][j]);
        if (j == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("-------");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        int i = k / n;
        int j = k % n;
        lprintl("%6.3f ", fillingMethod4(n, i, j, 1));
        if (j == n - 1) lprintl("\n");
    }

    for (int i = 0; i < n; i++) free(a[i]);
    free(a);
}

void code3(int n) {
    float** a = (float**)malloc(n * sizeof(float*));
    for (int i = 0; i < n; i++) a[i] = (float*)malloc(n * sizeof(float));

    for (int k = 0; k < n * n; k++) {
        int i = k % n;
        int j = k / n;
        a[j][i] = fillingMethod4(n, i, j, 0);
        lprintl("%6.3f ", a[j][i]);
        if (i == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("-------");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        int i = k % n;
        int j = k / n;
        lprintl("%6.3f ", fillingMethod4(n, i, j, 1));
        if (i == n - 1) lprintl("\n");
    }

    for (int i = 0; i < n; i++) free(a[i]);
    free(a);
}

int main() {
    int n;
    lopen("matrix0");

    scanf("%d", &n);

    code0(n);
    lprintl("\n\n\n");

    code1(n);
    lprintl("\n\n\n");

    code2(n);
    lprintl("\n\n\n");

    code3(n);

    lclosel();
    return 0;
}
