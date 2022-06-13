#include "baseengine.h"

BaseEngine::BaseEngine(const std::string engineName)
    : mNodesCount{0u}
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
    mNodesCount = 0u;
    mMinTree.clear();
    mMaxTree.clear();
}
