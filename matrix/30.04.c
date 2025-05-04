#include <stdio.h>
#include <stdlib.h>

#define LOG_FOLDER "../logs"
#include "../robot/log.h"


int max(int i, int j) {
    return i > j ? i : j;
}

int abs(int x) {
    return x < 0 ? -x : x;
}

int fillingMethod1(int n, int i, int j, char inverse) {
    if (!inverse) return n - max(i, j);
    if (i == j) return i ? 2 : 1;
    if (abs(i - j) == 1) return -1;
    return 0;
}

int fillingMethod2(int n, int i, int j, char inverse) {
    if (!inverse) return n - max(n - 1 - i, j);
    if (i + j == n - 1) return i ? 2 : 1;
    if (abs(i + j - n + 1) == 1) return -1;
    return 0;
}

void code0(int n) {
    int* a = (int*)malloc(n * n * sizeof(int));

    for (int k = 0; k < n * n; k++) {
        a[k] = fillingMethod2(n, k / n, k % n, 0);
        lprintl("%3d ", a[k]);
        if (k % n == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("----");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        lprintl("%3d ", fillingMethod2(n, k / n, k % n, 1));
        if (k % n == n - 1) lprintl("\n");
    }

    free(a);
}

void code1(int n) {
    int* a = (int*)malloc(n * n * sizeof(int));

    for (int k = 0; k < n * n; k++) {
        a[k] = fillingMethod2(n, k % n, k / n, 0);
        lprintl("%3d ", a[k]);
        if (k % n == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("----");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        lprintl("%3d ", fillingMethod2(n, k % n, k / n, 1));
        if (k % n == n - 1) lprintl("\n");
    }

    free(a);
}

void code2(int n) {
    int** a = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) a[i] = (int*)malloc(n * sizeof(int));

    for (int k = 0; k < n * n; k++) {
        int i = k / n;
        int j = k % n;
        a[i][j] = fillingMethod1(n, i, j, 0);
        lprintl("%3d ", a[i][j]);
        if (j == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("----");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        int i = k / n;
        int j = k % n;
        lprintl("%3d ", fillingMethod1(n, i, j, 1));
        if (j == n - 1) lprintl("\n");
    }

    for (int i = 0; i < n; i++) free(a[i]);
    free(a);
}

void code3(int n) {
    int** a = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) a[i] = (int*)malloc(n * sizeof(int));

    for (int k = 0; k < n * n; k++) {
        int i = k % n;
        int j = k / n;
        a[j][i] = fillingMethod1(n, i, j, 0);
        lprintl("%3d ", a[j][i]);
        if (i == n - 1) lprintl("\n");
    }

    for (int k = 0; k < n; k++) lprintl("----");
    lprintl("\n");

    for (int k = 0; k < n * n; k++) {
        int i = k % n;
        int j = k / n;
        lprintl("%3d ", fillingMethod1(n, i, j, 1));
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
