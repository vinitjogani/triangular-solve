// System libraries
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
// Project libraries
#include "../includes.h"


const double CONFIDENCE_INTERVAL = 1e-10;


double* Utils::toDenseVector(Sparse &M) {
    // Check for a vector
    if (M.n != 1) {
        fprintf(stderr, "[-] Argument must be a vector.\n");
        exit(1);
    }
    // Convert to dense vector by setting everything else to 0 and populating
    // non-zero values
    double *out = (double*)malloc(sizeof(double) * M.m);
    memset(out, 0, M.m * sizeof(double));
    for (int i = M.ptr[0]; i < M.ptr[1]; i++) {
        out[M.ind[i]] = M.val[i];
    }
    return out;
}

bool Utils::compareVec(double *x, double *y, int n) {
    // Compare two vectors <x> and <y> to see if they are "close enough"
    // accounting for rounding issues
    bool isSame = true;
    for (int i = 0; i < n; i++) {
        if (x[i] - y[i] >= CONFIDENCE_INTERVAL || x[i] - y[i] <= -CONFIDENCE_INTERVAL) {
            std::cout << "[-] Mismatch found: x["<<i<<"]=" << x[i] << " and y["<<i<<"]=" << y[i] << ".\n"; 
            isSame = false;
        }
    }
    return isSame;
}


void Utils::triangularSolve(std::map<int, std::vector<int>> levels, Sparse &A, double *x) {
    // Parallely solve each column given the level set analysis.
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

    // Load matrices into memory
    start = omp_get_wtime();
    Sparse &A = readerA.read();
    Sparse &b = readerB.read();
    double *x = Utils::toDenseVector(b);
    end = omp_get_wtime();
    loadingTime = end - start;

    // Perform analysis
    start = omp_get_wtime();
    Graph G(A);
    std::unordered_set<int> reachSet = G.getReachSet(b);
    std::map<int, std::vector<int>> levels = G.getLevelSets(reachSet);
    end = omp_get_wtime();
    analysisTime = end - start;

    // Actually solve the triangular system
    start = omp_get_wtime();
    triangularSolve(levels, A, x);
    end = omp_get_wtime();
    solvingTime = end - start;

    // Compare results
    start = omp_get_wtime();
    double *truth = Utils::toDenseVector(b);
    double *result = A*x;
    int testPassed = Utils::compareVec(truth, result, A.n);
    end = omp_get_wtime();
    testingTime = end - start;

    // Print test summary
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