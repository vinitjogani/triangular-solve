#pragma once
// System libraries
#include <set>
#include <vector>
#include <map>
#include <tuple>
#include <unordered_set>
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

        std::unordered_set<int> DFS(std::vector<int> frontier);

    public: 

        Graph(Sparse&);
        
        std::unordered_set<int> getReachSet(Sparse &);
        std::map<int, std::vector<int>> getLevelSets(std::unordered_set<int>);
        std::vector<std::tuple<int, int>> LBC();

    friend class Sparse;
};
