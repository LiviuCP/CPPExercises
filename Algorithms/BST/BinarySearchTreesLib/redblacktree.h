/*
Red-black tree nodes rules:
1) Each node is either RED or BLACK
2) The root is ALWAYS black
3) No two consecutive red nodes are allowed
4) The number of black roots from root to each NULL leaf (including root node) should be the same

Other conventions:
- each newly added node has initial color red (it could change after in order to match the above rules)
- the NULL leaf nodes are always considered black

Notes:
- no node can have a single black child (rule 4 violation)
- if a black node has a single red child it cannot have any grandchildren (red grandchild(ren): rule 3 violation, black grandchildren: rule 4 violation)
- a black node can have mixed children (one black, one red) if the red node has two black children (the black child could also have for example 0, 1 or 2 red children)
*/

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

#include "binarysearchtree.h"

template<typename K, typename V>
class RedBlackTree final : public BinarySearchTree<K, V>
{
public:
    RedBlackTree(const V& nullValue = {});
    RedBlackTree(const std::vector<K>& inputKeys, const V& defaultValue, const V& nullValue = {});
    RedBlackTree(const RedBlackTree& sourceTree);
    RedBlackTree(RedBlackTree&& sourceTree);

    RedBlackTree& operator=(const RedBlackTree& sourceTree);
    RedBlackTree& operator=(RedBlackTree&& sourceTree);

    void printTree() const override;

private:
    class RedBlackNode : public BinarySearchTree<K, V>::Node
    {
    public:
        using spRBNode = std::shared_ptr<RedBlackNode>;

        RedBlackNode(K key, V value);

        void setBlack(bool isBlackRequired);
        bool isBlack() const;

    private:
        bool m_IsBlack;
    };

    using spRBNode = typename RedBlackNode::spRBNode;

    // design decision: any assignment operator to work only between trees of same type
    using BinarySearchTree<K, V>::operator=;

    typename BinarySearchTree<K, V>::spNode _doAddOrUpdateNode(K key, const V& value) override;
    typename BinarySearchTree<K, V>::spNode _removeSingleChildedOrLeafNode(typename BinarySearchTree<K, V>::spNode nodeToRemove) override;
    typename BinarySearchTree<K, V>::spNode _createNewNode(K key, const V& value) override;

    std::string _getNodeAsString(typename BinarySearchTree<K, V>::spNode node, bool isValueRequired) const override;
};

template<typename K, typename V>
RedBlackTree<K, V>::RedBlackTree(const V& nullValue)
    : BinarySearchTree<K, V>{nullValue}
{
}

template<typename K, typename V>
RedBlackTree<K, V>::RedBlackTree(const std::vector<K>& inputKeys, const V& defaultValue, const V& nullValue)
    : RedBlackTree{nullValue}
{
    if (!inputKeys.empty() && defaultValue != nullValue)
    {
        // temporary object is required in order to avoid directly calling _doAddOrUpdateNode() which is virtual
        RedBlackTree temp{nullValue};

        for (const auto& inputKey : inputKeys)
        {
            spRBNode const addedNode{dynamic_pointer_cast<RedBlackNode>(temp._doAddOrUpdateNode(inputKey, defaultValue))};

            if (RedBlackTree<K, V>::isLoggingEnabled() && addedNode)
            {
                std::clog << "Warning: duplicate red-black tree key found: " << inputKey << std::endl;
            }
        }

        // move temporary object to current object
        *this = std::move(temp);
    }
}

template<typename K, typename V>
RedBlackTree<K, V>::RedBlackTree(const RedBlackTree& sourceTree)
    : RedBlackTree{sourceTree.getNullValue()}
{
    // temporary object is required in order to avoid directly calling _copyTreeNodes() which contains calls to virtual methods
    RedBlackTree temp{sourceTree.getNullValue()};
    temp = sourceTree;

    // move temporary object to current object
    *this = std::move(temp);
}

