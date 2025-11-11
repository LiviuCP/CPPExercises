#pragma once

#include <string>

#include "graphdatatypes.h"

class BaseEngine
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
