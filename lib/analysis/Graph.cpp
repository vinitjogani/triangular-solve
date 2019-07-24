// System libraries
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <unordered_set>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
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
}

std::unordered_set<int> Graph::DFS(std::vector<int> frontier) {
    // Use map for constant-time search and space efficiency
    std::unordered_set<int> explored;

    for (int source = 0; source < n; source++) 
        adj[source].level = 0;

    int node, level, child;
    while (frontier.size() > 0) {
        node = frontier.back();
        frontier.pop_back();
        explored.insert(node);
        // Traversing in reverse order is faster for triangular solve matrices
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

    return explored;
}

std::unordered_set<int> Graph::getReachSet(Sparse &b) {
    if (b.n != 1) {
        fprintf(stderr, "[-] Argument must be the RHS vector.\n");
        exit(1);
    }

    if (b.m * b.n == b.nz) {
        std::unordered_set<int> explored;
        for (int i = 0; i < n; i ++) 
            explored.insert(i); 
        return explored;
    }

    std::vector<int> frontier;
    for (int i = 0; i < b.ptr[1]; i++) {
        frontier.push_back(b.ind[i]);
    }

    return DFS(frontier);
}

std::map<int, std::vector<int>> Graph::getLevelSets(std::unordered_set<int> reachSet) {
    std::vector<int> sourceNodes;
    for (int node : reachSet) {
        if (adj[node].edgesIn.size() == 0) {
            sourceNodes.push_back(node);
        }
    }
    std::sort(sourceNodes.begin(), sourceNodes.end());

    DFS(sourceNodes);

    std::map<int, std::vector<int>> levels;
    for (int i = 0; i < n; i++) {
        if (reachSet.count(i)) {
            levels[adj[i].level].push_back(i);
        }
    }

    return levels;
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