#include "kruskaltree.h"

KruskalTree::KruskalTree()
    : mNodesCount{0u}
{
}

bool KruskalTree::build(const GraphMatrix& graphMatrix)
{
    bool success{false};

    _reset();

    const int c_RowsCount{graphMatrix.getNrOfRows()};

    if (c_RowsCount > 0 && c_RowsCount == graphMatrix.getNrOfColumns())
    {
        _buildGraph(graphMatrix);
        _buildTreeFromGraph();

        success = true;
    }

    return success;
}

const EdgeList& KruskalTree::getTreeEdges() const
{
    return mTreeEdges;
}

void KruskalTree::_buildGraph(const Matrix<int>& edgeCostsMatrix)
{
    const int c_RowsCount{edgeCostsMatrix.getNrOfRows()};

    assert(c_RowsCount > 0 && c_RowsCount == edgeCostsMatrix.getNrOfColumns());

    mNodesCount = static_cast<size_t>(c_RowsCount);

    for (int row{0}; row < c_RowsCount - 1; ++row)
    {
        for (Matrix<int>::ConstZIterator it{edgeCostsMatrix.getConstZIterator(row, row + 1)}; it != edgeCostsMatrix.constZRowEnd(row); ++it)
        {
            if (*it != 0)
            {
                mEdgeCostsMap.insert(std::pair<Cost, Edge>{*it, Edge{it.getRowNr(), it.getColumnNr()}});
            }
        }
    }
}

/* Add the nodes to components and merge the components together until only one component remains which contains all nodes.

   For each edge 3 cases should be considered:
   a) one of the nodes has no component (orphan): add it to the component of the other node
   b) none of the nodes is assigned to a component: add both to the component corresponding to the lower numbered node
   c) both nodes have components: merge the smaller component into the larger one

   The goal is to avoid creating any loops within minimum cost tree by ensuring only edges containing nodes from different components (or no component) are appended to it.
   Edges should be appended in increasing cost size order to ensure a minimum total cost.
*/
void KruskalTree::_buildTreeFromGraph()
{
    assert(mNodesCount > 0u);

    _buildEmptyComponents();

    size_t requiredEdgesCount{mNodesCount - 1};

    for (EdgeCostsMap::const_iterator it{mEdgeCostsMap.cbegin()}; it != mEdgeCostsMap.cend() && requiredEdgesCount > 0u; ++it)
    {
        bool edgeFound{false};
        Edge edge{it->second};

        if (mComponentNumbers[edge.first] != mComponentNumbers[edge.second])
        {
            if (scNullComponent == mComponentNumbers[edge.first])
            {
                _bindOrphanNodeToNonOrphanNode(edge.first, edge.second);
            }
            else if (scNullComponent == mComponentNumbers[edge.second])
            {
                _bindOrphanNodeToNonOrphanNode(edge.second, edge.first);
            }
            else if (mComponents[mComponentNumbers[edge.first]].size() >= mComponents[mComponentNumbers[edge.second]].size())
            {
                _mergeComponents(edge.first, edge.second);
            }
            else
            {
                _mergeComponents(edge.second, edge.first);
            }

            edgeFound = true;
        }
        else
        {
            if (scNullComponent == mComponentNumbers[edge.first])
            {
                _bindOrphanNodes(edge.first, edge.second);
                edgeFound = true;
            }
        }

        if (edgeFound)
        {
            mTreeEdges.push_back(edge);
            --requiredEdgesCount;
        }
    }
}

/* Empty components (one per node) are created. However initially no node "belongs" to its component but instead is considered standalone ("orphan")
   Nodes would then be progressively added to components and these ones are merged until finally a single component representing the Kruskal tree remains.
*/
void KruskalTree::_buildEmptyComponents()
{
    mComponents.resize(mNodesCount);
    mComponentNumbers.resize(mNodesCount);

    for (Node currentNodeNumber{0u}; currentNodeNumber < mNodesCount; ++currentNodeNumber)
    {
        mComponentNumbers.at(currentNodeNumber) = scNullComponent;
    }
}

void KruskalTree::_reset()
{
    mNodesCount = 0;
    mEdgeCostsMap.clear();
    mComponents.clear();
    mComponentNumbers.clear();
    mTreeEdges.clear();
}

void KruskalTree::_bindOrphanNodeToNonOrphanNode(Node orphanNode, Node componentNode)
{
    mComponentNumbers[orphanNode] = mComponentNumbers[componentNode];
    mComponents[mComponentNumbers[componentNode]].push_back(orphanNode);
}

void KruskalTree::_bindOrphanNodes(Node firstOrphanNode, Node secondOrphanNode)
{
    // add both to component corresponding to lowest numbered node (this is edge.first as edges are increasingly ordered: first < second)
    assert(firstOrphanNode < secondOrphanNode);

    mComponents[firstOrphanNode].push_back(firstOrphanNode);
    mComponents[firstOrphanNode].push_back(secondOrphanNode);
    mComponentNumbers[firstOrphanNode] = firstOrphanNode;
    mComponentNumbers[secondOrphanNode] = firstOrphanNode;
}

void KruskalTree::_mergeComponents(Node enclosingComponentNode, Node mergedComponentNode)
{
    size_t c_MergedComponentNumber{mComponentNumbers[mergedComponentNode]};

    for (const auto& node : mComponents[c_MergedComponentNumber])
    {
        mComponentNumbers[node] = mComponentNumbers[enclosingComponentNode];
        mComponents[mComponentNumbers[enclosingComponentNode]].push_back(node);
    }

    mComponents[c_MergedComponentNumber].clear();
}
