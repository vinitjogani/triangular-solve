#pragma once
// System libraries
#include <stdio.h>
// Project libraries
#include "../includes.h"

class MTXReader : public Reader {
    /*
        Implements the Reader interface specifically for reading 
        MTX files.
     */

    FILE *f;

    void readHeader(int&, int&, int&);
    void readContents();

    public:
    MTXReader(char*);
    Sparse &read();
};