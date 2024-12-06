#include "testutils.h"

bool TestUtils::areExpectedTreeValuesMet(const std::string& treeString, size_t treeSize, const std::string& expectedTreeString, size_t expectedTreeSize)
{
    return treeString == expectedTreeString && treeSize == expectedTreeSize;
}
