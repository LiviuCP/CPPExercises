#include "testutils.h"

bool TestUtils::areExpectedTreeValuesMet(const std::string &treeString, int treeSize, const std::string &expectedTreeString, int expectedTreeSize)
{
    return treeString == expectedTreeString && treeSize == expectedTreeSize;
}
