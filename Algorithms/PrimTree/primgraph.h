#ifndef PRIMGRAPH_H
#define PRIMGRAPH_H

#include <vector>
#include <utility>
#include <map>
#include <list>
#include <limits>

#include "matrix.h"

class PrimGraph
{
public:
    using Node = size_t;
    using Edge = std::pair<Node, Node>;
    using Cost = int;
    using GraphMatrix = Matrix<Cost>;
    using Tree = std::list<Edge>;

    explicit PrimGraph();

    bool build(const GraphMatrix& graphMatrix);

    const Tree& getMinTree() const;
    const Tree& getMaxTree() const;

private:
    struct NodeInfo
    {
        NodeInfo(Cost cost);

        Cost mNodeCost{scMaxCost};
        Node mPrecedingNode{scNullNode};
        bool mIsAddedToTree{false};
    };

    using NodeInfoMap = std::vector<NodeInfo>;

    void _buildGraph(const GraphMatrix& graphMatrix);
    void _buildTreeFromGraph(bool isMinTree);
    void _doBuildTree(const NodeInfoMap& nodeInfoMap, bool isMinTree);
    void _updateNeighborCosts(Node node, NodeInfoMap& nodeInfoMap, bool (*isNewCostBetter)(Cost newCost, Cost oldCost));
    void _reset();

    static constexpr Cost scMinCost{0};
    static constexpr Cost scMaxCost{std::numeric_limits<PrimGraph::Cost>::max()};
    static constexpr Node scNullNode{std::numeric_limits<PrimGraph::Node>::max()};

    size_t mNodesCount;
    GraphMatrix mGraphMatrix;
    Tree mMinTree;
    Tree mMaxTree;
};

#endif // PRIMGRAPH_H
