#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <omp.h>
#include "Graph.h"
#include "SparseMatrix.h"

const int thresh = 8;
const int agg = 128;

void Node::addInwardEdge(int from) {
    edgesIn.push_back(from);
}

void Node::addOutwardEdge(int to) {
    edgesOut.push_back(to);
}

Graph::Graph(SparseMatrix &A)
{
    n = A.n;
    adj = new Node[A.n];

    for (int col = 0; col < A.n; col++) {
        for (int row = A.ptr[col]+1; row < A.ptr[col+1]; row++) {
            adj[col].addOutwardEdge(A.ind[row]);
            adj[A.ind[row]].addInwardEdge(col);
        }
    }

    for (int col = 0; col < A.n; col++) {
        if (adj[col].edgesIn.size() == 0) {
            sourceNodes.push_back(col);
        }
    }

    std::cout << "Starting DFS.\n";
    DFS();
    std::cout << "DFS done.\n";
}

void Graph::DFS() {
    std::vector<int> frontier;
    for (int source: sourceNodes) {
        frontier.push_back(source);
        adj[source].level = 0;
    }

    int node, level, child;
    while (frontier.size() > 0) {
        node = frontier.back();
        frontier.pop_back();
        
        for (int childI = adj[node].edgesOut.size() - 1;  childI >= 0; childI --) {
            child = adj[node].edgesOut[childI];
            level =  adj[node].level + 1;
            if (adj[child].level < level) {
                adj[child].level = level;
                frontier.push_back(child);
                if (level + 1 > P) P = level + 1;
            }
        }
    }

    std::cout << "Critical path length = " << P << ".\n";
}

std::vector<std::tuple<int, int>> Graph::LBC() {
    std::vector<std::tuple<int, int>> L;

    if (n <= thresh) {
        L.push_back(std::make_tuple(0, P));
        return L;
    }

    for (int i = 0; i < P - agg; i += agg) {
        L.push_back(std::make_tuple(i, i+64));
    }

    return L;
}

void SparseMatrix::lsolve(double *b) {
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
    SparseMatrix A = SparseMatrix::readFrom((char*)"af_0_k101.mtx");
    SparseMatrix b = SparseMatrix::readFrom((char*)"b_sparse_af_0_k101.mtx");
    std::cout << "Loaded\n";
    
    double *x = b.dense();
    A.lsolve(x);
    std::cout << "Solved.";

    double *new_b = A * x;
    if(cmp(new_b, b.dense(), A.n)) std::cout << "Test passed.\n";
    else std::cout << "Test failed.\n";;

}