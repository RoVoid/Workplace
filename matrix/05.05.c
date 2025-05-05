#include <stdio.h>
#include <stdlib.h>

#define LOG_FOLDER "../logs"
#include "../robot/log.h"

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

int main() {
    int n;
    lopen("matrix1");

    scanf("%d", &n);

    lclosel();
    return 0;
}
