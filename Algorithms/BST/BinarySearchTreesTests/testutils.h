#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>

class BinarySearchTree;

namespace TestUtils
{
    bool areExpectedTreeValuesMet(const BinarySearchTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered = false);

    static const std::string scDefaultValue{"DF"};
    static const std::string scDefaultNullValue{""};
    static const std::string scCustomNullValue{"NULL"};
    static const std::string scEmptyTreeString{""};
}

#endif // TESTUTILS_H
