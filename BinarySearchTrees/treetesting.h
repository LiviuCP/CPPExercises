#ifndef TREETESTING_H
#define TREETESTING_H

#define TEST_TREE(SearchTreeType) \
{ \
    const std::string c_InitialValue{"initial"}; \
    const std::string c_NullValue{"none"}; \
    const std::string c_ChangedValue{"changed"}; \
    const std::string c_SecondChangedValue{"changed again"}; \
    const std::vector<int> nodesBase{10, -2, 4, 3, -5, 8, 9, 7, -8, 0, 2, 5, 4}; \
                                                                                 \
    cout << "1) Let's create a binary search tree from an array" << endl << endl; \
    cout << "Array is: "; \
                            \
    for (std::vector<int>::const_iterator it{nodesBase.cbegin()}; it != nodesBase.cend(); ++it) \
    { \
        cout << *it << " "; \
    } \
        \
    cout << endl << endl; \
                            \
    SearchTreeType searchTree{nodesBase, c_InitialValue, c_NullValue}; \
                                                                            \
    cout << endl << "Obtained tree (all nodes with value \"" << c_InitialValue << "\") is: " << endl << endl; \
    searchTree.printNodesInfo(); \
    cout << endl << "Tree has " << searchTree.getSize() << " nodes" << endl << endl; \
                                                                                      \
    cout << "2) We'll set a new value for node 3 and then print values for nodes 3, -8 and 7" << endl << endl; \
    (void)searchTree.addOrUpdateNode(3, c_ChangedValue); \
    cout << "Node 3: " << "\"" << searchTree.getNodeValue(3) << "\"" << endl; \
    cout << "Node -8: " << "\"" << searchTree.getNodeValue(-8) << "\"" << endl; \
    cout << "Node -7: " << "\"" << searchTree.getNodeValue(-7) << "\"" << endl; \
    cout << endl << "Tree has size: " << searchTree.getSize() << endl; \
                                                                        \
    cout << endl << "3) Now we'll update node 7 with the null value" << endl; \
    (void)searchTree.addOrUpdateNode(7, c_NullValue); \
    cout << "Node has now the same value as before: "  << "\"" << searchTree.getNodeValue(7) << "\"" << endl; \
    cout << endl << "Tree has size: " << searchTree.getSize() << endl << endl; \
    cout << "4) Now we'll remove some nodes: 4, 10, 9, -5 and -17" << endl << endl; \
    (void)searchTree.deleteNode(4); \
    (void)searchTree.deleteNode(10); \
    (void)searchTree.deleteNode(9); \
    (void)searchTree.deleteNode(-5); \
    const bool success{searchTree.deleteNode(-17)}; \
    cout << "Updated tree is: " << endl << endl; \
    searchTree.printNodesInfo(); \
    cout << endl << "It has size: " << searchTree.getSize() << endl; \
    if (!success) \
    { \
        clog << endl << "Warning: no node -17 to be removed" << endl << endl; \
    } \
        \
    cout << "5) Now we'll test some move/copy operations" << endl << endl; \
                                                                            \
    cout << "5a) First we'll create two new nodes based on original one, one with copy and one with move constructor" << endl << endl; \
    SearchTreeType searchTreeCopy{searchTree}; \
    cout << "Tree created with copy constructor: " << endl << endl; \
    searchTreeCopy.printNodesInfo(); \
    SearchTreeType searchTreeMove{std::move(searchTree)}; \
    cout << endl << "Tree created with move constructor:" << endl << endl; \
    searchTreeMove.printNodesInfo(); \
    cout << endl << "Original tree after move operation: " << endl; \
    searchTree.printNodesInfo(); \
    cout << endl << "Original tree size: " << searchTree.getSize() << endl; \
    cout << "Size of move constructor created tree: " << searchTreeMove.getSize() << endl; \
    cout << "Size of copy constructor created tree: " << searchTreeCopy.getSize() << endl; \
                                                                                           \
    cout << endl << "5b) Move assigning the move created tree to original tree" << endl << endl; \
    searchTree = std::move(searchTreeMove); \
    cout << "Tree created with move constructor" << endl; \
    searchTreeMove.printNodesInfo(); \
    cout << endl << "Original tree: " << endl << endl; \
    searchTree.printNodesInfo(); \
    cout << endl << "Original tree size: " << searchTree.getSize() << endl; \
    cout << "Size of move constructor created tree: " << searchTreeMove.getSize() << endl; \
                                                                                            \
    cout << endl << "5c) Copy assigning the original tree to move created tree" << endl << endl; \
    searchTreeMove = searchTree; \
    cout << "Tree created with move constructor:" << endl << endl; \
    searchTreeMove.printNodesInfo(); \
    cout << endl << "Original tree: " << endl << endl; \
    searchTree.printNodesInfo(); \
    cout << endl << "Original tree size: " << searchTree.getSize() << endl; \
    cout << "Size of move constructor created tree: " << searchTreeMove.getSize() << endl; \
                                                                                           \
    cout << endl << "5d) We'll now create a small tree and copy assign it to original and move assign it to move created tree" << endl << endl; \
    SearchTreeType newSearchTree{std::vector<int>{2, 1, 3}, c_InitialValue, c_NullValue}; \
    searchTree = newSearchTree; \
    searchTreeMove = std::move(newSearchTree); \
    cout << "Tree created with move constructor:" << endl << endl; \
    searchTreeMove.printNodesInfo(); \
    cout << endl << "Original tree: " << endl << endl; \
    searchTree.printNodesInfo(); \
    cout << endl << "New tree: " << endl; \
    newSearchTree.printNodesInfo(); \
    cout << endl << "Original tree size: " << searchTree.getSize() << endl; \
    cout << "Size of move constructor created tree: " << searchTreeMove.getSize() << endl; \
    cout << "New tree size: " << newSearchTree.getSize() << endl; \
    if (searchTree == searchTreeMove) \
    { \
        cout << endl << "The original tree and the tree created with move constructor are equal" << endl; \
    } \
    else \
    { \
        cout << endl << "The original tree and the tree created with move constructor are not equal" << endl; \
    } \
      \
    cout << endl << "5e) Now we'll add one node to each of the original tree and new tree" << endl; \
    searchTree.addOrUpdateNode(25, c_InitialValue); \
    newSearchTree.addOrUpdateNode(20, c_InitialValue); \
    cout << endl << "Original tree: " << endl << endl; \
    searchTree.printNodesInfo(); \
    cout << endl << "New tree: " << endl; \
    newSearchTree.printNodesInfo(); \
    cout << endl << "Original tree size: " << searchTree.getSize() << endl; \
    cout << "New tree size: " << newSearchTree.getSize() << endl; \
                                                                  \
    cout << endl << "6) Finally let's do a merge of two trees" << endl; \
    SearchTreeType firstTree{std::vector<int>{2, -5, 9, 7}, c_InitialValue}, secondTree{std::vector<int>{14, 3, -2, 8, 4}, c_InitialValue}; \
    firstTree.addOrUpdateNode(17, c_ChangedValue); \
    secondTree.addOrUpdateNode(17, c_SecondChangedValue); \
    SearchTreeType firstTreeCopy{firstTree}, secondTreeCopy{secondTree}; \
    cout << endl << "First tree: " << endl << endl; \
    firstTree.printNodesInfo(); \
    cout << endl << "Value of node 17 in first tree is: " << firstTree.getNodeValue(17) << endl; \
    cout << endl << "Second tree: " << endl << endl; \
    secondTree.printNodesInfo(); \
    cout << endl << "Value of node 17 in second tree is: " << secondTree.getNodeValue(17) << endl; \
    cout << endl << "First tree size: " << firstTree.getSize() << endl; \
    cout << "Second tree size: " << secondTree.getSize() << endl; \
                                                                    \
    cout << endl << "6a) Merge second tree into the first" << endl; \
    firstTree.mergeTree(secondTree); \
    cout << endl << "First tree is now: " << endl << endl; \
    firstTree.printNodesInfo(); \
    cout << endl << "Value of node 17 in merged tree is: " << firstTree.getNodeValue(17) << endl; \
    cout << endl << "Second tree is now: " << endl; \
    secondTree.printNodesInfo(); \
    cout << endl << "First tree size: " << firstTree.getSize() << endl; \
    cout << "Second tree size: " << secondTree.getSize() << endl; \
                                                                  \
    cout << endl << "6b) Merge first tree into the second" << endl; \
    secondTreeCopy.mergeTree(firstTreeCopy); \
    cout << endl << "First tree is now: " << endl; \
    firstTreeCopy.printNodesInfo(); \
    cout << endl << "Second tree is now: " << endl; \
    secondTreeCopy.printNodesInfo(); \
    cout << endl << "Value of key 17 in merged tree is: " << secondTreeCopy.getNodeValue(17) << endl; \
    cout << endl << "First tree size: " << firstTreeCopy.getSize() << endl; \
    cout << "Second tree size: " << secondTreeCopy.getSize() << endl; \
                                                                      \
    if (firstTree != secondTreeCopy) \
    { \
        cout << endl << "The merged tree versions are not equal" << endl; \
    } \
    else \
    { \
        cout << endl << "The merged tree versions are equal" << endl; \
    } \
      \
    cout << endl << "That's all folks!" << endl << endl; \
}

#endif // TREETESTING_H
