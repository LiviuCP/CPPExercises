#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>

namespace TestUtils
{
    bool areExpectedTreeValuesMet(const std::string& treeString, int treeSize, const std::string& expectedTreeString, int expectedTreeSize);

    static const std::string scDefaultValue{"DF"};
    static const std::string scDefaultNullValue{""};
    static const std::string scCustomNullValue{"NULL"};
    static const std::string scEmptyTreeString{""};
}

#endif // TESTUTILS_H
