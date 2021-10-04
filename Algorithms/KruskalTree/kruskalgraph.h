#ifndef KRUSKALGRAPH_H
#define KRUSKALGRAPH_H

#include <vector>
#include <utility>
#include <map>
#include <list>

#include "matrix.h"

class KruskalGraph
{
public:
    using Node = size_t;
    using Edge = std::pair<Node, Node>;
    using Cost = int;
    using GraphMatrix = Matrix<Cost>;
    using Tree = std::list<Edge>;

    explicit KruskalGraph();

    bool build(const GraphMatrix& graphMatrix);

    const Tree& getMinTreeEdges() const;
    const Tree& getMaxTreeEdges() const;

private:
    using Component = std::list<Node>;
    using EdgeCostsMap = std::multimap<Cost, Edge>;

    void _buildGraph(const Matrix<int>& edgeCostsMatrix);
    void _buildMinTreeFromGraph();
    void _buildMaxTreeFromGraph();
    void _buildEmptyComponents();
    void _reset();

    bool _addEdgeToTree(const Edge& edge);
    void _bindOrphanNodeToNonOrphanNode(Node orphanNode, Node componentNode);
    void _bindOrphanNodes(Node firstOrphanNode, Node secondOrphanNode);
    void _mergeComponents(Node enclosingComponentNode, Node mergedComponentNode);

    static constexpr size_t scNullComponent{-1u};

    size_t mNodesCount;
    EdgeCostsMap mEdgeCostsMap;
    std::vector<Component> mComponents;
    std::vector<size_t> mComponentNumbers;
    Tree mMinTreeEdges;
    Tree mMaxTreeEdges;
};

#endif // KRUSKALGRAPH_H
