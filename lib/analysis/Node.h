
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
    friend class Sparse;
};