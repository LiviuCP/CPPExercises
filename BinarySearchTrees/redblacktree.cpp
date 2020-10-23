#include <iostream>
#include <utility>

#include <cassert>

#include "redblacktree.h"

RedBlackTree::RedBlackTree(const std::string& defaultNullValue)
    : BinarySearchTree(defaultNullValue)
{
}

// we don't use the similar base BST class constructor in init list as we need to use another logic in building an unempty red-black tree (RB rules to be followed)
RedBlackTree::RedBlackTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& defaultNullValue)
    : BinarySearchTree(defaultNullValue)
{
    if (inputKeys.size() != 0 && defaultValue != defaultNullValue)
    {
        for (std::vector<int>::const_iterator it{inputKeys.cbegin()}; it != inputKeys.cend(); ++it)
        {
            RedBlackNode* addedNode{_doAddOrUpdateRBNode(*it, defaultValue)};
            if (addedNode == nullptr)
            {
                std::clog << "Warning: duplicate red-black tree key found: " << *it << std::endl;
            }
        }
    }
}

RedBlackTree::RedBlackTree(const RedBlackTree& sourceTree)
    : BinarySearchTree(sourceTree.m_DefaultNullValue)
{
    _copyRBTreeNodes(sourceTree);
}

RedBlackTree::RedBlackTree(RedBlackTree&& sourceTree)
{
    m_Root = sourceTree.m_Root;
    m_DefaultNullValue = sourceTree.m_DefaultNullValue;
    m_Size = sourceTree.m_Size;

    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

bool RedBlackTree::addOrUpdateNode(int key, const std::string& value)
{
    bool newNodeAdded{false};

    if (value != m_DefaultNullValue)
    {
        const RedBlackNode* addedNode{_doAddOrUpdateRBNode(key, value)};

        if (addedNode != nullptr)
        {
            newNodeAdded = true;
        }
    }

    return newNodeAdded;
}

bool RedBlackTree::deleteNode(int key)
{
    bool deleted{false};

    // no need for dynamic_cast as we only work with RedBlackNode node objects in the RB tree class (no mixed node types scenario) and the base class is not abstract
    RedBlackNode* nodeToDelete{static_cast<RedBlackNode*>(_findNode(key))};

    if (nodeToDelete != nullptr)
    {
        // for two-children node to be removed: the in-order successor content will be copied to the node; then the successor will be recursively deleted
        if (nodeToDelete->getLeftChild() != nullptr && nodeToDelete->getRightChild() != nullptr)
        {
            nodeToDelete->copyInOrderSuccessorKeyAndValue();
            nodeToDelete = static_cast<RedBlackNode*>(nodeToDelete->getInOrderSuccessor());
        }

        _removeNodeFromRBTree(nodeToDelete);

        delete nodeToDelete;
        nodeToDelete = nullptr;
        deleted = true;
    }

    return deleted;
}

void RedBlackTree::mergeTree(RedBlackTree& sourceTree)
{
    assert(m_DefaultNullValue == sourceTree.m_DefaultNullValue && "Default null values of trees don't match");

    if (this != &sourceTree)
    {
        _copyRBTreeNodes(sourceTree);
        sourceTree._deleteAllTreeNodes();
    }
}

RedBlackTree& RedBlackTree::operator=(const RedBlackTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _deleteAllTreeNodes();
        }

        m_DefaultNullValue = sourceTree.m_DefaultNullValue;
        _copyRBTreeNodes(sourceTree);
    }

    return *this;
}

RedBlackTree& RedBlackTree::operator=(RedBlackTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _deleteAllTreeNodes();
        }

        m_Root = sourceTree.m_Root;
        m_Size = sourceTree.m_Size;
        sourceTree.m_Root = nullptr;
        sourceTree.m_Size = 0;
        m_DefaultNullValue = sourceTree.m_DefaultNullValue;
    }

    return *this;
}

void RedBlackTree::printNodesInfo() const
{
    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<Node*>::const_iterator it{nodesArray.cbegin()}; it != nodesArray.cend(); ++it)
    {
        std::cout << "Node: " << (*it)->getKey();
        std::cout << "/ Colour: " << (static_cast<RedBlackNode*>(*it)->isBlack() ? "B" : "R");
        std::cout << " / Is child: " << ((*it)->isLeftChild() ? "L" : (*it)->isRightChild() ? "R" : "N");

        _printNodeRelativesInfo(*it);

        std::cout << std::endl;
    }

    if (nodesArray.size() == 0)
    {
        std::clog << "Warning: red-black tree has no nodes" << std::endl;
    }
}

