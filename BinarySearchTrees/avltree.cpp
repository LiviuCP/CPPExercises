#include <iostream>
#include <utility>

#include <cassert>

#include "avltree.h"

AVLTree::AVLTree(const std::string& defaultNullValue)
    : BinarySearchTree(defaultNullValue)
{
}

// we don't use the similar base BST class constructor in init list as we need to use another logic in building an unempty AVL tree (AVL rules to be followed)
AVLTree::AVLTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& defaultNullValue)
    : BinarySearchTree{defaultNullValue}
{
    if (inputKeys.size() != 0 && defaultValue != defaultNullValue)
    {
        for (std::vector<int>::const_iterator it{inputKeys.cbegin()}; it != inputKeys.cend(); ++it)
        {
            AVLNode* addedNode{_doAddOrUpdateNode(*it, defaultValue)};
            if (addedNode == nullptr)
            {
                std::clog << "Warning: duplicate AVL tree key found: " << *it << std::endl;
            }
        }
    }
}

AVLTree::AVLTree(const AVLTree& sourceTree)
    : BinarySearchTree{sourceTree.m_DefaultNullValue}
{
    _copyTreeNodes(sourceTree);
}

AVLTree::AVLTree(AVLTree&& sourceTree)
{
    m_Root = sourceTree.m_Root;
    m_DefaultNullValue = sourceTree.m_DefaultNullValue;
    m_Size = sourceTree.m_Size;

    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

bool AVLTree::addOrUpdateNode(int key, const std::string& value)
{
    bool newNodeAdded{false};

    if (value != m_DefaultNullValue)
    {
        const AVLNode* addedNode{_doAddOrUpdateNode(key, value)};

        if (addedNode != nullptr)
        {
            newNodeAdded = true;
        }
    }

    return newNodeAdded;
}

bool AVLTree::deleteNode(int key)
{
    bool deleted{false};

    // no need for dynamic_cast as we only work with AVLNode node objects in the AVL tree class (no mixed node types scenario) and the base class is not abstract
    AVLNode* nodeToDelete{static_cast<AVLNode*>(_findNode(key))};

    if (nodeToDelete != nullptr)
    {
        // for two-children node to be removed: the in-order successor content will be copied to the node; then the successor will be recursively deleted
        if (nodeToDelete->getLeftChild() != nullptr && nodeToDelete->getRightChild() != nullptr)
        {
            nodeToDelete->copyInOrderSuccessorKeyAndValue();
            nodeToDelete = static_cast<AVLNode*>(nodeToDelete->getInOrderSuccessor());
        }

        _removeSingleChildedOrLeafNode(nodeToDelete);

        delete nodeToDelete;
        nodeToDelete = nullptr;
        deleted = true;
    }

    return deleted;
}

void AVLTree::mergeTree(AVLTree& sourceTree)
{
    assert(m_DefaultNullValue == sourceTree.m_DefaultNullValue && "Default null values of trees don't match");

    if (this != &sourceTree)
    {
        _copyTreeNodes(sourceTree);
        sourceTree._deleteAllTreeNodes();
    }
}

AVLTree& AVLTree::operator=(const AVLTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _deleteAllTreeNodes();
        }

        m_DefaultNullValue = sourceTree.m_DefaultNullValue;
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

AVLTree& AVLTree::operator=(AVLTree&& sourceTree)
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

/* Steps:
   - do a bottom-up search of the first unbalanced ancestor
   - balance the tree formed by this ancestor and the child and grandchild located in the search path
*/
AVLTree::AVLNode* AVLTree::_doAddOrUpdateNode(int key, const std::string& value)
{
    AVLNode* addedNode{static_cast<AVLNode*>(BinarySearchTree::_doAddOrUpdateNode(key, value))};

    if (addedNode != nullptr)
    {
        addedNode->updateHeight(); // should not be required (node added as leaf with correct subtree height setup from constructor) - just a precautionary measure (probably should be removed)
        _updateAncestorHeights(addedNode);

        AVLNode* child{addedNode};
        AVLNode* parent{static_cast<AVLNode*>(addedNode->getParent())};
        AVLNode* grandparent{static_cast<AVLNode*>(addedNode->getGrandparent())};

        // find the first unbalanced ancestor (start with grandparent of added child as the child is leaf and the parent also cannot be unbalanced after the node has been added)
        while (grandparent != nullptr)
        {
            if (!grandparent->isBalanced())
            {
                break;
            }
            else
            {
                child = parent;
                parent = grandparent;
                grandparent = static_cast<AVLNode*>(grandparent->getParent());
            }
        }

        if (grandparent != nullptr)
        {
            (void)_balanceSubtree(grandparent, parent, child);
        }
    }

    return addedNode;
}

/* Steps involved:
   - remove node by standard BST method
   - if removed node is not root:
     - update parent height and ancestor heights
     - recursively balance resulting unbalanced nodes (sub-trees) starting with parent and going bottom-up until reaching root
*/
void AVLTree::_removeSingleChildedOrLeafNode(AVLTree::AVLNode* nodeToRemove)
{
    assert(nodeToRemove != nullptr && "Attempt to remove a null node for AVL tree");
    assert((nodeToRemove->getLeftChild() == nullptr || nodeToRemove->getRightChild() == nullptr) && "Node to be removed has more than one child");

    AVLNode* parent{static_cast<AVLNode*>(nodeToRemove->getParent())};

    /* reason for using (void): the replacing node is not relevant as the removed node parent will be the first node to be checked whether being unbalanced
       (no matter if removed node is leaf or has one child */
    (void)BinarySearchTree::_removeSingleChildedOrLeafNode(nodeToRemove);

    if (parent != nullptr)
    {
        parent->updateHeight();
        _updateAncestorHeights(parent);

        AVLNode* currentNode{parent};

        while (currentNode != nullptr)
        {
            if (!currentNode->isBalanced())
            {
                AVLNode* currentNodeChild{currentNode->getGreaterHeightChild()};
                assert(currentNodeChild != nullptr && "Null larger height child found for unbalanced node"); // defensive programming, normally this issue should not occur
                AVLNode* currentNodeGrandchild{currentNodeChild->getGreaterHeightChild()};
                assert(currentNodeGrandchild != nullptr && "Null larger height grandchild found for unbalanced node"); // defensive programming, normally this issue should not occur
                currentNode = _balanceSubtree(currentNode, currentNodeChild, currentNodeGrandchild);
            }
            else
            {
                currentNode = static_cast<AVLNode*>(currentNode->getParent());
            }
        }
    }
}

void AVLTree::_copyTreeNodes(const AVLTree& sourceTree)
{
    std::vector<Node*> sourceTreeArray;
    sourceTree._convertTreeToArray(sourceTreeArray);

    for (std::vector<Node*>::const_iterator it{sourceTreeArray.cbegin()}; it != sourceTreeArray.cend(); ++it)
    {
        const Node* addedNode{_doAddOrUpdateNode((*it)->getKey(), (*it)->getValue())};

        if (addedNode == nullptr)
        {
            std::clog << "Warning: AVL node " << (*it)->getKey() << " already present. Value overridden" << std::endl;
        }
    }
}

void AVLTree::_updateAncestorHeights(AVLTree::AVLNode* node)
{
    assert(node != nullptr && "Attempt to update ancestors of a null node");

    AVLNode* nodeToUpdate{static_cast<AVLNode*>(node->getParent())};

    while (nodeToUpdate != nullptr)
    {
        nodeToUpdate->updateHeight();
        nodeToUpdate = static_cast<AVLNode*>(nodeToUpdate->getParent());
    }
}

AVLTree::AVLNode* AVLTree::_balanceSubtree(AVLNode* grandparent, AVLNode* parent, AVLNode* child)
{
    assert(grandparent != nullptr && "Null subtree root detected");
    assert(parent != nullptr && "Null subtree root child detected");
    assert(child != nullptr && "Null subtree root grandchild detected");
    assert(child->getParent() == parent && parent->getParent() == grandparent && "Invalid relationships detected between subtree nodes");

    // this node reference is required by the delete operation as after balancing it will recur starting from resulting root in search for another unbalanced node (search stops when reaching TREE ROOT)
    AVLNode* resultingSubtreeRoot{parent};

    // step 1: do the corresponding rotations based on the subtree topology/relationships between the three nodes in order to obtain a balanced subtree (restore AVL rules)
    if (parent->isLeftChild() && child->isLeftChild()) // left - left case (rotate grandparent right)
    {
        _rotateNodeRight(grandparent);
    }
    else if (parent->isLeftChild() && child->isRightChild()) // left - right case (rotate parent left and then grandparent right)
    {
        resultingSubtreeRoot = child;
        _rotateNodeLeft(parent);
        _rotateNodeRight(grandparent);
    }
    else if (parent->isRightChild() && child->isLeftChild()) // right - left case (mirrored left - right)
    {
        resultingSubtreeRoot = child;
        _rotateNodeRight(parent);
        _rotateNodeLeft(grandparent);
    }
    else // right - right case (mirrored left - left)
    {
        _rotateNodeLeft(grandparent);
    }

    // step 2: do bottom-up update of the node heights (subtree and ancestors)
    grandparent->updateHeight();
    parent->updateHeight();

    if (resultingSubtreeRoot == child)
    {
        child->updateHeight();
    }

    _updateAncestorHeights(resultingSubtreeRoot);

    return resultingSubtreeRoot;
}

AVLTree::AVLNode* AVLTree::_createNewNode(int key, const std::string &value)
{
    AVLNode* newNode{new AVLNode{key, value}};
    return newNode;
}

AVLTree::AVLNode::AVLNode(int key, std::string value)
    : Node{key, value}
    , m_Height{1} // the subtree height includes the node itself (we assume it is not connected to anything when first created)
{
}

void AVLTree::AVLNode::updateHeight()
{
    const short c_LeftSubtreeHeight{m_LeftChild != nullptr ? static_cast<AVLNode*>(m_LeftChild)->m_Height : short{0}};
    const short c_RightSubtreeHeight{m_RightChild != nullptr ? static_cast<AVLNode*>(m_RightChild)->m_Height : short{0}};

    m_Height = 1 + std::max(c_LeftSubtreeHeight, c_RightSubtreeHeight);
}

bool AVLTree::AVLNode::isBalanced() const
{
    const short c_LeftSubtreeHeight{m_LeftChild != nullptr ? static_cast<AVLNode*>(m_LeftChild)->m_Height : short{0}};
    const short c_RightSubtreeHeight{m_RightChild != nullptr ? static_cast<AVLNode*>(m_RightChild)->m_Height : short{0}};

    const bool c_IsBalanced{std::abs(c_LeftSubtreeHeight-c_RightSubtreeHeight) <= 1};

    return c_IsBalanced;
}

AVLTree::AVLNode* AVLTree::AVLNode::getGreaterHeightChild() const
{
    AVLNode* result{nullptr};
    AVLNode* leftChild{static_cast<AVLNode*>(m_LeftChild)};
    AVLNode* rightChild{static_cast<AVLNode*>(m_RightChild)};

    if (leftChild == nullptr || (rightChild != nullptr && leftChild->m_Height < rightChild->m_Height))
    {
        result = rightChild;
    }
    else
    {
        result = leftChild;
    }

    return result;
}
