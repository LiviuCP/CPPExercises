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

    cout << "Let's create a binary search tree from an array" << endl << endl;
    cout << "Array is: ";

    for (std::vector<int>::const_iterator it{nodesBase.cbegin()}; it != nodesBase.cend(); ++it)
    {
        cout << *it << " ";
    }

    cout << endl << endl;

    SimpleBST simpleBST{nodesBase, c_InitialValue, c_NullValue};

    cout << "Obtained tree (all nodes with value \"" << c_InitialValue << "\") is: " << endl << endl;
    simpleBST.printNodesInfo();
    cout << endl;

    cout << "We'll set a new value for node 3" << endl << endl;
    (void)simpleBST.addOrUpdateNode(3, c_ChangedValue);

    cout << "Now we'll print values for nodes 3, -8 and -7" << endl << endl;
    cout << "Node 3: " << "\"" << simpleBST.getNodeValue(3) << "\"" << endl;
    cout << "Node -8: " << "\"" << simpleBST.getNodeValue(-8) << "\"" << endl;
    cout << "Node -7: " << "\"" << simpleBST.getNodeValue(-7) << "\"" << endl;

    cout << endl << "Now we'll update node 7 with the null value" << endl;
    (void)simpleBST.addOrUpdateNode(7, c_NullValue);
    cout << "Node has now the same value as before: "  << "\"" << simpleBST.getNodeValue(7) << "\"" << endl << endl;
    cout << "Finally we'll remove three nodes: 4, 10, 9, -5 and -17" << endl << endl;
    (void)simpleBST.deleteNode(4); // this causes the issue (-5 is considered ROOT)
    (void)simpleBST.deleteNode(10);
    (void)simpleBST.deleteNode(9);
    (void)simpleBST.deleteNode(-5);
    const bool success{simpleBST.deleteNode(-17)};
    cout << "New tree is: " << endl << endl;
    simpleBST.printNodesInfo();
    if (!success)
    {
        clog << endl << "Warning! No node -17 to be removed" << endl << endl;
    }
    cout << "That's all folks!" << endl << endl;

    return 0;
}
