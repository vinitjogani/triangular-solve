#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SparseMatrix.h"

#define MAX_LINE_LENGTH 200

SparseMatrix::SparseMatrix(int m, int n, int nz) : m(m), n(n), nz(nz) {
    this->ptr = (int*)malloc(sizeof(int) * n + 1);
    this->ind = (int*)malloc(sizeof(int) * nz);
    this->val = (double*)malloc(sizeof(double) * nz);
}

SparseMatrix SparseMatrix::readFrom(char *filename) {
    FILE *f = fopen(filename, "r");
    // Skip the header
    char *buffer = (char *)malloc(MAX_LINE_LENGTH);
    do {
        if(fgets(buffer, MAX_LINE_LENGTH - 1, f) == NULL) {
            fprintf(stderr, "Could not read file.\n");
            exit(1);
        }
    }
    while (buffer[0] == '%');
    // Read in the size
    int m, n, nz;
    int read_count = sscanf(buffer, "%d %d %d", &m, &n, &nz);
    if (read_count == 2) nz = m * n;
    free(buffer);
    // Allocate the memory for the matrix
    SparseMatrix M (m, n, nz);
    // Read in the actual matrix in CSR format
    int row, col = 1, row_prev=0, col_prev=0;
    M.ptr[0] = 0;
    for (int n = 0; n < M.nz; n++) {
        if (read_count == 3) {
            if(fscanf(f, "%d %d %lf", &row, &col, M.val + n) != 3) {
                fprintf(stderr, "Invalid format.\n");
                exit(1);
            }
        } else {
            row = (row_prev + 1 > M.m) ? 1 : row_prev + 1;
            if (row < row_prev) col++;
            if(fscanf(f, "%lf", M.val + n) != 1) {
                fprintf(stderr, "Invalid format.\n");
                exit(1);
            }
        }
        if (col > col_prev) {
            for(int i = col_prev; i < col; i ++)
                M.ptr[i] = n;
        }
        M.ind[n] = row-1;

        row_prev = row;
        col_prev = col;
    }
    M.ptr[M.n] = M.nz;
    // Return the matrix
    return M;
}

double* SparseMatrix::dense() {
    double *out = (double*)malloc(sizeof(double) * m);
    memset(out, 0, m * sizeof(double));
    for (int i = 0; i < ptr[1]; i++) {
        out[ind[i]] = val[i];
    }
    return out;
}

void SparseMatrix::_solve(double *b, int j) {
    double tmp;
    int i;
    if (b[j] != 0) {
        b[j] /= val[ptr[j]];
        for (int p = ptr[j] + 1; p < ptr[j + 1]; p++) {
            tmp = val[p] * b[j];
            i = ind[p];

            #pragma omp atomic update
            b[i] -= tmp;
        }
    }
}

// void SparseMatrix::lsolve(double *b) {
//     for (int j = 0; j < n; j ++) {
//         _solve(b, j);
//     }
// }

double *SparseMatrix::operator*(double *x) {
    double *y = (double*)malloc(sizeof(double) * n);
    memset(y, 0, sizeof(double) * n);

    for (int j = 0; j < n; j++) {
        for (int p = ptr[j]; p < ptr[j + 1]; p++) {
            y[ind[p]] += ((val)[p] * x[j]);
        }
    }
    return y;
}