#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <omp.h>

#include "includes.h"


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
            for(unsigned col = 0; col < levels[l].size(); col++)
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
    double start, end;
    
    start = omp_get_wtime();
    Reader *readerA = new MTXReader((char*)"../af_0_k101.mtx");
    Sparse &A = readerA->read();
    end = omp_get_wtime();
    std::cout << "[+] Loaded A in " << end - start << " seconds.\n";

    start = omp_get_wtime();
    Reader *readerB = new MTXReader((char*)"../b_sparse_af_0_k101.mtx");
    Sparse &b = readerB->read();
    double *x = b.dense();
    end = omp_get_wtime();
    std::cout << "[+] Loaded b in " << end - start << " seconds.\n";

    start = omp_get_wtime();
    A.lsolve(x);
    end = omp_get_wtime();
    std::cout << "[+] Solved in " << end - start << " seconds.\n";

    // double *new_b = A * x;
    // if(cmp(new_b, b.dense(), A.n)) std::cout << "Test passed.\n";
    // else std::cout << "Test failed.\n";;

}