#pragma once
#include "Sparse.h"
#include <set>
#include <vector>
#include <tuple>


class Graph
{
    // Number of vertices in the Graph
    int n;
    // Critical path length
    int P = 0;
    // Adjacency matrix
    Node *adj;
    // Source nodes 
    std::vector<int> sourceNodes;

    void DFS();

    public: 
    Graph(Sparse&);
    std::vector<std::tuple<int, int>> LBC();

    friend class Sparse;
};