template<typename K, typename V>
RedBlackTree<K, V>::RedBlackTree(RedBlackTree&& sourceTree)
{
    RedBlackTree::_moveAssignTree(sourceTree);
}

template<typename K, typename V>
RedBlackTree<K, V>& RedBlackTree<K, V>::operator=(const RedBlackTree& sourceTree)
{
    if (this != &sourceTree)
    {
        RedBlackTree::clear();

        RedBlackTree::_setNullValue(sourceTree.getNullValue());
        RedBlackTree::_copyTreeNodes(sourceTree);
    }

    return *this;
}

template<typename K, typename V>
RedBlackTree<K, V>& RedBlackTree<K, V>::operator=(RedBlackTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        RedBlackTree::clear();
        RedBlackTree::_moveAssignTree(sourceTree);
    }

    return *this;
}

template<typename K, typename V>
void RedBlackTree<K, V>::printTree() const
{
    std::vector<typename RedBlackTree<K, V>::spNode> nodesArray;
    RedBlackTree::_convertTreeToArray(nodesArray);

    for (auto it{nodesArray.cbegin()}; it != nodesArray.cend(); ++it)
    {
        spRBNode const node{dynamic_pointer_cast<RedBlackNode>(*it)};

        if (node)
        {
            std::cout << "Node: " << node->getKey();
            std::cout << " / Colour: " << (node->isBlack() ? "B" : "R");
            std::cout << " / Is child: " << (node->isLeftChild() ? "L" : node->isRightChild() ? "R" : "N");

            RedBlackTree::_printNodeRelatives(node);

            std::cout << std::endl;
        }
        else
        {
            assert(false);
        }
    }

    if (RedBlackTree<K, V>::isLoggingEnabled() && nodesArray.size() == 0)
    {
        std::clog << "Warning: red-black tree has no nodes" << std::endl;
    }
}

