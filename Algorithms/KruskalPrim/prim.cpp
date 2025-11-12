#include "prim.h"

PrimEngine::PrimEngine()
    : BaseEngine{"Prim"}
{
}

bool PrimEngine::buildTrees(const GraphMatrix& graphMatrix)
{
    bool success{false};

    _reset();

    const matrix_size_t c_RowsCount{graphMatrix.getNrOfRows()};

    if (c_RowsCount > 0 && c_RowsCount == graphMatrix.getNrOfColumns())
    {
        _buildGraph(graphMatrix);
        _buildTreeFromGraph(true);  // minimum cost tree
        _buildTreeFromGraph(false); // maximum cost tree

        success = true;
    }

    return success;
}

void PrimEngine::_buildGraph(const GraphMatrix& graphMatrix)
{
    const matrix_size_t c_RowsCount{graphMatrix.getNrOfRows()};

    if (c_RowsCount > 0 && c_RowsCount == graphMatrix.getNrOfColumns())
    {
        mNodesCount = c_RowsCount;
        mGraphMatrix = graphMatrix;
    }
    else
    {
        assert(false);
    }
}

void PrimEngine::_buildTreeFromGraph(bool isMinTree)
{
    NodeInfoMap nodeInfoMap;

    if (mNodesCount > 0)
    {
        const Cost c_InitialNodeCost{isMinTree ? scMaxCost : scMinCost};

        auto isNewCostBetter{isMinTree ? [](Cost newCost, Cost oldCost) { return newCost <= oldCost; }
                                       : [](Cost newCost, Cost oldCost) { return newCost >= oldCost; }};

        nodeInfoMap.resize(mNodesCount, c_InitialNodeCost);

        Node currentNode{0};
        size_t addedNodesCount{1};
        nodeInfoMap.at(currentNode).mIsAddedToTree = true;

        while (addedNodesCount < mNodesCount)
        {
            // after adding the new node to tree the costs of its neighbors (that haven't been added yet) need to be
            // updated
            _updateNeighborCosts(currentNode, nodeInfoMap, isNewCostBetter);

            Node nextNode{currentNode};
            Cost nextNodeCost{c_InitialNodeCost};

            // the next node to be added is chosen from the pool of nodes that haven't been added to tree yet (NOT
            // necesarily neighbors of current node)
            for (Node node{0}; node < nodeInfoMap.size(); ++node)
            {
                if (!nodeInfoMap.at(node).mIsAddedToTree &&
                    isNewCostBetter(nodeInfoMap.at(node).mNodeCost, nextNodeCost))
                {
                    nextNode = node;
                    nextNodeCost = nodeInfoMap.at(node).mNodeCost;
                }
            }

            currentNode = nextNode;
            nodeInfoMap.at(currentNode).mIsAddedToTree = true;
            ++addedNodesCount;
        }

        _doBuildTree(nodeInfoMap, isMinTree);
    }
    else
    {
        assert(false);
    }
}

void PrimEngine::_doBuildTree(const NodeInfoMap& nodeInfoMap, bool isMinTree)
{
    Tree& tree{isMinTree ? mMinTree : mMaxTree};

    assert(tree.empty());

    const size_t c_NodesCount{nodeInfoMap.size()};

    if (c_NodesCount > 1)
    {
        for (Node node{1}; node < c_NodesCount; ++node)
        {
            const Node currentPrecedingNode{nodeInfoMap.at(node).mPrecedingNode};

            // handle incomplete tree scenario (graph is broken into separate parts) and only take nodes with concrete
            // predecessor into account
            if (currentPrecedingNode < mNodesCount)
            {
                tree.push_back(Edge{currentPrecedingNode, node});
            }
        }
    }
}

void PrimEngine::_updateNeighborCosts(Node node, NodeInfoMap& nodeInfoMap,
                                      bool (*isNewCostBetter)(Cost newCost, Cost oldCost))
{
    for (GraphMatrix::ConstZIterator it{mGraphMatrix.constZRowBegin(node)}; it != mGraphMatrix.constZRowEnd(node); ++it)
    {
        if (*it > 0)
        {
            const Node c_Neighbor{*it.getColumnNr()};

            if (isNewCostBetter(mGraphMatrix.at(node, c_Neighbor), nodeInfoMap.at(c_Neighbor).mNodeCost) &&
                !nodeInfoMap.at(c_Neighbor).mIsAddedToTree)
            {
                nodeInfoMap.at(c_Neighbor).mNodeCost = mGraphMatrix.at(node, c_Neighbor);
                nodeInfoMap.at(c_Neighbor).mPrecedingNode = node;
            }
        }
    }
}

void PrimEngine::_reset()
{
    BaseEngine::_reset();
    mGraphMatrix.clear();
}

PrimEngine::NodeInfo::NodeInfo(Cost cost)
    : mNodeCost{cost}
    , mPrecedingNode{scNullNode}
    , mIsAddedToTree{false}
{
}
