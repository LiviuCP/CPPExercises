#ifndef KRUSKALTREE_H
#define KRUSKALTREE_H

#include <vector>
#include <utility>
#include <map>
#include <list>

#include "matrix.h"

using Node = size_t;
using Edge = std::pair<Node, Node>;
using Component = std::list<Node>;
using Cost = int;
using GraphMatrix = Matrix<Cost>;
using EdgeCostsMap = std::multimap<Cost, Edge>;
using EdgeList = std::list<Edge>;

class KruskalTree
{
public:
    explicit KruskalTree();

    bool build(const GraphMatrix& graphMatrix);
    const EdgeList& getTreeEdges() const;

private:
    void _buildGraph(const Matrix<int>& edgeCostsMatrix);
    void _buildTreeFromGraph();
    void _buildEmptyComponents();
    void _reset();

    void _bindOrphanNodeToNonOrphanNode(Node orphanNode, Node componentNode);
    void _bindOrphanNodes(Node firstOrphanNode, Node secondOrphanNode);
    void _mergeComponents(Node enclosingComponentNode, Node mergedComponentNode);

    static constexpr size_t scNullComponent{-1u};

    size_t mNodesCount;
    EdgeCostsMap mEdgeCostsMap;
    std::vector<Component> mComponents;
    std::vector<size_t> mComponentNumbers;
    EdgeList mTreeEdges;
};

#endif // KRUSKALTREE_H
