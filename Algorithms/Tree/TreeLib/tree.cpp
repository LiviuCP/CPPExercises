#include <cassert>

#include "tree.h"

Tree::Tree(NodeValue rootValue)
    : m_Root{Node::create(rootValue)}
{
}

bool Tree::addRootNode(NodeValue value)
{
    bool added = false;

    if (!m_Root)
    {
        m_Root = Node::create(value);
        added = true;
    }

    return added;
}

void Tree::invertRecursively()
{
    if (m_Root)
    {
        m_Root->_invertSuccessorsRecursively();
    }
}

void Tree::invertIteratively()
{
    if (m_Root)
    {
        m_Root->_invertSuccessorsIteratively();
    }
}

void Tree::clear()
{
    if (m_Root)
    {
        m_Root.reset();
    }
}

NodeValues Tree::getNodeValues() const
{
    NodeValues nodeValues;

    for (const auto& node : _getFlattenedStructure())
    {
        if (!node)
        {
            assert(false);
            break;
        }

        nodeValues.emplace_back(node->m_Value);
    }

    return nodeValues;
}

NodeWp Tree::getRootNode() const
{
    return m_Root;
}

size_t Tree::size() const
{
    return _getFlattenedStructure().size();
}

bool Tree::empty() const
{
    return !m_Root;
}

std::vector<ConstNodeSp> Tree::_getFlattenedStructure() const
{
    std::vector<ConstNodeSp> result;

    if (m_Root)
    {
        result.emplace_back(m_Root);

        for (const auto& node : m_Root->_getFlattenedSuccessors())
        {
            result.emplace_back(node);
        }
    }

    return result;
}
