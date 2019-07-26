// System libraries
#include <map>
#include <vector>
// Project libraries
#include "../includes.h"

class Utils {
    /*
        An extendable list of convenience methods for this specific
        problem of triangular systems that ties all other functions together.
     */

    public:
        static double* toDenseVector(Sparse &);
        static void timedTest(Reader&, Reader&);
        static bool compareVec(double*, double*, int);
        static void triangularSolve(std::map<int, std::vector<int>>, Sparse&, double *);
        static void triangularSolveParitioned(std::vector<Partition>, Sparse &, double*);
};