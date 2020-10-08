#include <iostream>
#include <vector>
#include <string>

#include "binarysearchtree.h"

using namespace std;

int main()
{
    const std::string c_Value{"initial value"};
    const std::string c_NullValue{"no value (no node)"};
    SimpleBST simpleBST{c_NullValue};
    const std::vector<int> nodesBase{10, -2, 4, 3, -5, 8, 9, 7, -8, 0, 2, 5, 4};

    cout << "Let's create a binary search tree from an array" << endl << endl;
    cout << "Array is: ";

    for (std::vector<int>::const_iterator it{nodesBase.cbegin()}; it != nodesBase.cend(); ++it)
    {
        cout << *it << " ";
        simpleBST.addOrUpdateNode(*it, c_Value);
    }

    cout << endl << endl;

    cout << "Obtained tree (all nodes with value \"initial value\") is: " << endl << endl;
    simpleBST.printNodesInfo();
    cout << endl;

    cout << "We'll set a new value for node 3" << endl << endl;
    (void)simpleBST.addOrUpdateNode(3, "another value");

    cout << "Now we'll print values for nodes 3, -8 and -7" << endl << endl;
    cout << "Node 3: " << "\"" << simpleBST.getNodeValue(3) << "\"" << endl;
    cout << "Node -8: " << "\"" << simpleBST.getNodeValue(-8) << "\"" << endl;
    cout << "Node -7: " << "\"" << simpleBST.getNodeValue(-7) << "\"" << endl;

    cout << endl << "Finally we'll update node 7 with the null value" << endl;
    (void)simpleBST.addOrUpdateNode(7, c_NullValue);
    cout << "Node has now the same value as before: "  << "\"" << simpleBST.getNodeValue(7) << "\"" << endl;
    cout << endl << "That's all folks!" << endl << endl;

    return 0;
}
