#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Sparse.h"

#define MAX_LINE_LENGTH 200

Sparse::Sparse(int m, int n, int nz) : m(m), n(n), nz(nz) {
    // Allocate memory for the pointer arrays
    this->ptr = (int*)malloc(sizeof(int) * n + 1);
    this->ind = (int*)malloc(sizeof(int) * nz);
    this->val = (double*)malloc(sizeof(double) * nz);
}

void Sparse::_solve(double *b, int j) {
    // Solves for a particular column, given the RHS (b).
    double tmp;
    int i;
    if (b[j] != 0) {
        b[j] /= val[ptr[j]];
        for (int p = ptr[j] + 1; p < ptr[j + 1]; p++) {
            // Limit operations done in the atomic update
            tmp = val[p] * b[j];
            i = ind[p];
            // Atomically update the RHS value
            #pragma omp atomic update
            b[i] -= tmp;
        }
    }
}

double *Sparse::operator*(double *x) {
    // Multiply sparse matrix with a dense vector for comparing 
    // solved result with the actual RHS.

    double *y = (double*)malloc(sizeof(double) * n);
    memset(y, 0, sizeof(double) * n);
    for (int j = 0; j < n; j++) {
        for (int p = ptr[j]; p < ptr[j + 1]; p++) {
            y[ind[p]] += ((val)[p] * x[j]);
        }
    }
    return y;
}