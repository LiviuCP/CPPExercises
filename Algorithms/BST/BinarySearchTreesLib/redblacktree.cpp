#include <iostream>
#include <cassert>

#include "redblacktree.h"

RedBlackTree::RedBlackTree(const std::string& nullValue)
    : BinarySearchTree{nullValue}
{
}

RedBlackTree::RedBlackTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
    : RedBlackTree{nullValue}
{
    if (!inputKeys.empty() && defaultValue != nullValue)
    {
        // temporary object is required in order to avoid directly calling _doAddOrUpdateNode() which is virtual
        RedBlackTree temp{nullValue};

        for (const auto& inputKey : inputKeys)
        {
            spRBNode const addedNode{dynamic_pointer_cast<RedBlackNode>(temp._doAddOrUpdateNode(inputKey, defaultValue))};

            if (isLoggingEnabled() && addedNode)
            {
                std::clog << "Warning: duplicate red-black tree key found: " << inputKey << std::endl;
            }
        }

        // move temporary object to current object
        *this = std::move(temp);
    }
}

RedBlackTree::RedBlackTree(const RedBlackTree& sourceTree)
    : RedBlackTree{sourceTree.getNullValue()}
{
    // temporary object is required in order to avoid directly calling _copyTreeNodes() which contains calls to virtual methods
    RedBlackTree temp{sourceTree.getNullValue()};
    temp = sourceTree;

    // move temporary object to current object
    *this = std::move(temp);
}

RedBlackTree::RedBlackTree(RedBlackTree&& sourceTree)
{
    _moveAssignTree(sourceTree);
}

RedBlackTree& RedBlackTree::operator=(const RedBlackTree& sourceTree)
{
    if (this != &sourceTree)
    {
        clear();

        _setNullValue(sourceTree.getNullValue());
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

RedBlackTree& RedBlackTree::operator=(RedBlackTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        clear();
        _moveAssignTree(sourceTree);
    }

    return *this;
}

void RedBlackTree::printTree() const
{
    std::vector<spNode> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<spNode>::const_iterator it{nodesArray.cbegin()}; it != nodesArray.cend(); ++it)
    {
        spRBNode const node{dynamic_pointer_cast<RedBlackNode>(*it)};

        if (node)
        {
            std::cout << "Node: " << node->getKey();
            std::cout << " / Colour: " << (node->isBlack() ? "B" : "R");
            std::cout << " / Is child: " << (node->isLeftChild() ? "L" : node->isRightChild() ? "R" : "N");

            _printNodeRelatives(node);

            std::cout << std::endl;
        }
        else
        {
            assert(false);
        }
    }

    if (isLoggingEnabled() && nodesArray.size() == 0)
    {
        std::clog << "Warning: red-black tree has no nodes" << std::endl;
    }
}

