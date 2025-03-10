#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 512
#define N 512
#define X 512
#define BLOCK_SIZE 32

void initialize_matrix(double matrix[M][N]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void multiply_row_order(double A[M][N], double B[N][X], double C[M][X]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < X; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_column_order(double A[M][N], double B[N][X], double C[M][X]) {
    for (int j = 0; j < X; j++) {
        for (int i = 0; i < M; i++) {
            C[i][j] = 0;
        }
        for (int k = 0; k < N; k++) {
            for (int i = 0; i < M; i++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_blocking(double A[M][N], double B[N][X], double C[M][X]) {
    for (int i = 0; i < M; i += BLOCK_SIZE) {
        for (int j = 0; j < X; j += BLOCK_SIZE) {
            for (int k = 0; k < N; k += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE; ii++) {
                    for (int jj = j; jj < j + BLOCK_SIZE; jj++) {
                        for (int kk = k; kk < k + BLOCK_SIZE; kk++) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

void benchmark(void (*multiply_func)(double[M][N], double[N][X], double[M][X]), char *desc) {
    double A[M][N], B[N][X], C[M][X] = {0};
    initialize_matrix(A);
    initialize_matrix(B);
    clock_t start = clock();
    multiply_func(A, B, C);
    clock_t end = clock();
    printf("%s: Tempo gasto = %lf segundos\n", desc, (double)(end - start) / CLOCKS_PER_SEC);
}

int main() {
    srand(time(NULL));
    benchmark(multiply_row_order, "Multiplicação em ordem de linha");
    benchmark(multiply_column_order, "Multiplicação em ordem de coluna");
    benchmark(multiply_blocking, "Multiplicação com blocagem");
    return 0;
}
