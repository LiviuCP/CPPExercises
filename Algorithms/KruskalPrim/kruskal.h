#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <vector>
#include <map>
#include <limits>

#include "baseengine.h"

class KruskalEngine : public BaseEngine
{
public:
    explicit KruskalEngine();
    bool buildTrees(const GraphMatrix& graphMatrix) override;

private:
    using Component = std::list<Node>;
    using EdgeCostsMap = std::multimap<Cost, Edge>;

    void _buildGraph(const GraphMatrix& graphMatrix);
    void _buildMinTreeFromGraph();
    void _buildMaxTreeFromGraph();
    void _buildEmptyComponents();
    void _reset() override;

    bool _addEdgeToTree(const Edge& edge);
    void _bindOrphanNodeToNonOrphanNode(Node orphanNode, Node componentNode);
    void _bindOrphanNodes(Node firstOrphanNode, Node secondOrphanNode);
    void _mergeComponents(Node enclosingComponentNode, Node mergedComponentNode);

    static constexpr size_t scNullComponent{std::numeric_limits<size_t>::max()};

    EdgeCostsMap mEdgeCostsMap;
    std::vector<Component> mComponents;
    std::vector<size_t> mComponentNumbers;
};

#endif // KRUSKAL_H
