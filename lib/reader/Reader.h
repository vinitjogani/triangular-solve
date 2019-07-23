#pragma once
// System libraries
#include <stdio.h>
#include <string>
// Project libraries
#include "../Sparse.h"


const int MAX_LINE_LENGTH = 256;


enum ReadError {
    UnknownException = -1,
    IOException = 1,
    NullPointerException = 2,
    AccessException = 3,
    FormatError = 4
};


class Reader {

    private: 

        Sparse *M;


    protected:

        static void error(std::string);
        static void error(ReadError, std::string);

        void initialize(int, int, int);
        void setRowPointer(int, int);
        void setColumnPointer(int, int);
        void setValue(int, double);
        Sparse &data();


    public: 

        virtual Sparse &read() = 0;
};