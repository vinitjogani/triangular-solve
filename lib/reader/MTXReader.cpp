// System libraries
#include <stdio.h>
// Project libraries
#include "../includes.h"

MTXReader::MTXReader(char *filename) {
    f = fopen(filename, "r");
    if (f == NULL) error("Could not open file");
}

void MTXReader::readHeader(int &m, int &n, int &nz) {
    // Skip the header comments
    char buffer[MAX_LINE_LENGTH];
    do {
        if(!fgets(buffer, MAX_LINE_LENGTH - 1, f)) {
            error("Could not read file");
        }
    }
    while (buffer[0] == '%');

    // Read in the size
    int readCount = sscanf(buffer, "%d %d %d", &m, &n, &nz);
    if (readCount < 2) error(FormatError, "Improper format");
    if (readCount == 2) nz = m * n;
}

void MTXReader::readContents() {
    Sparse &M = data();

    // Read in the actual matrix in CCS format
    bool isSparse = (M.m*M.n) != M.nz;
    int row, col = 1, col_prev=0;
    double val;
    for (int e = 0; e < M.nz; e++) {
        // Handle both sparse and dense matrices
        if (isSparse) {
            if(fscanf(f, "%d %d %lf", &row, &col, &val) != 3) {
                error(FormatError, "Improper format");
            }
        } 
        else {
            if (row + 1 > M.m) {
                col++;
                row = 1;
            } 
            else {
                row++;
            }
            if(fscanf(f, "%lf", &val) != 1) {
                error(FormatError, "Improper format");
            }
        }

        // Update value
        setValue(e, val);
        // Update column pointers if we moved on to new column
        if (col > col_prev) {
            for(int i = col_prev; i < col; i ++)
                setColumnPointer(i, e);
        }
        col_prev = col;
        // Update row index, subtracting 1 to make it zero-indexed
        setRowPointer(e, row);
    }

    // Fill in pointers for any remaining columns 
    for(int i = col; i <= M.n; i ++)
        setColumnPointer(i, M.nz);
}

Sparse &MTXReader::read() {
    // Read the header
    int m, n, nz;
    readHeader(m, n, nz);
    // Allocate the memory for the matrix
    initialize(m, n, nz);
    // Read the contents of the matrix
    readContents();

    return data();
}
