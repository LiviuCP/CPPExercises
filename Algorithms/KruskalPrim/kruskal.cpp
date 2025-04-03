#include "kruskal.h"
#include "datautils.h"

KruskalEngine::KruskalEngine()
    : BaseEngine{"Kruskal"}
{
}

bool KruskalEngine::buildTrees(const GraphMatrix& graphMatrix)
{
    bool success{false};

    _reset();

    const matrix_size_t c_RowsCount{graphMatrix.getNrOfRows()};

    if (c_RowsCount > 0 && c_RowsCount == graphMatrix.getNrOfColumns())
    {
        _buildGraph(graphMatrix);
        _buildMinTreeFromGraph();
        _buildMaxTreeFromGraph();

        success = true;
    }

    return success;
}

void KruskalEngine::_buildGraph(const GraphMatrix& graphMatrix)
{
    const matrix_size_t c_RowsCount{graphMatrix.getNrOfRows()};

    if (c_RowsCount > 0 && c_RowsCount == graphMatrix.getNrOfColumns())
    {
        mNodesCount = c_RowsCount;

        for (matrix_size_t row{0}; row < c_RowsCount - 1; ++row)
        {
            for (Matrix<Cost>::ConstZIterator it{graphMatrix.getConstZIterator(row, row + 1)}; it != graphMatrix.constZRowEnd(row); ++it)
            {
                if (*it != 0)
                {
                    // no need to check validity of getRowNr() / getColumnNr() output, the matrix is not empty and it is a forward iterator
                    mEdgeCostsMap.insert(std::pair<Cost, Edge>{*it, Edge{*it.getRowNr(), *it.getColumnNr()}});
                }
            }
        }
    }
    else
    {
        assert(false);
    }
}

/* Edges should be appended in increasing cost size order to ensure a minimum total cost.
*/
void KruskalEngine::_buildMinTreeFromGraph()
{
    if (mNodesCount > 0)
    {
        _buildEmptyComponents();

        size_t requiredEdgesCount{mNodesCount - 1};

        for (EdgeCostsMap::const_iterator it{mEdgeCostsMap.cbegin()}; it != mEdgeCostsMap.cend() && requiredEdgesCount > 0; ++it)
        {
            bool edgeAdded{_addEdgeToTree(it->second)};

            if (edgeAdded)
            {
                mMinTree.push_back(it->second);
                --requiredEdgesCount;
            }
        }
    }
    else
    {
        assert(false);
    }
}

void KruskalEngine::_buildMaxTreeFromGraph()
{
    assert(mNodesCount > 0);

    if (mNodesCount > 0)
    {
        _buildEmptyComponents();

        size_t requiredEdgesCount{mNodesCount - 1};

        for (EdgeCostsMap::const_reverse_iterator it{mEdgeCostsMap.crbegin()}; it != mEdgeCostsMap.crend() && requiredEdgesCount > 0; ++it)
        {
            bool edgeAdded{_addEdgeToTree(it->second)};

            if (edgeAdded)
            {
                mMaxTree.push_back(it->second);
                --requiredEdgesCount;
            }
        }
    }
}

/* Empty components (one per node) are created. However initially no node "belongs" to its component but instead is considered standalone ("orphan")
   Nodes would then be progressively added to components and these ones are merged until finally a single component representing the Kruskal tree remains.
*/
void KruskalEngine::_buildEmptyComponents()
{
    assert(mNodesCount > 0);

    if (mNodesCount > 0)
    {
        if (!mComponents.empty() || !mComponentNumbers.empty())
        {
            mComponents.clear();
            mComponentNumbers.clear();
        }

        mComponents.resize(mNodesCount);
        mComponentNumbers.resize(mNodesCount);

        for (Node currentNodeNumber{0}; currentNodeNumber < mNodesCount; ++currentNodeNumber)
        {
            mComponentNumbers.at(currentNodeNumber) = scNullComponent;
        }
    }
}

void KruskalEngine::_reset()
{
    BaseEngine::_reset();

    mEdgeCostsMap.clear();
    mComponents.clear();
    mComponentNumbers.clear();
}

/* Add the nodes to components and merge the components together until only one component remains which contains all nodes.

   For each edge 3 cases should be considered:
   a) one of the nodes has no component (orphan): add it to the component of the other node
   b) none of the nodes is assigned to a component: add both to the component corresponding to the lower numbered node
   c) both nodes have components: merge the smaller component into the larger one

   The goal is to avoid creating any loops within minimum cost tree by ensuring only edges containing nodes from different components (or no component) are appended to it.
*/
bool KruskalEngine::_addEdgeToTree(const Edge& edge)
{
    bool success{false};

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

        success = true;
    }
    else
    {
        if (scNullComponent == mComponentNumbers[edge.first])
        {
            _bindOrphanNodes(edge.first, edge.second);
            success = true;
        }
    }

    return success;
}

void KruskalEngine::_bindOrphanNodeToNonOrphanNode(Node orphanNode, Node componentNode)
{
    mComponentNumbers[orphanNode] = mComponentNumbers[componentNode];
    mComponents[mComponentNumbers[componentNode]].push_back(orphanNode);
}

/* add both nodes to component corresponding to lowest numbered node
   (this is edge.first as edges are increasingly ordered: first < second)
*/
void KruskalEngine::_bindOrphanNodes(Node firstOrphanNode, Node secondOrphanNode)
{
    mComponents[firstOrphanNode].push_back(firstOrphanNode);
    mComponents[firstOrphanNode].push_back(secondOrphanNode);
    mComponentNumbers[firstOrphanNode] = firstOrphanNode;
    mComponentNumbers[secondOrphanNode] = firstOrphanNode;
}

void KruskalEngine::_mergeComponents(Node enclosingComponentNode, Node mergedComponentNode)
{
    size_t c_MergedComponentNumber{mComponentNumbers[mergedComponentNode]};

    for (const auto& node : mComponents[c_MergedComponentNumber])
    {
        mComponentNumbers[node] = mComponentNumbers[enclosingComponentNode];
        mComponents[mComponentNumbers[enclosingComponentNode]].push_back(node);
    }

    mComponents[c_MergedComponentNumber].clear();
}
