#pragma once

class Sparse {

    private:

        // Contains pointers to start and end of column
        int *ptr; 
        // Contains the row number for the i-th non-zero value
        int *ind;
        // Contains the actual values of the non-zero entries
        double *val; 

        void _solve(double*, int);
    
    public:
    
        // Matrix dimensions
        const int m, n, nz;
        // Constructor takes in number of rows, columns and non-zero entries
        Sparse(int, int, int);

        void lsolve(double*);

        double *operator *(double*);

        friend class Graph;
        friend class Reader;
        friend class Utils;
};