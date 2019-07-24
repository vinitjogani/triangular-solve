#pragma once

class Sparse {
    /*
        Represents a Sparse matrix/vector stored in Compressed
        Column Storage (CCS) format.
     */

    private:

        // Contains pointers to start and end of column
        int *ptr; 
        // Contains the row number for the i-th non-zero value
        int *ind;
        // Contains the actual values of the non-zero entries
        double *val; 
        // Eliminate a particular column from all depending rows
        void _solve(double*, int);
    
    public:
    
        // Matrix dimensions
        const int m, n, nz;
        // Constructor takes in number of rows, columns and non-zero entries
        Sparse(int, int, int);
        // Sparse matrix to dense vector multiply, to check results
        double *operator *(double*);

        // These classes provide analysis and other utilities
        friend class Graph;
        friend class Reader;
        friend class Utils;
};