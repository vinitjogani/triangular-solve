#pragma once
// System libraries
#include <stdio.h>
#include <string>
// Project libraries
#include "../includes.h"

// Constants and error definitions
const int MAX_LINE_LENGTH = 256;
enum ReadError {
    UnknownException = -1,
    IOException = 1,
    NullPointerException = 2,
    AccessException = 3,
    FormatError = 4
};


class Reader {

    /*
        Abstract class for Sparse matrix Readers. Can be extended to other formats
        that can be read into a Compressed Column Storage matrix.
     */

    private: 
        // This is the copy of the matrix we operate on.
        Sparse *M;


    protected:
        // Convenience methods to throw neat errors.
        static void error(std::string);
        static void error(ReadError, std::string);
        // Initializers and setters to modify Sparse matrix
        // so that the private instances only have to be exposed to 
        // one abstract class instead of other implementations.
        void initialize(int, int, int);
        void setRowPointer(int, int);
        void setColumnPointer(int, int);
        void setValue(int, double);
        // Get the underlying matrix object.
        Sparse &data();


    public: 
        // Abstract method to be implemented by any reader.
        virtual Sparse &read() = 0;
};