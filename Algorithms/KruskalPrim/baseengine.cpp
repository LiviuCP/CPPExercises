module;

#include <list>
#include <string>
#include <utility>

export module baseengine;
export import matrix;

using Node = size_t;
using Edge = std::pair<Node, Node>;
using Cost = size_t;
using GraphMatrix = Matrix<Cost>;
using Tree = std::list<Edge>;

export class BaseEngine
{
public:
    virtual bool buildTrees(const GraphMatrix& graphMatrix) = 0;

    const Tree& getMinTree() const;
    const Tree& getMaxTree() const;

    std::string getName() const;

protected:
    explicit BaseEngine(const std::string engineName = "");
    virtual void _reset();

    size_t mNodesCount;

    Tree mMinTree;
    Tree mMaxTree;

    std::string mName;
};

BaseEngine::BaseEngine(const std::string engineName)
    : mNodesCount{0}
    , mName{engineName}
{
}

const Tree& BaseEngine::getMinTree() const
{
    return mMinTree;
}

const Tree& BaseEngine::getMaxTree() const
{
    return mMaxTree;
}

std::string BaseEngine::getName() const
{
    return mName;
}

void BaseEngine::_reset()
{
    mNodesCount = 0;
    mMinTree.clear();
    mMaxTree.clear();
}