/* Two steps are required for adding a new RB node (update works the same as for simple BST):
  - add the node as per BST standard (inherited) procedure
  - apply required transformation to resulting tree structure (rotations, recoloring) for ensuring the four rules (see redblacktree.h) are obeyed
*/
RedBlackTree::spNode RedBlackTree::_doAddOrUpdateNode(int key, const std::string& value)
{
    spRBNode addedNode{dynamic_pointer_cast<RedBlackNode>(BinarySearchTree::_doAddOrUpdateNode(key, value))};
    spRBNode currentNode{addedNode};

    for (spRBNode parent{currentNode ? dynamic_pointer_cast<RedBlackNode>(currentNode->getParent()) : nullptr}; parent && !parent->isBlack();)
    {
        spRBNode const grandparent{dynamic_pointer_cast<RedBlackNode>(currentNode->getGrandparent())};

        if (!grandparent)
        {
            assert(false && "There should always be a valid grandparent (parent is red)!");
            break;
        }

        spRBNode const uncle{dynamic_pointer_cast<RedBlackNode>(currentNode->getUncle())};

        if (uncle && !uncle->isBlack())
        {
            parent->setBlack(true);
            uncle->setBlack(true);
            grandparent->setBlack(false);
            currentNode = grandparent;
            parent = dynamic_pointer_cast<RedBlackNode>(currentNode->getParent());

            if (!parent)
            {
                currentNode->setBlack(true);
            }

            continue;
        }

        // parent and current node cannot be root so they are either left or right child (no need for [node]->isRightChild())
        if (const bool c_IsNodeLeftChild{currentNode->isLeftChild()}, c_IsParentLeftChild{parent->isLeftChild()}; c_IsParentLeftChild && c_IsNodeLeftChild)
        {
            _rotateNodeRight(grandparent); // left - left: rotate grandparent right, then swap colors of grandparent (black->red) and parent (red->black)
            grandparent->setBlack(false);
            parent->setBlack(true);
        }
        else if (c_IsParentLeftChild && !c_IsNodeLeftChild)
        {
            _rotateNodeLeft(parent); // left - right: rotate parent left and then apply previous case (but this time current node and grandparent have colors swapped)
            _rotateNodeRight(grandparent);
            grandparent->setBlack(false);
            currentNode->setBlack(true);
        }
        else if (!c_IsParentLeftChild && c_IsNodeLeftChild)
        {
            _rotateNodeRight(parent); // right - left: rotate parent left, then apply next case (but this time current node and grandparent have colors swapped)
            _rotateNodeLeft(grandparent);
            grandparent->setBlack(false);
            currentNode->setBlack(true);
        }
        else
        {
            _rotateNodeLeft(grandparent); // right - right: rotate grandparent left, then swap colors of grandparent (black->red) and parent (red->black)
            grandparent->setBlack(false);
            parent->setBlack(true);
        }

        break;
    }

    if (spRBNode const rootNode{dynamic_pointer_cast<RedBlackNode>(_getRoot())}; rootNode)
    {
        // ensure the root is black node (cornercase: only one node added to empty red-black tree)
        if (currentNode && !rootNode->isBlack())
        {
            currentNode->setBlack(true);
        }
    }
    else
    {
        assert(false && "There should be a valid root node after adding the current node!");
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
RedBlackTree::spNode RedBlackTree::_removeSingleChildedOrLeafNode(spNode nodeToRemove)
{
    if (spRBNode const rbNodeToRemove{dynamic_pointer_cast<RedBlackNode>(nodeToRemove)}; rbNodeToRemove)
    {
        assert((!rbNodeToRemove->getLeftChild() || !rbNodeToRemove->getRightChild()) && "Node to be removed has more than one child");

        spRBNode parent{dynamic_pointer_cast<RedBlackNode>(rbNodeToRemove->getParent())}; // store parent in advance (BinarySearchTree::_removeSingleChildedOrLeafNode() decouples from parent)
        spRBNode sibling{dynamic_pointer_cast<RedBlackNode>(rbNodeToRemove->getSibling())}; // same for sibling
        spRBNode const replacingNode{dynamic_pointer_cast<RedBlackNode>(BinarySearchTree::_removeSingleChildedOrLeafNode(nodeToRemove))};
        spRBNode doubleBlackNode{nullptr}; // used if replacing node is a null leaf, convert it to a double-black node (see case 2)

        bool isTreeValid{!rbNodeToRemove->isBlack()}; // no action required if removed node is a red leaf

        if (!isTreeValid && replacingNode) // case 1: removed black node with red child
        {
            assert(!replacingNode->isBlack() && "The removed node was black and had a single black child prior to removal (rule 4 violation)");
            replacingNode->setBlack(true);
            isTreeValid = true;
        }

        while (!isTreeValid) // case 2: removed black leaf node
        {
            if (doubleBlackNode == _getRoot())
            {
                isTreeValid = true; // for double-black root: "convert" double black to black and finish
                continue;
            }

            if (!sibling || !sibling->getParent())
            {
                assert(false && "Invalid sibling of non-root black node found"); // required for avoiding rule 4 violation
                isTreeValid = false;
                break;
            }

            // keep reference to sibling children before doing any rotations
            spRBNode const siblingLeftChild{dynamic_pointer_cast<RedBlackNode>(sibling->getLeftChild())};
            spRBNode const siblingRightChild{dynamic_pointer_cast<RedBlackNode>(sibling->getRightChild())};

            if (!sibling->isBlack()) // sub-case 2c: red sibling - update sibling and RECUR to one of the sub-cases below (in the recursion: the updated sibling is used as starting point)
            {
                if (!siblingLeftChild || !siblingRightChild) // sibling should have 2 non-null black children (otherwise rule 4 violation)
                {
                    assert(false && "At least a null child identified for red sibling");
                    isTreeValid = false;
                    break;
                }

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

                continue;
            }

            isTreeValid = true; // tree is assumed valid, gets invalidated only if the double black node changes (see below)

            const bool c_IsSiblingLeftChildRed{siblingLeftChild && !siblingLeftChild->isBlack()};
            const bool c_IsSiblingRightChildRed{siblingRightChild && !siblingRightChild->isBlack()};

            if (!c_IsSiblingLeftChildRed && !c_IsSiblingRightChildRed) // sub-case 2a: two sibling black children (everything else is sub-case 2b: black sibling with at least one red child)
            {
                sibling->setBlack(false);

                if (!parent->isBlack())
                {
                    parent->setBlack(true); // set parent black, local black height remains preserved
                    continue;
                }

                doubleBlackNode = parent; // set parent double black and recur
                parent = dynamic_pointer_cast<RedBlackNode>(doubleBlackNode->getParent());
                sibling = dynamic_pointer_cast<RedBlackNode>(doubleBlackNode->getSibling());
                isTreeValid = false; // double black node changed, tree is considered invalid again
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
        }
    }
    else
    {
        assert(false && "Attempt to remove a null node!");
    }

    return nullptr; // no replacing node required for red-black nodes (return value only for signature purposes)
}

RedBlackTree::spNode RedBlackTree::_createNewNode(int key, const std::string& value)
{
    return std::make_shared<RedBlackNode>(key, value);
}

std::string RedBlackTree::_getNodeAsString(spNode node, bool isValueRequired) const
{
    std::string result{"NULL"};

    spRBNode const rbNode{dynamic_pointer_cast<RedBlackNode>(node)};

    if (rbNode)
    {
        result = BinarySearchTree::_getNodeAsString(rbNode, isValueRequired);
        result += rbNode->isBlack() ? ":BK" : ":RD";
    }
    else
    {
        assert(false);
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
