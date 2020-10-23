#include <iostream>
#include <vector>
#include <string>

#include "binarysearchtree.h"
#include "redblacktree.h"

#include "treetesting.h"

#define SIMPLE_BST
//#define RED_BLACK_TREE

using namespace std;

int main()
{
#ifdef SIMPLE_BST
    TEST_TREE(BinarySearchTree);
#elif defined(RED_BLACK_TREE)
    TEST_TREE(RedBlackTree);
#else
//    to be added later
#endif
    return 0;
}
