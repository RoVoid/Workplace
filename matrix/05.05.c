#include <stdio.h>
#include <stdlib.h>

#define MATRIX_MAX_COUNT 10

typedef struct {
    unsigned n, m;
    float* array;
} Matrix;

Matrix matrixs[MATRIX_MAX_COUNT], result;

void clearMatrixs(char flag) {
    for (unsigned i = 0; i < MATRIX_MAX_COUNT; i++) {
        if (!matrixs[i].array) continue;
        matrixs[i].n = matrixs[i].m = 0;
        free(matrixs[i].array);
        matrixs[i].array = NULL;
    }
    if (flag) {
        free(result.array);
        result.array = NULL;
    }
}

void inputMatrixs() {
    clearMatrixs(0);
    FILE* file = fopen("matrix/in.txt", "r");
    unsigned i = 0, n, m;
    if (!file) {
        printf("File Problem\n");
        return;
    }

    while (i < MATRIX_MAX_COUNT && fscanf(file, "%u%u", &n, &m) == 2) {
        matrixs[i].n = n;
        matrixs[i].m = m;
        matrixs[i].array = (float*)malloc(n * m * sizeof(float));
        for (unsigned k = 0; k < n * m; k++)
            fscanf(file, "%f", &matrixs[i].array[k]);
        i++;
    }

    fclose(file);
}

void printMatrix(int i) {
    if (i >= 0) {
        for (unsigned k = 0; k < matrixs[i].n * matrixs[i].m; k++) {
            printf("%6.3f ", matrixs[i].array[k]);
            if (k % matrixs[i].m == matrixs[i].m - 1) printf("\n");
        }
        for (int k = 0; k < matrixs[i].m; k++) printf("-------");
        printf("\n");
    }
    else {
        for (unsigned k = 0; k < result.n * result.m; k++) {
            printf("%6.3f ", result.array[k]);
            if (k % result.m == result.m - 1) printf("\n");
        }
        for (int k = 0; k < result.m; k++) printf("-------");
        printf("\n");
    }
    printf("\n");
}

int main() {
    inputMatrixs();

    unsigned start;
    scanf("%u", &start);
    result.n = matrixs[start].n;
    result.m = matrixs[start].m;
    result.array = (float*)malloc(result.n * result.m * sizeof(float));
    for (unsigned k = 0; k < result.n * result.m; k++)
        result.array[k] = matrixs[start].array[k];

    printf("\n--------------------------\n");
    printf("i - update input matrixs\n");
    printf("= - set equal to matrix\n");
    printf("+ - add matrix\n");
    printf("* - multiply by matrix\n");
    printf("m - multiply by number\n");
    printf("o - print matrix\n");
    printf("x - exit");
    printf("\n--------------------------\n\n");

    char operation;
    while (1) {
        while ((operation = getchar()) == '\n');

        if (operation == 'i') {
            inputMatrixs();
            continue;
        }
        else if (operation == '=') {
            unsigned i;
            scanf("%u", &i);
            free(result.array);
            result.n = matrixs[i].n;
            result.m = matrixs[i].m;
            result.array = (float*)malloc(result.n * result.m * sizeof(float));
            for (unsigned k = 0; k < result.n * result.m; k++)
                result.array[k] = matrixs[i].array[k];
        }
        else if (operation == '+') {
            unsigned i;
            scanf("%u", &i);
            if (matrixs[i].n != result.n || matrixs[i].m != result.m) continue;
            for (unsigned k = 0; k < result.n * result.m; k++)
                result.array[k] += matrixs[i].array[k];
        }
        else if (operation == '*') {
            unsigned i;
            scanf("%u", &i);
            if (result.m != matrixs[i].n) continue;
            Matrix newResult;
            newResult.n = result.n;
            newResult.m = matrixs[i].m;
            newResult.array = (float*)calloc(newResult.n * newResult.m, sizeof(float));
            for (unsigned r = 0; r < newResult.n; r++) {
                for (unsigned c = 0; c < newResult.m; c++) {
                    for (unsigned k = 0; k < result.m; k++) {
                        newResult.array[r * newResult.m + c] +=
                            result.array[r * result.m + k] * matrixs[i].array[k * matrixs[i].m + c];
                    }
                }
            }
            free(result.array);
            result = newResult;
        }
        else if (operation == 'm') {
            float x;
            scanf("%f", &x);
            for (unsigned k = 0; k < result.n * result.m; k++)
                result.array[k] *= x;
        }
        else if (operation == 'o') {
            unsigned i;
            scanf("%u", &i);
            printMatrix(i);
            continue;;
        }
        else if (operation == 'x') {
            break;
        }
        printMatrix(-1);
    }

    clearMatrixs(1);
    return 0;
}
