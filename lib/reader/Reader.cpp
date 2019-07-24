// System libraries
#include <iostream>
#include <string>
#include <stdlib.h>
// Project libraries
#include "../includes.h"


void Reader::error(ReadError code, std::string message) {
    std::cout << "[-] Error code: " << code << ". " << message << ".\n";
    exit(code);
}

void Reader::error(std::string message) {
    error(UnknownException, message);
}

void Reader::initialize(int m, int n, int nz) {
    M = new Sparse(m, n, nz);
}

void Reader::setRowPointer(int index, int rowNumber) {
    if (M == NULL)
        error(NullPointerException, "Matrix not yet initialized");
    if (index >= M->nz)
        error(AccessException, "Index out of bounds");

    M->ind[index] = rowNumber - 1;
}

void Reader::setColumnPointer(int column, int startPointer) {
    if (M == NULL)
        error(NullPointerException, "Matrix not yet initialized");
    if (column > M->n)
        error(AccessException, "Index out of bounds");

    M->ptr[column] = startPointer;
}

void Reader::setValue(int index, double value) {
    if (M == NULL)
        error(NullPointerException, "Matrix not yet initialized");
    if (index >= M->nz)
        error(AccessException, "Index out of bounds");

    M->val[index] = value;
}

Sparse &Reader::data() {
    return *M;
}