#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 200

typedef struct {
    int m, n, nz;
    int *ptr, *ind;
    double *val;
} CompressedMatrix;

double *to_dense_vector(CompressedMatrix *x) {
    double *out = malloc(sizeof(double) * x->m);
    memset(out, 0, x->m * sizeof(double));
    for (int i = 0; i < x->ptr[1]; i++) {
        out[x->ind[i]] = x->val[i];
    }
    return out;
}

/*
 * Lower triangular solver Lx=b
 * L is stored in the compressed column storage format
 * Inputs are:
 * n : the matrix dimension
 * Lp : the column pointer of L
 * Li : the row index of L
 * Lx : the values of L
 * In/Out:
 * x : the right hand-side b at start and the solution x at the end.
 */
int lsolve(CompressedMatrix *M, double *b) {
    int p, j;
    for (j = 0; j < M->n; j++) {
        if (b[j] != 0) {
            b[j] /= M->val[M->ptr[j]];
            for (p = M->ptr[j] + 1; p < M->ptr[j + 1]; p++) {
                b[M->ind[p]] -= M->val[p] * b[j];
            }
        }
    }
    return (1);
}



/*
 * Sparse matrix-vector multiply: y = A*x
 * A is stored in the compressed column storage format
 * Inputs:
 * Ap : the column pointer of A
 * Ai : the row index of A
 * Ax : the values of A
 * x : is a dense vector
 * Output:
 * y : is a dense vector that stores the result of multiplication
 */
double *spmv_csc(CompressedMatrix *A, double *x) {
    double *y = malloc(sizeof(double) * A->n);
    memset(y, 0, sizeof(double) * A->n);

    int p, j;
    for (j = 0; j < A->n; j++) {
        for (p = A->ptr[j]; p < A->ptr[j + 1]; p++) {
            y[A->ind[p]] += ((A->val)[p] * x[j]);
        }
    }
    return y;
}

int cmp_vec(double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        if (x[i] - y[i] >= 1e-10 || x[i] - y[i] <= -1e-10) {
            printf("x[%d] = %0.90lf, y[%d] = %0.90lf\n", i, x[i], i, y[i]);
            return 1;
        }
    }
    return 0;
}

CompressedMatrix *read_matrix(char *filename) {
    FILE *f = fopen(filename, "r");
    // Skip the header
    char *buffer = (char *)malloc(MAX_LINE_LENGTH);
    do
        fgets(buffer, MAX_LINE_LENGTH - 1, f);
    while (buffer[0] == '%');
    // Read in the size
    CompressedMatrix *M = malloc(sizeof(CompressedMatrix));
    int read_count = sscanf(buffer, "%d %d %d", &(M->m), &(M->n), &(M->nz));
    if (read_count == 2)
        M->nz = M->m * M->n;
    free(buffer);
    // Allocate the memory for the matrix
    M->ptr = malloc(sizeof(int) * M->n + 1);
    M->ind = malloc(sizeof(int) * M->nz);
    M->val = malloc(sizeof(double) * M->nz);
    // Read in the actual matrix in CSR format
    int row, col = 1, row_prev=0, col_prev=0;
    M->ptr[0] = 0;
    for (int n = 0; n < M->nz; n++) {
        if (read_count == 3) {
            fscanf(f, "%d %d %lf", &row, &col, M->val + n);
        } else {
            row = (row_prev + 1 > M->m) ? 1 : row_prev + 1;
            if (row < row_prev) col++;
            fscanf(f, "%lf", M->val + n);
        }
        if (col > col_prev) {
            for(int i = col_prev; i < col; i ++)
                M->ptr[i] = n;
        }
        M->ind[n] = row-1;

        row_prev = row;
        col_prev = col;
    }
    M->ptr[M->n] = M->nz;
    // Return the matrix
    return M;
}

void pprint(CompressedMatrix *matrix) {
    printf("Compressed matrix: [MxN]=[%dx%d] with %d non-zero values.\n", matrix->m, matrix->n, matrix->nz);
    
    printf("Ptr = [ ");
    for (int i = 0; i < matrix->n + 1; i++) {
        printf("%d ", matrix->ptr[i]);
    }
    printf("]\n");

    printf("Ind = [ ");
    for (int i = 0; i < matrix->nz; i++) {
        printf("%d ", matrix->ind[i]);
    }
    printf("]\n");

    printf("Val = [ ");
    for (int i = 0; i < matrix->nz; i++) {
        printf("%lf ", matrix->val[i]);
    }
    printf("]\n");
}

double gettime() {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    return start.tv_sec * 1000 + ((double)start.tv_nsec / 1e+6);
}

int main(int argc, char **argv) {
    double start;
    if (argc < 3) {
        fprintf(stderr, "Usage: %s a_filename b_filename\n", argv[0]);
        exit(1);
    }

    start = gettime();
    CompressedMatrix *A = read_matrix(argv[1]);
    CompressedMatrix *b = read_matrix(argv[2]);
    printf("Loaded in %lf ms.\n", gettime() - start);

    start = gettime();
    double *x = to_dense_vector(b);
    lsolve(A, x);
    printf("Solved in %0.2lf ms.\n", gettime() - start);

    double *new_b = spmv_csc(A, x);
    if(!cmp_vec(new_b, to_dense_vector(b), A->n)) {
        printf("Test passed.\n");
    }
    else {
        printf("Test failed.\n");
    }

    return 0;
}