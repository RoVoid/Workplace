#include <stdio.h> //двумерный по строкам
#include <stdlib.h>
#include <math.h>

long long factorial(int n) {
    if (n <= 1) return 1;
    long long fact = 1;
    for (int i = 2;i <= n;i++) {
        fact *= i;
    }
    return fact;
}

void Hilbert(double** mas, int n) {
    int i, j;
    for (i = 0;i < n;i++) {
        for (j = 0;j < n;j++) {
            mas[i][j] = 1.0 / (i + j + 1.0);
        }
    }return;
}
void InverseHilbert(double** mas, int n) {
    int i, j;
    for (i = 0;i < n;i++) {
        for (j = 0;j < n;j++) {
            double num = factorial(n + i) * factorial(n + j);
            double denom = pow(factorial(i), 2) * pow(factorial(j), 2) * factorial(n - i - 1) * factorial(n - j - 1) * (i + j + 1);
            mas[i][j] = pow(-1, i + j) * (num / denom);
        }
    }
}
int main() {
    int n;
    printf("Укажите количество строк и столбцов\n");
    scanf("%d", &n);
    FILE* fp = fopen("output.txt", "w");
    double** mas = NULL;
    mas = (double**)malloc(n * sizeof(double*));
    for (int i = 0;i < n;i++) { mas[i] = (double*)malloc(n * sizeof(double)); }
    Hilbert(mas, n);
    for (int i = 0;i < n;i++) {
        for (int j = 0; j < n; j++) { fprintf(fp, "%10.6lf", mas[i][j]); }
        fprintf(fp, "\n");
    }
    return 0;
}