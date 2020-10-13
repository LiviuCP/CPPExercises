#include <iostream>
#include <vector>
#include <string>

#include "binarysearchtree.h"
#include "treetesting.h"

#define BINARY_SEARCH_TREE

using namespace std;

int main()
{
#ifdef BINARY_SEARCH_TREE
    TEST_TREE(BinarySearchTree);
#endif
    return 0;
}
