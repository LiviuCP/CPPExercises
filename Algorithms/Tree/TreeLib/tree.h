#ifndef TREE_H
#define TREE_H

#include <memory>

#include "node.h"

class Tree;
using TreeSp = std::shared_ptr<Tree>;

class Tree
{
public:
    explicit Tree() = default;
    explicit Tree(NodeValue rootValue);

    bool addRootNode(NodeValue value);

    void invertRecursively();
    void invertIteratively();

    void clear();

    NodeValues getNodeValues() const;
    NodeWp getRootNode() const;
    size_t size() const;
    bool empty() const;

protected:
    std::vector<ConstNodeSp> _getFlattenedStructure() const;

private:
    NodeSp m_Root;
};



#endif // TREE_H
