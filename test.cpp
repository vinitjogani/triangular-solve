#include <stdio.h>
#include <time.h>
#include "SparseMatrix.h"

double gettime() {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    return start.tv_sec * 1000 + ((double)start.tv_nsec / 1e+6);
}

int cmp(double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        if (x[i] - y[i] >= 1e-10 || x[i] - y[i] <= -1e-10) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    double start;
    if (argc < 3) {
        fprintf(stderr, "Usage: %s a_filename b_filename\n", argv[0]);
        return 1;
    }

    start = gettime();
    SparseMatrix A = SparseMatrix::readFrom(argv[1]);
    SparseMatrix b = SparseMatrix::readFrom(argv[2]);
    printf("Loaded in %lf ms.\n", gettime() - start);

    start = gettime();
    double *x = b.dense();
    A.lsolve(x);
    printf("Solved in %0.2lf ms.\n", gettime() - start);

    double *new_b = A * x;
    if(cmp(new_b, b.dense(), A.n)) printf("Test passed.\n");
    else printf("Test failed.\n");

    return 0;
}