RedBlackTree::RedBlackNode* RedBlackTree::_doAddOrUpdateRBNode(int key, const std::string& value)
{
    // first: add the RB node as per standard procedure for binary search trees
    RedBlackNode* addedNode{static_cast<RedBlackNode*>(BinarySearchTree::_doAddOrUpdateNode(key, value))};

    // second: apply the required transformations for enforcing the RB tree rules
    if (addedNode != nullptr)
    {
        RedBlackNode* currentNode{addedNode};
        RedBlackNode* parent{static_cast<RedBlackNode*>(currentNode->getParent())};

        // there is always a grandparent (parent is red so there should be a black grandparent)
        while(parent != nullptr && !parent->isBlack())
        {
            RedBlackNode* uncle{static_cast<RedBlackNode*>(currentNode->getUncle())};
            RedBlackNode* grandParent{static_cast<RedBlackNode*>(currentNode->getGrandparent())};

            if (uncle == nullptr || uncle->isBlack())
            {
                // parent and current node cannot be root so they are either left or right child (no need for [node]->isRightChild())
                bool isNodeLeftChild{currentNode->isLeftChild()};
                bool isParentLeftChild{parent->isLeftChild()};

                if (isParentLeftChild && isNodeLeftChild)
                {
                    // left - left: rotate grandparent right, then swap colors of grandparent (black->red) and parent (red->black)
                    _rotateNodeRight(grandParent);
                    grandParent->setBlack(false);
                    parent->setBlack(true);
                }
                else if (isParentLeftChild && !isNodeLeftChild)
                {
                    // left - right: rotate parent left and then apply previous case (but this time current node and grandparent have colors swapped)
                    _rotateNodeLeft(parent);
                    _rotateNodeRight(grandParent);
                    grandParent->setBlack(false);
                    currentNode->setBlack(true);
                }
                else if (!isParentLeftChild && isNodeLeftChild)
                {
                    // right - left: rotate parent left, then apply next case (but this time current node and grandparent have colors swapped)
                    _rotateNodeRight(parent);
                    _rotateNodeLeft(grandParent);
                    grandParent->setBlack(false);
                    currentNode->setBlack(true);
                }
                else
                {
                    // right - right: rotate grandparent left, then swap colors of grandparent (black->red) and parent (red->black)
                    _rotateNodeLeft(grandParent);
                    grandParent->setBlack(false);
                    parent->setBlack(true);
                }

                break;
            }
            else
            {
                parent->setBlack(true);
                uncle->setBlack(true);
                grandParent->setBlack(false);
                currentNode = grandParent;
                parent = static_cast<RedBlackNode*>(currentNode->getParent());

                if (parent == nullptr)
                {
                    currentNode->setBlack(true);
                }
            }
        }

        // ensure the root is black node (cornercase: only one node added to empty red-black tree)
        if (!static_cast<RedBlackNode*>(m_Root)->isBlack())
        {
            currentNode->setBlack(true);
        }
    }

    return addedNode;
}

