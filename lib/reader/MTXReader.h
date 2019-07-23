#pragma once
// System libraries
#include <stdio.h>
// Project libraries
#include "../Sparse.h"

class MTXReader : Reader {
    FILE *f;

    void readHeader(int&, int&, int&);
    void readContents();

    public:
    MTXReader(char*);
    Sparse &read();
};