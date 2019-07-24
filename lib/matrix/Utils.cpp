// System libraries
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
// Project libraries
#include "../includes.h"

double* Utils::toDenseVector(Sparse &M) {
    if (M.n != 1) {
        fprintf(stderr, "[-] Argument must be a vector.\n");
        exit(1);
    }

    double *out = (double*)malloc(sizeof(double) * M.m);
    memset(out, 0, M.m * sizeof(double));
    for (int i = M.ptr[0]; i < M.ptr[1]; i++) {
        out[M.ind[i]] = M.val[i];
    }

    return out;
}

int Utils::compareVec(double *x, double *y, int n) {
    int j = 1;
    for (int i = 0; i < n; i++) {
        if (x[i] - y[i] >= 1e-10 || x[i] - y[i] <= -1e-10) {
            printf("%d %0.15lf %0.15lf\n", i, x[i], y[i]);
            j = 0;
        }
    }
    return j;
}


void Utils::triangularSolve(std::map<int, std::vector<int>> levels, Sparse &A, double *x) {
    #pragma omp parallel
    {
        for (unsigned l = 0; l < levels.size(); l++) {
            #pragma omp for
            for(unsigned col = 0; col < levels[l].size(); col++)
                A._solve(x, levels[l][col]);
        }
    }
}


void Utils::timedTest(Reader& readerA, Reader& readerB) {
    double start, end;
    double loadingTime, analysisTime, solvingTime, testingTime;

    start = omp_get_wtime();
    Sparse &A = readerA.read();
    Sparse &b = readerB.read();
    double *x = Utils::toDenseVector(b);
    end = omp_get_wtime();
    loadingTime = end - start;

    start = omp_get_wtime();
    Graph G(A);
    std::unordered_set<int> reachSet = G.getReachSet(b);
    std::map<int, std::vector<int>> levels = G.getLevelSets(reachSet);
    end = omp_get_wtime();
    analysisTime = end - start;

    start = omp_get_wtime();
    triangularSolve(levels, A, x);
    end = omp_get_wtime();
    solvingTime = end - start;

    start = omp_get_wtime();
    double *truth = Utils::toDenseVector(b);
    double *result = A*x;
    int testPassed = Utils::compareVec(truth, result, A.n);
    end = omp_get_wtime();
    testingTime = end - start;

    if(testPassed) {
        std::cout << "[+] Test passed. Summary: \n";
    } 
    else {
        std::cout << "[-] Test failed. Summary: \n";
    }
    std::cout << "\t==> Matrices loaded in: \t\t" << loadingTime << " seconds.\n";
    std::cout << "\t==> Sparsity structure analyzed in: \t" << analysisTime << " seconds.\n";
    std::cout << "\t==> Triangular system solved in: \t" << solvingTime << " seconds.\n";
    std::cout << "\t==> Results verified in: \t\t" << testingTime << " seconds.\n";
}