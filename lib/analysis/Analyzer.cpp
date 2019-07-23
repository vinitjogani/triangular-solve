#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <omp.h>
#include "Graph.h"
#include "Sparse.h"

void Sparse::lsolve(double *b) {
    Graph G(*this);

    std::vector<std::tuple<int, int>> L = G.LBC();
    std::map<int, std::vector<int>> levels;

    for (int i = 0; i < G.n; i++) {
        levels[G.adj[i].level].push_back(i);
    }

    double start = omp_get_wtime();
    // for (std::tuple<int, int> l : L) {
    #pragma omp parallel
    {
        for (int l = 0; l < G.P; l++) {
            #pragma omp for
            for(int col = 0; col < levels[l].size(); col++)
            {
                _solve(b, levels[l][col]);
            }
        }
    }
    std::cout << omp_get_wtime() - start << " seconds taken\n";
    // }
}


int cmp(double *x, double *y, int n) {
    int j = 1;
    for (int i = 0; i < n; i++) {
        if (x[i] - y[i] >= 1e-10 || x[i] - y[i] <= -1e-10) {
            printf("%d %0.15lf %0.15lf\n", i, x[i], y[i]);
            j = 0;
        }
    }
    return j;
}


int main() {
    Sparse A = Sparse::readFrom((char*)"af_0_k101.mtx");
    Sparse b = Sparse::readFrom((char*)"b_sparse_af_0_k101.mtx");
    std::cout << "Loaded\n";
    
    double *x = b.dense();
    A.lsolve(x);
    std::cout << "Solved.";

    double *new_b = A * x;
    if(cmp(new_b, b.dense(), A.n)) std::cout << "Test passed.\n";
    else std::cout << "Test failed.\n";;

}