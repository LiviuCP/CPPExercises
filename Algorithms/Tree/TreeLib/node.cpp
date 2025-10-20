#include <cassert>

#include "node.h"

Node::Node(NodeValue value)
    : m_Value{value}
{
}

void Node::createAndAppendChildren(const NodeValues& childValues)
{
    for (const auto& value : childValues)
    {
        const NodeSp node{Node::create(value)};
        m_Children.emplace_back(node);
    }
}

NodeWp Node::getChildAtIndex(size_t childIndex) const
{
    return childIndex < m_Children.size() ? m_Children[childIndex] : nullptr;
}

NodeValue Node::getValue() const
{
    return m_Value;
}

NodeSp Node::create(NodeValue value)
{
    // using new instead of std::make_shared as the latter one does not have access to protected Node constructor
    return std::shared_ptr<Node>{new Node{value}};
}

void Node::_invertSuccessorsRecursively()
{
    for (auto& child : m_Children)
    {
        if (!child)
        {
            assert(false);
            break;
        }

        child->_invertSuccessorsRecursively();
    }

    std::reverse(m_Children.begin(), m_Children.end());
}

void Node::_invertSuccessorsIteratively()
{
    std::reverse(m_Children.begin(), m_Children.end());
    std::vector<NodeSp> currentDepthNodes{m_Children.begin(), m_Children.end()};

    for (; !currentDepthNodes.empty();)
    {
        std::vector<NodeSp> newDepthNodes;

        for (auto& node : currentDepthNodes)
        {
            if (!node)
            {
                assert(false);
                currentDepthNodes.clear();
                break;
            }

            auto& children = node->m_Children;
            std::reverse(children.begin(), children.end());

            for (auto& child : children)
            {
                newDepthNodes.push_back(child);
            }
        }

        currentDepthNodes = std::move(newDepthNodes);
    }
}

std::vector<ConstNodeSp> Node::_getFlattenedSuccessors() const
{
    std::vector<ConstNodeSp> result{m_Children.cbegin(), m_Children.cend()};

    for (std::vector<ConstNodeSp> currentDepthNodes{result}; !currentDepthNodes.empty();)
    {
        std::vector<ConstNodeSp> newDepthNodes;

        for (auto& node : currentDepthNodes)
        {
            if (!node)
            {
                assert(false);
                currentDepthNodes.clear();
                break;
            }

            auto& children = node->m_Children;

            for (auto& child : children)
            {
                result.emplace_back(child);
                newDepthNodes.emplace_back(child);
            }
        }

        currentDepthNodes = std::move(newDepthNodes);
    }

    return result;
}
