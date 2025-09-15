#include <stdio.h>
#include <stdlib.h>

void swap(int* a, unsigned i, unsigned j) {
    if (i == j) return;
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void quickSort(int* a, unsigned n) {
    unsigned b[2000], l, r;
    int pi = 0;

    b[0] = 0;
    b[1] = n - 1;

    while (pi >= 0) {
        l = b[pi * 2];
        r = b[pi * 2 + 1];
        pi--;

        if (l >= r) continue;

        int p = a[(l + r) / 2];
        int i = l, j = r;

        while (i <= j) {
            while (a[i] < p) i++;
            while (a[j] > p) j--;
            if (i <= j) {
                swap(a, i, j);
                i++;
                j--;
            }
        }

        if (i >= n) i = n - 1;
        if (j < 0) j = 0;

        if (l < j) {
            pi++;
            b[pi * 2] = l;
            b[pi * 2 + 1] = j;
        }
        if (i < r) {
            pi++;
            b[pi * 2] = i;
            b[pi * 2 + 1] = r;
        }
    }
}



int main() {
    unsigned n;
    int* a;

    scanf("%u", &n);
    a = (int*)malloc(n * sizeof(int));
    if (!a) return 1;
    for (unsigned i = 0; i < n; i++) scanf("%d", a + i);

    quickSort(a, n);

    for (unsigned i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");

    free(a);
    return 0;
}
