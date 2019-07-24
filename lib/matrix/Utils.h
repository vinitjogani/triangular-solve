// System libraries
#include <map>
#include <vector>
// Project libraries
#include "../includes.h"

class Utils {
    public:
    static double* toDenseVector(Sparse &);
    static void timedTest(Reader&, Reader&);
    static int compareVec(double*, double*, int);
    static void triangularSolve(std::map<int, std::vector<int>>, Sparse&, double *);
};