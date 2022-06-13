#ifndef PRIM_H
#define PRIM_H

#include <vector>
#include <limits>

#include "baseengine.h"

class PrimEngine : public BaseEngine
{
public:
    explicit PrimEngine();
    bool buildTrees(const GraphMatrix& graphMatrix) override;

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
    void _reset() override;

    static constexpr Cost scMinCost{0};
    static constexpr Cost scMaxCost{std::numeric_limits<Cost>::max()};
    static constexpr Node scNullNode{std::numeric_limits<Node>::max()};

    GraphMatrix mGraphMatrix;
};

#endif // PRIM_H