/* Two steps are required for adding a new RB node (update works the same as for simple BST):
  - add the node as per BST standard (inherited) procedure
  - apply required transformation to resulting tree structure (rotations, recoloring) for ensuring the four rules (see redblacktree.h) are obeyed
*/
template<typename K, typename V>
typename BinarySearchTree<K, V>::spNode RedBlackTree<K, V>::_doAddOrUpdateNode(K key, const V& value)
{
    spRBNode addedNode{dynamic_pointer_cast<RedBlackNode>(BinarySearchTree<K, V>::_doAddOrUpdateNode(key, value))};
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
            RedBlackTree::_rotateNodeRight(grandparent); // left - left: rotate grandparent right, then swap colors of grandparent (black->red) and parent (red->black)
            grandparent->setBlack(false);
            parent->setBlack(true);
        }
        else if (c_IsParentLeftChild && !c_IsNodeLeftChild)
        {
            RedBlackTree::_rotateNodeLeft(parent); // left - right: rotate parent left and then apply previous case (but this time current node and grandparent have colors swapped)
            RedBlackTree::_rotateNodeRight(grandparent);
            grandparent->setBlack(false);
            currentNode->setBlack(true);
        }
        else if (!c_IsParentLeftChild && c_IsNodeLeftChild)
        {
            RedBlackTree::_rotateNodeRight(parent); // right - left: rotate parent left, then apply next case (but this time current node and grandparent have colors swapped)
            RedBlackTree::_rotateNodeLeft(grandparent);
            grandparent->setBlack(false);
            currentNode->setBlack(true);
        }
        else
        {
            RedBlackTree::_rotateNodeLeft(grandparent); // right - right: rotate grandparent left, then swap colors of grandparent (black->red) and parent (red->black)
            grandparent->setBlack(false);
            parent->setBlack(true);
        }

        break;
    }

    if (spRBNode const rootNode{dynamic_pointer_cast<RedBlackNode>(RedBlackTree::_getRoot())}; rootNode)
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
template<typename K, typename V>
typename BinarySearchTree<K, V>::spNode RedBlackTree<K, V>::_removeSingleChildedOrLeafNode(typename BinarySearchTree<K, V>::spNode nodeToRemove)
{
    if (spRBNode const rbNodeToRemove{dynamic_pointer_cast<RedBlackNode>(nodeToRemove)}; rbNodeToRemove)
    {
        assert((!rbNodeToRemove->getLeftChild() || !rbNodeToRemove->getRightChild()) && "Node to be removed has more than one child");

        spRBNode parent{dynamic_pointer_cast<RedBlackNode>(rbNodeToRemove->getParent())}; // store parent in advance (TBinarySearchTree::_removeSingleChildedOrLeafNode() decouples from parent)
        spRBNode sibling{dynamic_pointer_cast<RedBlackNode>(rbNodeToRemove->getSibling())}; // same for sibling
        spRBNode const replacingNode{dynamic_pointer_cast<RedBlackNode>(BinarySearchTree<K, V>::_removeSingleChildedOrLeafNode(nodeToRemove))};
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
            if (doubleBlackNode == RedBlackTree::_getRoot())
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
                    RedBlackTree::_rotateNodeRight(parent);
                    sibling = siblingRightChild; // new sibling when recurring due to parent rotation
                }
                else // mirrored case
                {
                    RedBlackTree::_rotateNodeLeft(parent);
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
                RedBlackTree::_rotateNodeRight(parent);
                sibling->setBlack(parent->isBlack()); // if removed node parent is red: swap the parent and sibling colors in order to get local black height preserved
                parent->setBlack(true);
                siblingLeftChild->setBlack(true);
            }
            else if (sibling->isLeftChild() && !c_IsSiblingLeftChildRed) // 2b: left-right (left sibling with right red child only, no sibling left child)
            {
                // swap sibling and sibling child colors after first rotation; after second rotation ensure parent and sibling have the same color (preserve local black height)
                RedBlackTree::_rotateNodeLeft(sibling);
                RedBlackTree::_rotateNodeRight(parent);
                sibling->setBlack(parent->isBlack());
                siblingRightChild->setBlack(true);
            }
            else if (c_IsSiblingRightChildRed)  // 2b: right-right (left-left mirrored)
            {
                RedBlackTree::_rotateNodeLeft(parent);
                sibling->setBlack(parent->isBlack());
                parent->setBlack(true);
                siblingRightChild->setBlack(true);
            }
            else // 2b: right-left (left-right mirrored)
            {
                RedBlackTree::_rotateNodeRight(sibling);
                RedBlackTree::_rotateNodeLeft(parent);
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

template<typename K, typename V>
typename BinarySearchTree<K, V>::spNode RedBlackTree<K, V>::_createNewNode(K key, const V& value)
{
    return std::make_shared<RedBlackNode>(key, value);
}

template<typename K, typename V>
std::string RedBlackTree<K, V>::_getNodeAsString(typename BinarySearchTree<K, V>::spNode node, bool isValueRequired) const
{
    std::string result{"NULL"};

    spRBNode const rbNode{dynamic_pointer_cast<RedBlackNode>(node)};

    if (rbNode)
    {
        result = BinarySearchTree<K, V>::_getNodeAsString(rbNode, isValueRequired);
        result += rbNode->isBlack() ? ":BK" : ":RD";
    }
    else
    {
        assert(false);
    }

    return result;
}

template<typename K, typename V>
RedBlackTree<K, V>::RedBlackNode::RedBlackNode(K key, V value)
    : BinarySearchTree<K, V>::Node{key, value}
    , m_IsBlack{false}  // all newly nodes are red by default as this is the convention for adding them to the red-black-tree
{
}

template<typename K, typename V>
void RedBlackTree<K, V>::RedBlackNode::setBlack(bool isBlackRequired)
{
    if (m_IsBlack != isBlackRequired)
    {
        m_IsBlack = isBlackRequired;
    }
}

template<typename K, typename V>
bool RedBlackTree<K, V>::RedBlackNode::isBlack() const
{
    return m_IsBlack;
}

#endif // REDBLACKTREE_H
