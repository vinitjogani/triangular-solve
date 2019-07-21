class SparseMatrix {
    int *ptr, *ind;
    double *val;
    SparseMatrix(int, int, int);
    
    public:
    const int m, n, nz;
    static SparseMatrix readFrom(char *);
    double* dense();
    void lsolve(double*);
    void _solve(double*, int);
    double *operator *(double*);
};