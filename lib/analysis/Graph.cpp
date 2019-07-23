// System libraries
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <omp.h>
// Project libraries
#include "../includes.h"

const int thresh = 8;
const int agg = 128;


Graph::Graph(Sparse &A)
{
    n = A.n;
    adj = new Node[A.n];

    for (int col = 0; col < A.n; col++) {
        for (int row = A.ptr[col]+1; row < A.ptr[col+1]; row++) {
            adj[col].edgesOut.push_back(A.ind[row]);
            adj[A.ind[row]].edgesIn.push_back(col);
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