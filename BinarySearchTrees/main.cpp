#include <iostream>
#include <vector>
#include <string>

#include "binarysearchtree.h"

using namespace std;

int main()
{
    const std::string c_InitialValue{"initial"};
    const std::string c_NullValue{"none"};
    const std::string c_ChangedValue{"changed"};
    const std::vector<int> nodesBase{10, -2, 4, 3, -5, 8, 9, 7, -8, 0, 2, 5, 4};

    cout << "1) Let's create a binary search tree from an array" << endl << endl;
    cout << "Array is: ";

    for (std::vector<int>::const_iterator it{nodesBase.cbegin()}; it != nodesBase.cend(); ++it)
    {
        cout << *it << " ";
    }

    cout << endl << endl;

    SimpleBST simpleBST{nodesBase, c_InitialValue, c_NullValue};

    cout << endl << "Obtained tree (all nodes with value \"" << c_InitialValue << "\") is: " << endl << endl;
    simpleBST.printNodesInfo();
    cout << endl;

    cout << "2) We'll set a new value for node 3 and then print values for nodes 3, -8 and 7" << endl << endl;
    (void)simpleBST.addOrUpdateNode(3, c_ChangedValue);
    cout << "Node 3: " << "\"" << simpleBST.getNodeValue(3) << "\"" << endl;
    cout << "Node -8: " << "\"" << simpleBST.getNodeValue(-8) << "\"" << endl;
    cout << "Node -7: " << "\"" << simpleBST.getNodeValue(-7) << "\"" << endl;

    cout << endl << "3) Now we'll update node 7 with the null value" << endl;
    (void)simpleBST.addOrUpdateNode(7, c_NullValue);
    cout << "Node has now the same value as before: "  << "\"" << simpleBST.getNodeValue(7) << "\"" << endl << endl;
    cout << "4) Now we'll remove three nodes: 4, 10, 9, -5 and -17" << endl << endl;
    (void)simpleBST.deleteNode(4); // this causes the issue (-5 is considered ROOT)
    (void)simpleBST.deleteNode(10);
    (void)simpleBST.deleteNode(9);
    (void)simpleBST.deleteNode(-5);
    const bool success{simpleBST.deleteNode(-17)};
    cout << "New tree is: " << endl << endl;
    simpleBST.printNodesInfo();
    if (!success)
    {
        clog << endl << "Warning: no node -17 to be removed" << endl << endl;
    }

    cout << "5) Now we'll test some move/copy operations" << endl << endl;

    cout << "5a) First we'll create two new nodes based on original one, one with copy and one with move constructor" << endl << endl;
    SimpleBST simpleBSTCopy{simpleBST};
    cout << "Tree created with copy constructor: " << endl << endl;
    simpleBSTCopy.printNodesInfo();
    SimpleBST simpleBSTMove{std::move(simpleBST)};
    cout << endl << "Tree created with move constructor:" << endl << endl;
    simpleBSTMove.printNodesInfo();
    cout << endl << "Original tree after move operation: " << endl;
    simpleBST.printNodesInfo();

    cout << endl << "5b) Move assigning the move created tree to original tree" << endl << endl;
    simpleBST = std::move(simpleBSTMove);
    cout << "Tree created with move constructor" << endl;
    simpleBSTMove.printNodesInfo();
    cout << endl << "Original tree: " << endl << endl;
    simpleBST.printNodesInfo();

    cout << endl << "5c) Copy assigning the original tree to move created tree" << endl << endl;
    simpleBSTMove = simpleBST;
    cout << "Tree created with move constructor:" << endl << endl;
    simpleBSTMove.printNodesInfo();
    cout << endl << "Original tree: " << endl << endl;
    simpleBST.printNodesInfo();

    cout << endl << "5d) Finally we'll create a small tree and copy assign it to original and move assign it to move created tree" << endl << endl;
    SimpleBST newBST{std::vector<int>{2, 1, 3}, c_InitialValue, c_NullValue};
    simpleBST = newBST;
    simpleBSTMove = std::move(newBST);
    cout << "Tree created with move constructor:" << endl << endl;
    simpleBSTMove.printNodesInfo();
    cout << endl << "Original tree: " << endl << endl;
    simpleBST.printNodesInfo();
    cout << endl << "New tree: " << endl;
    newBST.printNodesInfo();

    cout << endl << "That's all folks!" << endl << endl;

    return 0;
}
