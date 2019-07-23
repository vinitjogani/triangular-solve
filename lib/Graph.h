#pragma once
#include "SparseMatrix.h"
#include <set>
#include <vector>
#include <tuple>

class Node
{
    // Level of the node
    int level = -1;
    // Edges of the node
    std::vector<int> edgesIn, edgesOut;

    public:
    void addInwardEdge(int);
    void addOutwardEdge(int);

    friend class Graph;
    friend class SparseMatrix;
};

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
    Graph(SparseMatrix&);
    std::vector<std::tuple<int, int>> LBC();

    friend class SparseMatrix;
};