/* Removing a red-black tree node occurs in two steps:
   - step 1: store reference to parent and sibling; then remove the node from tree by using the "standard" (inherited) BST procedure
   - step 2: correct the resulting tree structure by performing rotations and/or recoloring in order to make sure all four rules (see redblacktree.h) are still followed.

   Notes:
   - the second step is performed only if a black node has been removed. Removing a red node (which can only be leaf) does not affect the rules.
   - the second step contains two cases: child of removed node is red (simple case - red child becomes black) and a black leaf node has been removed (complex case - more sub-cases/scenarios)
*/
void RedBlackTree::_removeNodeFromRBTree(RedBlackTree::RedBlackNode* nodeToRemove)
{
    assert(nodeToRemove != nullptr && "Attempt to remove a null node from red-black tree");

    RedBlackNode* parent{static_cast<RedBlackNode*>(nodeToRemove->getParent())};
    RedBlackNode* sibling{static_cast<RedBlackNode*>(nodeToRemove->getSibling())};
    RedBlackNode* replacingNode{static_cast<RedBlackNode*>(_removeSingleChildedOrLeafNode(nodeToRemove))};

    if (nodeToRemove->isBlack())
    {
        if (replacingNode != nullptr) // case 1: removed black node with red child
        {
            assert(!replacingNode->isBlack() && "The removed node was black and had a single black child prior to removal (rule 4 violation)");
            replacingNode->setBlack(true);
        }
        else // case 2: removed black leaf node
        {
            RedBlackNode* doubleBlackNode{replacingNode}; // convert replacing node (null leaf) to double-black node
            bool isTreeValid{false};

            while (!isTreeValid)
            {
                // non-root BLACK node SHOULD HAVE a sibling to avoid rule 4 violation
                if (doubleBlackNode != m_Root)
                {
                    assert((sibling != nullptr && sibling->getParent() != nullptr) && "Invalid sibling of non-root black node found");
                }

                RedBlackNode* siblingLeftChild{static_cast<RedBlackNode*>(sibling->getLeftChild())};
                RedBlackNode* siblingRightChild{static_cast<RedBlackNode*>(sibling->getRightChild())};
                bool isSiblingLeftChildRed{siblingLeftChild != nullptr && !siblingLeftChild->isBlack()};
                bool isSiblingRightChildRed{siblingRightChild != nullptr && !siblingRightChild->isBlack()};
                bool isLeftSibling{sibling->isLeftChild()};

                if (doubleBlackNode == m_Root)
                {
                    isTreeValid = true; // for double-black root: "convert" double black to black and finish
                }
                else if (sibling->isBlack())
                {
                    if (isSiblingLeftChildRed || isSiblingRightChildRed) // sub-case 2a: black sibling with at least one red child
                    {
                        if (isLeftSibling && isSiblingLeftChildRed) // left-left (left sibling with left red child (right sibling child null, cannot be black) or both children red)
                        {
                            parent->rotateRight();
                            sibling->setBlack(parent->isBlack()); // if removed node parent is red: swap the parent and sibling colors in order to get local black height preserved
                            parent->setBlack(true);
                            siblingLeftChild->setBlack(true);
                        }
                        else if (isLeftSibling && !isSiblingLeftChildRed) // left-right (left sibling with right red child only, no sibling left child)
                        {
                            // swap sibling and sibling child colors after first rotation; after second rotation ensure parent and sibling have the same color (preserve local black height)
                            sibling->rotateLeft();
                            parent->rotateRight();
                            sibling->setBlack(parent->isBlack());
                            siblingRightChild->setBlack(true);
                        }
                        else if (isSiblingRightChildRed)  // right-right (left-left mirrored)
                        {
                            parent->rotateLeft();
                            sibling->setBlack(parent->isBlack());
                            parent->setBlack(true);
                            siblingRightChild->setBlack(true);
                        }
                        else // right-left (left-right mirrored)
                        {
                            sibling->rotateRight();
                            parent->rotateLeft();
                            sibling->setBlack(parent->isBlack());
                            siblingLeftChild->setBlack(true);
                        }

                        isTreeValid = true;
                    }
                    else // sub-case 2b: two sibling black children
                    {
                        sibling->setBlack(false);

                        if (parent->isBlack())
                        {
                            doubleBlackNode = parent; // set parent double black and recur
                            parent = static_cast<RedBlackNode*>(doubleBlackNode->getParent());
                            sibling = static_cast<RedBlackNode*>(doubleBlackNode->getSibling());
                        }
                        else
                        {
                            parent->setBlack(true); // set parent black, local black height remains preserved
                            isTreeValid = true;
                        }
                    }
                }
                else // sub-case 2c: red sibling (should have EXACTLY two black non-null children)
                {
                    assert((siblingLeftChild != nullptr && siblingRightChild != nullptr) && "At least one null child identified for red sibling");
                    sibling->setBlack(true);

                    if (isLeftSibling)
                    {
                        parent->rotateRight();
                        siblingRightChild->setBlack(false);
                    }
                    else
                    {
                        parent->rotateLeft();
                        siblingLeftChild->setBlack(false);
                    }
                }
            }
        }
    }
}

void RedBlackTree::_copyRBTreeNodes(const RedBlackTree& sourceTree)
{
    std::vector<Node*> sourceTreeArray;
    sourceTree._convertTreeToArray(sourceTreeArray);

    for (std::vector<Node*>::const_iterator it{sourceTreeArray.cbegin()}; it != sourceTreeArray.cend(); ++it)
    {
        const Node* addedNode{_doAddOrUpdateRBNode((*it)->getKey(), (*it)->getValue())};

        if (addedNode == nullptr)
        {
            std::clog << "Warning: red-black node " << (*it)->getKey() << " already present. Value overridden" << std::endl;
        }
    }
}

BinarySearchTree::Node* RedBlackTree::_createNewNode(int key, const std::string& value)
{
    RedBlackNode* newNode{new RedBlackNode{key, value}};
    return newNode;
}

RedBlackTree::RedBlackNode::RedBlackNode(int key, std::string value)
    : Node(key, value)
    , m_IsBlack{false}  // all newly nodes are red by default as this is the convention for adding them to the red-black-tree
{
}

void RedBlackTree::RedBlackNode::setBlack(bool isBlackRequired)
{
    m_IsBlack = isBlackRequired;
}

bool RedBlackTree::RedBlackNode::isBlack() const
{
    return m_IsBlack;
}
