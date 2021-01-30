#include "binarysearchtree.h"
#include "testutils.h"

bool TestUtils::areExpectedTreeValuesMet(const BinarySearchTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered)
{
    bool areValuesMet{false};

    if (searchTree != nullptr)
    {
        areValuesMet = (treeString == searchTree->getTreeAsString(areNodeValuesConsidered) &&
                        treeSize == searchTree->getSize());
    }

    return areValuesMet;
}
