#pragma once
// System libraries
#include <set>
#include <vector>
#include <tuple>
// Project libraries
#include "../includes.h"


struct Node
{
    public:
        // Level of the node
        int level = -1;
        // Edges of the node
        std::vector<int> edgesIn, edgesOut;
};

class Graph
{
    private:
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
