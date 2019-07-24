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
    // Initialize fields and allocate memory for adjacency list
    n = A.n;
    adj = new Node[A.n];

    // Populate adjacency list
    for (int col = 0; col < A.n; col++) {
        for (int row = A.ptr[col]+1; row < A.ptr[col+1]; row++) {
            adj[col].edgesOut.push_back(A.ind[row]);
            adj[A.ind[row]].edgesIn.push_back(col);
        }
    }
}

std::unordered_set<int> Graph::DFS(std::vector<int> frontier) {
    // Use unordered set for constant-time search and space efficiency
    std::unordered_set<int> explored;

    // Reset all levels to 0, in case DFS was run multiple times
    for (int source = 0; source < n; source++) 
        adj[source].level = 0;

    // Perform actual depth-first search
    int node, level, child;
    while (frontier.size() > 0) {
        // Pop the last element, and add it to explored set
        node = frontier.back();
        frontier.pop_back();
        explored.insert(node);

        // Traversing in reverse order is faster for triangular solve matrices
        // as it leads to lesser dependencies (which would cause nodes to be re-added
        // fewer times).
        for (int childI = adj[node].edgesOut.size() - 1;  childI >= 0; childI --) {
            // Get the appropriate child and push it back to the frontier if 
            // it is discovered at a deeper level (we want level to be max distance
            // from source node).
            child = adj[node].edgesOut[childI];
            level =  adj[node].level + 1;
            if (adj[child].level < level) {
                adj[child].level = level;
                frontier.push_back(child);
                // Update critical path length if necessary
                if (level + 1 > P) P = level + 1;
            }
        }
    }

    // Return the set of explored nodes, can be used to determine reach set
    return explored;
}

std::unordered_set<int> Graph::getReachSet(Sparse &b) {
    // Check for a vector
    if (b.n != 1) {
        fprintf(stderr, "[-] Argument must be the RHS vector.\n");
        exit(1);
    }

    // If b vector is dense, no need to do this work, add everything to reach set
    if (b.m * b.n == b.nz) {
        std::unordered_set<int> explored;
        for (int i = 0; i < n; i ++) 
            explored.insert(i); 
        return explored;
    }

    // Otherwise, add all non-zero row numbers to the frontier
    std::vector<int> frontier;
    for (int i = 0; i < b.ptr[1]; i++) {
        frontier.push_back(b.ind[i]);
    }

    // Use DFS to compute the reach set from the frontier
    return DFS(frontier);
}

std::map<int, std::vector<int>> Graph::getLevelSets(std::unordered_set<int> reachSet) {
    // Compute all the source nodes (nodes with no incoming edges)
    std::vector<int> sourceNodes;
    for (int node : reachSet) {
        if (adj[node].edgesIn.size() == 0) {
            sourceNodes.push_back(node);
        }
    }
    // Sort them to ensure topological order as unordered map does not guarantee order
    std::sort(sourceNodes.begin(), sourceNodes.end());

    // Run DFS on the source nodes to get the levels for each node
    DFS(sourceNodes);

    // For each column, if it exists in the reach set, add it to 
    // the appropriate level set.
    std::map<int, std::vector<int>> levels;
    for (int i = 0; i < n; i++) {
        if (reachSet.count(i)) {
            levels[adj[i].level].push_back(i);
        }
    }

    // Return the computed levels
    return levels;
}