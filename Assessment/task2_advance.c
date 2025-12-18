#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double **allocMatrix(int r, int c) {
    double **m = malloc(r * sizeof(double *));
    for (int i = 0; i < r; i++)
        m[i] = malloc(c * sizeof(double));
    return m;
}

void freeMatrix(double **m, int r) {
    for (int i = 0; i < r; i++)
        free(m[i]);
    free(m);
}

void printMatrix(FILE *f, double **m, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (isnan(m[i][j]))
                fprintf(f, "NaN");
            else
                fprintf(f, "%lf", m[i][j]);
            if (j < c - 1) fprintf(f, ", ");
        }
        fprintf(f, "\n");
    }
}

int capThreads(int req, int max) {
    return req < max ? req : max;
}

double **add(double **A, double **B, int r, int c, int t) {
    double **R = allocMatrix(r, c);
    t = capThreads(t, r);
    #pragma omp parallel for num_threads(t)
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            R[i][j] = A[i][j] + B[i][j];
    return R;
}

double **sub(double **A, double **B, int r, int c, int t) {
    double **R = allocMatrix(r, c);
    t = capThreads(t, r);
    #pragma omp parallel for num_threads(t)
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            R[i][j] = A[i][j] - B[i][j];
    return R;
}

double **elemMul(double **A, double **B, int r, int c, int t) {
    double **R = allocMatrix(r, c);
    t = capThreads(t, r);
    #pragma omp parallel for num_threads(t)
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            R[i][j] = A[i][j] * B[i][j];
    return R;
}

double **elemDiv(double **A, double **B, int r, int c, int t) {
    double **R = allocMatrix(r, c);
    t = capThreads(t, r);
    #pragma omp parallel for num_threads(t)
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            R[i][j] = (B[i][j] == 0) ? NAN : A[i][j] / B[i][j];
    return R;
}

double **transpose(double **A, int r, int c, int t) {
    double **T = allocMatrix(c, r);
    t = capThreads(t, r);
    #pragma omp parallel for num_threads(t)
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            T[j][i] = A[i][j];
    return T;
}

double **matMul(double **A, double **B, int rA, int cA, int cB, int t) {
    double **R = allocMatrix(rA, cB);
    t = capThreads(t, rA);
    #pragma omp parallel for num_threads(t)
    for (int i = 0; i < rA; i++) {
        for (int j = 0; j < cB; j++) {
            double sum = 0;
            for (int k = 0; k < cA; k++)
                sum += A[i][k] * B[k][j];
            R[i][j] = sum;
        }
    }
    return R;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./matrix <file> <threads>\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) {
        printf("Input file error\n");
        return 1;
    }

    int threads = atoi(argv[2]);
    FILE *out = fopen("Task2_Matrices_12.txt", "w");

    int pair = 1;

    while (1) {
        int r1, c1, r2, c2;
        if (fscanf(in, "%d,%d", &r1, &c1) != 2) break;
        double **A = allocMatrix(r1, c1);
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c1; j++)
                fscanf(in, "%lf,", &A[i][j]);

        if (fscanf(in, "%d,%d", &r2, &c2) != 2) break;
        double **B = allocMatrix(r2, c2);
        for (int i = 0; i < r2; i++)
            for (int j = 0; j < c2; j++)
                fscanf(in, "%lf,", &B[i][j]);

        fprintf(out, "\n===============================\n");
        fprintf(out, "MATRIX PAIR %d\n", pair++);
        fprintf(out, "===============================\n\n");

        if (r1 == r2 && c1 == c2) {
            fprintf(out, "Addition (%d,%d)\n", r1, c1);
            double **R = add(A, B, r1, c1, threads);
            printMatrix(out, R, r1, c1);
            freeMatrix(R, r1);

            fprintf(out, "\nSubtraction (%d,%d)\n", r1, c1);
            R = sub(A, B, r1, c1, threads);
            printMatrix(out, R, r1, c1);
            freeMatrix(R, r1);

            fprintf(out, "\nElement-wise Multiply (%d,%d)\n", r1, c1);
            R = elemMul(A, B, r1, c1, threads);
            printMatrix(out, R, r1, c1);
            freeMatrix(R, r1);

            fprintf(out, "\nElement-wise Divide (%d,%d)\n", r1, c1);
            R = elemDiv(A, B, r1, c1, threads);
            printMatrix(out, R, r1, c1);
            freeMatrix(R, r1);
        } else {
            fprintf(out, "Addition cannot be done (different size)\n");
            fprintf(out, "Subtraction cannot be done (different size)\n");
            fprintf(out, "Element-wise operations cannot be done (different size)\n");
        }

        fprintf(out, "\nTranspose A (%d,%d)\n", c1, r1);
        double **T = transpose(A, r1, c1, threads);
        printMatrix(out, T, c1, r1);
        freeMatrix(T, c1);

        fprintf(out, "\nTranspose B (%d,%d)\n", c2, r2);
        T = transpose(B, r2, c2, threads);
        printMatrix(out, T, c2, r2);
        freeMatrix(T, c2);

        if (c1 == r2) {
            fprintf(out, "\nMatrix Multiply (%d,%d)\n", r1, c2);
            double **R = matMul(A, B, r1, c1, c2, threads);
            printMatrix(out, R, r1, c2);
            freeMatrix(R, r1);
        } else {
            fprintf(out, "\nMatrix multiplication cannot be done\n");
        }

        freeMatrix(A, r1);
        freeMatrix(B, r2);
    }

    fclose(in);
    fclose(out);
    return 0;
}