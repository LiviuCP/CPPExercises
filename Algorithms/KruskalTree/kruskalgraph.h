#ifndef KRUSKALGRAPH_H
#define KRUSKALGRAPH_H

#include <vector>
#include <utility>
#include <map>
#include <list>
#include <limits>

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

    const Tree& getMinTree() const;
    const Tree& getMaxTree() const;

private:
    using Component = std::list<Node>;
    using EdgeCostsMap = std::multimap<Cost, Edge>;

    void _buildGraph(const GraphMatrix& graphMatrix);
    void _buildMinTreeFromGraph();
    void _buildMaxTreeFromGraph();
    void _buildEmptyComponents();
    void _reset();

    bool _addEdgeToTree(const Edge& edge);
    void _bindOrphanNodeToNonOrphanNode(Node orphanNode, Node componentNode);
    void _bindOrphanNodes(Node firstOrphanNode, Node secondOrphanNode);
    void _mergeComponents(Node enclosingComponentNode, Node mergedComponentNode);

    static constexpr size_t scNullComponent{std::numeric_limits<size_t>::max()};

    size_t mNodesCount;
    EdgeCostsMap mEdgeCostsMap;
    std::vector<Component> mComponents;
    std::vector<size_t> mComponentNumbers;
    Tree mMinTree;
    Tree mMaxTree;
};

#endif // KRUSKALGRAPH_H
