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
    /*
        Store basic information for the nodes of the graph.
    */

    public:
        // Level of the node
        int level = -1;
        // Edges of the node
        std::vector<int> edgesIn, edgesOut;
};

class Graph
{
    /*
        Methods to analyze the sparsity structure of a matrix
        by constructing a DAG and performing search algorithms.
     */

    private:
        // Number of vertices in the graph
        int n;
        // The critical path length of the graph
        int P = 0;
        // The adjacency matrix of the graph
        Node *adj;
        // DFS algorithm whch takes in an initial frontier of nodes to explore
        // and returns the set of explored nodes.
        std::unordered_set<int> DFS(std::vector<int> frontier);

    public: 
        // Constructs a dependency graph from the sparse matrix
        Graph(Sparse&);
        // Get all elements in the reach set, given the RHS vector for triangular solve
        std::unordered_set<int> getReachSet(Sparse &);
        // Get the level sets, given the reach-Set
        std::map<int, std::vector<int>> getLevelSets(std::unordered_set<int>);

    friend class Sparse;
};
