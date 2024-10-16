#include <iostream>
#include <cassert>

#include "redblacktree.h"

RedBlackTree::RedBlackTree(const std::string& nullValue)
    : BinarySearchTree{nullValue}
{
}

/* This RB constructor does not call the "same arguments" base BST class constructor in init list
   The reason is that the base constructor creates Node objects (instead of RedBlackNode) and uses the simple BST logic for connecting them with each other
   Consequently the "empty tree" base constructor is called instead
*/
RedBlackTree::RedBlackTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
    : BinarySearchTree{nullValue}
{
    if (inputKeys.size() != 0 && defaultValue != nullValue)
    {
        for (std::vector<int>::const_iterator it{inputKeys.cbegin()}; it != inputKeys.cend(); ++it)
        {
            RedBlackNode* addedNode{_doAddOrUpdateNode(*it, defaultValue)};

            if (BinarySearchTree::sLoggingEnabled && addedNode == nullptr)
            {
                std::clog << "Warning: duplicate red-black tree key found: " << *it << std::endl;
            }
        }
    }
}

RedBlackTree::RedBlackTree(const RedBlackTree& sourceTree)
    : BinarySearchTree{sourceTree.m_NullValue} // the "empty tree" base constructor is called for the above mentioned reasons
{
    _copyTreeNodes(sourceTree);
}

RedBlackTree::RedBlackTree(RedBlackTree&& sourceTree)
{
    m_Root = sourceTree.m_Root;
    m_NullValue = sourceTree.m_NullValue;
    m_Size = sourceTree.m_Size;

    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

RedBlackTree& RedBlackTree::operator=(const RedBlackTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _doClearTreeContent();
        }

        m_NullValue = sourceTree.m_NullValue;
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

RedBlackTree& RedBlackTree::operator=(RedBlackTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _doClearTreeContent();
        }

        m_Root = sourceTree.m_Root;
        m_Size = sourceTree.m_Size;
        sourceTree.m_Root = nullptr;
        sourceTree.m_Size = 0;
        m_NullValue = sourceTree.m_NullValue;
    }

    return *this;
}

void RedBlackTree::printTree() const
{
    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<Node*>::const_iterator it{nodesArray.cbegin()}; it != nodesArray.cend(); ++it)
    {
        std::cout << "Node: " << (*it)->getKey();
        std::cout << " / Colour: " << (static_cast<RedBlackNode*>(*it)->isBlack() ? "B" : "R");
        std::cout << " / Is child: " << ((*it)->isLeftChild() ? "L" : (*it)->isRightChild() ? "R" : "N");

        _printNodeRelatives(*it);

        std::cout << std::endl;
    }

    if (BinarySearchTree::sLoggingEnabled && nodesArray.size() == 0)
    {
        std::clog << "Warning: red-black tree has no nodes" << std::endl;
    }
}

