#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>

class Node;
using NodeSp = std::shared_ptr<Node>;
using NodeWp = std::weak_ptr<Node>;
using ConstNodeSp = std::shared_ptr<const Node>;
using NodeValue = int;
using NodeValues = std::vector<NodeValue>;

class Node
{
public:
    void createAndAppendChildren(const NodeValues& childValues);

    NodeWp getChildAtIndex(size_t childIndex) const;
    NodeValue getValue() const;

protected:
    friend class Tree;

    static NodeSp create(NodeValue value);
    explicit Node(NodeValue value);

    void _invertSuccessorsRecursively();
    void _invertSuccessorsIteratively();

    std::vector<ConstNodeSp> _getFlattenedSuccessors() const;

private:
    NodeValue m_Value;
    std::vector<std::shared_ptr<Node>> m_Children;
};

#endif // NODE_H
