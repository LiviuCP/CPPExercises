#include <iostream>
#include <vector>
#include <string>

#include "redblacktree.h"
#include "avltree.h"
#include "utils.h"

using namespace std;
using IntStrBinarySearchTree = BinarySearchTree<int, std::string>;
using IntStrRedBlackTree = RedBlackTree<int, std::string>;
using IntStrAVLTree = AVLTree<int, std::string>;

int main()
{
    const vector<int> c_Keys{-2, 5, 4, 3, 1, 10, 11, 12, 8, 15, -4, 2, 9, 7, -14, 0};
    const string c_DefaultValue{"default"};

    Utilities::clearScreen();

    cout << "We will construct three binary search trees by using an array of keys" << endl << endl;

    cout << "The array is: ";

    for (const auto& key : c_Keys)
    {
        cout << key << " ";
    }

    cout << endl << endl;

    cout << "First, a simple (unbalanced) search tree: " << endl << endl;

    const IntStrBinarySearchTree c_BinarySearchTree{c_Keys, c_DefaultValue};
    c_BinarySearchTree.printTree();

    cout << endl << "Second, a red-black (balanced) search tree:" << endl << endl;

    const IntStrRedBlackTree c_RedBlackTree{c_Keys, c_DefaultValue};
    c_RedBlackTree.printTree();

    cout << endl << "Third, an AVL (balanced) search tree:" << endl << endl;

    const IntStrAVLTree c_AVLTree{c_Keys, c_DefaultValue};
    c_AVLTree.printTree();

    cout << endl << "That's all folks!" << endl << endl;

    return 0;
}