/* Two steps are required for adding a new RB node (update works the same as for simple BST):
  - add the node as per BST standard (inherited) procedure
  - apply required transformation to resulting tree structure (rotations, recoloring) for ensuring the four rules (see redblacktree.h) are obeyed
*/
RedBlackTree::RedBlackNode* RedBlackTree::_doAddOrUpdateNode(int key, const std::string& value)
{
    RedBlackNode* addedNode{static_cast<RedBlackNode*>(BinarySearchTree::_doAddOrUpdateNode(key, value))};

    if (addedNode != nullptr)
    {
        RedBlackNode* currentNode{addedNode};
        RedBlackNode* parent{static_cast<RedBlackNode*>(currentNode->getParent())};

        while(parent != nullptr && !parent->isBlack())
        {
            RedBlackNode* const uncle{static_cast<RedBlackNode*>(currentNode->getUncle())};
            RedBlackNode* const grandparent{static_cast<RedBlackNode*>(currentNode->getGrandparent())};

            assert(grandparent != nullptr && "Null grandparent for current node"); // there should always be a grandparent (parent is red)

            if (uncle == nullptr || uncle->isBlack())
            {
                // parent and current node cannot be root so they are either left or right child (no need for [node]->isRightChild())
                const bool c_IsNodeLeftChild{currentNode->isLeftChild()};
                const bool c_IsParentLeftChild{parent->isLeftChild()};

                if (c_IsParentLeftChild && c_IsNodeLeftChild) // left - left: rotate grandparent right, then swap colors of grandparent (black->red) and parent (red->black)
                {
                    _rotateNodeRight(grandparent);
                    grandparent->setBlack(false);
                    parent->setBlack(true);
                }
                else if (c_IsParentLeftChild && !c_IsNodeLeftChild) // left - right: rotate parent left and then apply previous case (but this time current node and grandparent have colors swapped)
                {
                    _rotateNodeLeft(parent);
                    _rotateNodeRight(grandparent);
                    grandparent->setBlack(false);
                    currentNode->setBlack(true);
                }
                else if (!c_IsParentLeftChild && c_IsNodeLeftChild) // right - left: rotate parent left, then apply next case (but this time current node and grandparent have colors swapped)
                {
                    _rotateNodeRight(parent);
                    _rotateNodeLeft(grandparent);
                    grandparent->setBlack(false);
                    currentNode->setBlack(true);
                }
                else // right - right: rotate grandparent left, then swap colors of grandparent (black->red) and parent (red->black)
                {
                    _rotateNodeLeft(grandparent);
                    grandparent->setBlack(false);
                    parent->setBlack(true);
                }

                break;
            }
            else
            {
                parent->setBlack(true);
                uncle->setBlack(true);
                grandparent->setBlack(false);
                currentNode = grandparent;
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
RedBlackTree::RedBlackNode* RedBlackTree::_removeSingleChildedOrLeafNode(Node* const nodeToRemove)
{
    assert(nodeToRemove != nullptr && "Attempt to remove a null node from red-black tree");
    assert((nodeToRemove->getLeftChild() == nullptr || nodeToRemove->getRightChild() == nullptr) && "Node to be removed has more than one child");

    RedBlackNode* parent{static_cast<RedBlackNode*>(nodeToRemove->getParent())};
    RedBlackNode* sibling{static_cast<RedBlackNode*>(nodeToRemove->getSibling())};
    RedBlackNode* const replacingNode{static_cast<RedBlackNode*>(BinarySearchTree::_removeSingleChildedOrLeafNode(nodeToRemove))};

    if (!static_cast<RedBlackNode*>(nodeToRemove)->isBlack()) // no action required if removed node is a red leaf
    {
    }
    else if (replacingNode != nullptr) // case 1: removed black node with red child
    {
        assert(!replacingNode->isBlack() && "The removed node was black and had a single black child prior to removal (rule 4 violation)");
        replacingNode->setBlack(true);
    }
    else // case 2: removed black leaf node
    {
        const RedBlackNode* doubleBlackNode{replacingNode}; // convert replacing node (null leaf) to double-black node
        bool isTreeValid{false};

        while (!isTreeValid)
        {
            if (doubleBlackNode == m_Root)
            {
                isTreeValid = true; // for double-black root: "convert" double black to black and finish
            }
            else
            {
                assert((sibling != nullptr && sibling->getParent() != nullptr) && "Invalid sibling of non-root black node found"); // required for avoiding rule 4 violation

                // keep reference to sibling children before doing any rotations
                RedBlackNode* const siblingLeftChild{static_cast<RedBlackNode*>(sibling->getLeftChild())};
                RedBlackNode* const siblingRightChild{static_cast<RedBlackNode*>(sibling->getRightChild())};

                if (sibling->isBlack())
                {
                    const bool c_IsSiblingLeftChildRed{siblingLeftChild != nullptr && !siblingLeftChild->isBlack()};
                    const bool c_IsSiblingRightChildRed{siblingRightChild != nullptr && !siblingRightChild->isBlack()};
                    bool doubleBlackNodeChanged{false};

                    if (!c_IsSiblingLeftChildRed && !c_IsSiblingRightChildRed) // sub-case 2a: two sibling black children (everything else is sub-case 2b: black sibling with at least one red child)
                    {
                        sibling->setBlack(false);

                        if (parent->isBlack())
                        {
                            doubleBlackNode = parent; // set parent double black and recur
                            parent = static_cast<RedBlackNode*>(doubleBlackNode->getParent());
                            sibling = static_cast<RedBlackNode*>(doubleBlackNode->getSibling());
                            doubleBlackNodeChanged = true;
                        }
                        else
                        {
                            parent->setBlack(true); // set parent black, local black height remains preserved
                        }
                    }
                    else if (sibling->isLeftChild() && c_IsSiblingLeftChildRed) // 2b: left-left (left sibling with left red child (right sibling child null, cannot be black) or both children red)
                    {
                        _rotateNodeRight(parent);
                        sibling->setBlack(parent->isBlack()); // if removed node parent is red: swap the parent and sibling colors in order to get local black height preserved
                        parent->setBlack(true);
                        siblingLeftChild->setBlack(true);
                    }
                    else if (sibling->isLeftChild() && !c_IsSiblingLeftChildRed) // 2b: left-right (left sibling with right red child only, no sibling left child)
                    {
                        // swap sibling and sibling child colors after first rotation; after second rotation ensure parent and sibling have the same color (preserve local black height)
                        _rotateNodeLeft(sibling);
                        _rotateNodeRight(parent);
                        sibling->setBlack(parent->isBlack());
                        siblingRightChild->setBlack(true);
                    }
                    else if (c_IsSiblingRightChildRed)  // 2b: right-right (left-left mirrored)
                    {
                        _rotateNodeLeft(parent);
                        sibling->setBlack(parent->isBlack());
                        parent->setBlack(true);
                        siblingRightChild->setBlack(true);
                    }
                    else // 2b: right-left (left-right mirrored)
                    {
                        _rotateNodeRight(sibling);
                        _rotateNodeLeft(parent);
                        sibling->setBlack(parent->isBlack());
                        siblingLeftChild->setBlack(true);
                    }

                    isTreeValid = !doubleBlackNodeChanged;
                }
                else // sub-case 2c: red sibling - apply below changes and RECUR to one of the cases above (in the recursion: the sibling determined below is used as starting point (new sibling))
                {
                    // sibling should have 2 non-null black children (otherwise rule 4 violation)
                    assert((siblingLeftChild != nullptr && siblingRightChild != nullptr) && "At least a null child identified for red sibling");

                    // parent and sibling should swap colors BEFORE sibling reference changes to another node
                    parent->setBlack(false);
                    sibling->setBlack(true);

                    if (sibling->isLeftChild())
                    {
                        _rotateNodeRight(parent);
                        sibling = siblingRightChild; // new sibling when recurring due to parent rotation
                    }
                    else // mirrored case
                    {
                        _rotateNodeLeft(parent);
                        sibling = siblingLeftChild;
                    }
                }
            }
        }
    }

    return nullptr; // no replacing node required for red-black nodes (return value only for signature purposes)
}

RedBlackTree::RedBlackNode* RedBlackTree::_createNewNode(int key, const std::string& value)
{
    RedBlackNode* const newNode{new RedBlackNode{key, value}};
    return newNode;
}

std::string RedBlackTree::_getNodeAsString(const BinarySearchTree::Node* const node, bool isValueRequired) const
{
    std::string result{"NULL"};

    if (node != nullptr)
    {
        result = BinarySearchTree::_getNodeAsString(node, isValueRequired);

        if (static_cast<const RedBlackNode*>(node)->isBlack())
        {
            result += ":BK";
        }
        else
        {
            result += ":RD";
        }
    }

    return result;
}

RedBlackTree::RedBlackNode::RedBlackNode(int key, std::string value)
    : Node{key, value}
    , m_IsBlack{false}  // all newly nodes are red by default as this is the convention for adding them to the red-black-tree
{
}

void RedBlackTree::RedBlackNode::setBlack(bool isBlackRequired)
{
    if (m_IsBlack != isBlackRequired)
    {
        m_IsBlack = isBlackRequired;
    }
}

bool RedBlackTree::RedBlackNode::isBlack() const
{
    return m_IsBlack;
}
