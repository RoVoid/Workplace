#include <stdio.h>
#include <stdlib.h>

#define LOG_FOLDER "../logs/"
#include "../robot/log.h"

#define EPS 1e-12

double dabs(const double x) {
    union {
        double d;
        unsigned long long u;
    } v = { x };
    v.u &= 0x7FFFFFFFFFFFFFFF;
    return v.d;
}

int main() {
    FILE* file = fopen("matrix/gauss_in.txt", "r");
    if (!file) return -1;

    unsigned n, i, j;
    fscanf(file, "%u", &n);

    double** a = (double**)malloc(n * sizeof(double*));
    if (!a) return -2;

    for (i = 0; i < n; i++) {
        a[i] = (double*)malloc((n + 1) * sizeof(double));
        if (!a[i]) {
            for (unsigned k = 0; k < i; k++) free(a[k]);
            free(a);
            return -3;
        }
    }

    for (i = 0; i < n; i++)
        for (j = 0; j < n + 1; j++)
            fscanf(file, "%lf", &a[i][j]);
    fclose(file);

    unsigned row = 0;

    for (j = 0; j < n + 1 && row < n; j++) {
        // Поиск максимального по модулю элемента в столбце j, начиная с row
        unsigned maxRow = row;
        double maxAbs = dabs(a[maxRow][j]);
        for (i = row + 1; i < n; i++) {
            double val = dabs(a[i][j]);
            if (val > maxAbs) {
                maxAbs = val;
                maxRow = i;
            }
        }

        // Пропуск нулевых столбцов
        if (maxAbs < EPS) continue;

        // Меняем местами строки
        if (maxRow != row) {
            double* tmp = a[row];
            a[row] = a[maxRow];
            a[maxRow] = tmp;
        }

        // Нормируем ступеньку
        double pivot = a[row][j];
        for (unsigned k = j; k < n + 1; k++) {
            a[row][k] /= pivot;
            if (dabs(a[row][k]) < EPS) a[row][k] = 0;
        }

        // Обнуляем все элементы в столбце j, ниже и выше row
        for (i = 0; i < n; i++) {
            if (i == row) continue;
            double factor = a[i][j];
            for (unsigned k = j; k < n + 1; k++) {
                a[i][k] -= factor * a[row][k];
                if (dabs(a[i][k]) < EPS) a[i][k] = 0;
            }
        }

        row++;
    }

    lopen("gauss");

    lprintl("Ступенчатый вид матрицы:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            lprintl("%10.5lf ", a[i][j]);
        }
        lprintl("| %10.5lf\n", a[i][n]);
    }

    int error = 0;
    // Проверка на нулевые строки с ненулевым решением
    for (i = 0; i < n; i++) {
        unsigned p = n;
        for (j = 0; j < n; j++) {
            if (dabs(a[i][j]) > EPS) {
                p = j;
                break;
            }
        }
        if (p == n && dabs(a[i][n]) > EPS) {
            error = 1;
            break;
        }
    }

    lprintl("\n");

    if (error) lprintl("Система несовместна\n");
    else {
        lprintl("Решение:\n");

        for (j = 0; j < n; j++) {
            double s = 0;
            for (i = 0; i < n; i++) {
                if (dabs(a[i][j] - 1) < EPS) {
                    s = a[i][n];
                    break;
                }
            }
            lprintl(" x%u = %.10lf\n", j + 1, s);
        }
    }

    lclosel();

    for (i = 0; i < n; i++) free(a[i]);
    free(a);

    return 0;
}
