#include <cassert>

#include "avltree.h"

AVLTree::AVLTree(const std::string& nullValue)
    : BinarySearchTree(nullValue)
{
}

AVLTree::AVLTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
    : AVLTree{nullValue}
{
    if (!inputKeys.empty() && defaultValue != nullValue)
    {
        // temporary object is required in order to avoid directly calling _createTreeStructure() which contains calls to virtual methods
        AVLTree temp{nullValue};
        temp._createTreeStructure(inputKeys, defaultValue, nullValue);

        // move temporary object to current object
        *this = std::move(temp);
    }
}

AVLTree::AVLTree(const AVLTree& sourceTree)
    : AVLTree{sourceTree.getNullValue()}
{
    // temporary object is required in order to avoid directly calling _copyTreeNodes() which contains calls to virtual methods
    AVLTree temp{sourceTree.getNullValue()};
    temp = sourceTree;

    // move temporary object to current object
    *this = std::move(temp);
}

AVLTree::AVLTree(AVLTree&& sourceTree)
{
    _moveAssignTree(sourceTree);
}

AVLTree& AVLTree::operator=(const AVLTree& sourceTree)
{
    if (this != &sourceTree)
    {
        clear();

        _setNullValue(sourceTree.getNullValue());
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

AVLTree& AVLTree::operator=(AVLTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        clear();
        _moveAssignTree(sourceTree);
    }

    return *this;
}

/* Steps:
   - do a bottom-up search of the first unbalanced ancestor
   - balance the tree formed by this ancestor and the child and grandchild located in the search path
*/
AVLTree::spNode AVLTree::_doAddOrUpdateNode(int key, const std::string& value)
{
    spAVLNode const addedNode{dynamic_pointer_cast<AVLNode>(BinarySearchTree::_doAddOrUpdateNode(key, value))};

    if (addedNode)
    {
        addedNode->updateHeight(); // should not be required (node added as leaf with correct subtree height setup from constructor) - just a precautionary measure (probably should be removed)
        _updateAncestorHeights(addedNode);

        spAVLNode child{addedNode};
        spAVLNode parent{dynamic_pointer_cast<AVLNode>(addedNode->getParent())};
        spAVLNode grandparent{dynamic_pointer_cast<AVLNode>(addedNode->getGrandparent())};

        // find the first unbalanced ancestor (start with grandparent of added child as the child is leaf and the parent also cannot be unbalanced after the node has been added)
        while (grandparent)
        {
            if (!grandparent->isBalanced())
            {
                break;
            }

            child = parent;
            parent = grandparent;
            grandparent = dynamic_pointer_cast<AVLNode>(grandparent->getParent());
        }

        if (grandparent)
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
AVLTree::spNode AVLTree::_removeSingleChildedOrLeafNode(spNode nodeToRemove)
{
    spAVLNode parent{nullptr};

    if (nodeToRemove)
    {
        assert((!nodeToRemove->getLeftChild() || !nodeToRemove->getRightChild()) && "Node to be removed has more than one child");
        parent = dynamic_pointer_cast<AVLNode>(nodeToRemove->getParent()); // store parent in advance (BinarySearchTree::_removeSingleChildedOrLeafNode() decouples from parent)

        /* reason for using (void): the replacing node is not relevant as the removed node parent will be the first node to be checked whether being unbalanced
           (no matter if removed node is leaf or has one child */
        (void)BinarySearchTree::_removeSingleChildedOrLeafNode(nodeToRemove);
    }
    else
    {
        assert(false && "Attempt to remove a null node!");
    }

    if (parent)
    {
        parent->updateHeight();
        _updateAncestorHeights(parent);
    }

    for (spAVLNode currentNode{parent}; currentNode != nullptr;)
    {
        if (currentNode->isBalanced())
        {
            currentNode = dynamic_pointer_cast<AVLNode>(currentNode->getParent());
            continue;
        }

        spAVLNode const currentNodeChild{currentNode->getGreaterHeightChild()};

        if (!currentNodeChild)
        {
            assert(false && "Null larger height child found for unbalanced node"); // defensive programming, normally this issue should not occur
            break;
        }

        spAVLNode const currentNodeGrandchild{currentNodeChild->getGreaterHeightChild()};

        if (!currentNodeGrandchild)
        {
            assert(false && "Null larger height grandchild found for unbalanced node"); // defensive programming, normally this issue should not occur
            break;
        }

        currentNode = _balanceSubtree(currentNode, currentNodeChild, currentNodeGrandchild);
    }

    return nullptr; // no replacing node required for AVL nodes (return value only for signature purposes)
}

AVLTree::spNode AVLTree::_createNewNode(int key, const std::string& value)
{
    return std::make_shared<AVLNode>(key, value);
}

void AVLTree::_updateAncestorHeights(spAVLNode node)
{
    if (node)
    {
        spAVLNode nodeToUpdate{dynamic_pointer_cast<AVLNode>(node->getParent())};

        while (nodeToUpdate)
        {
            nodeToUpdate->updateHeight();
            nodeToUpdate = dynamic_pointer_cast<AVLNode>(nodeToUpdate->getParent());
        }
    }
    else
    {
        assert(false && "Attempt to update ancestors of a null node");
    }
}

AVLTree::spAVLNode AVLTree::_balanceSubtree(spAVLNode grandparent, spAVLNode parent, spAVLNode child)
{
    spAVLNode resultingSubtreeRoot{nullptr};

    if (grandparent && parent && child)
    {
        assert(child->getParent() == parent && parent->getParent() == grandparent && "Invalid relationships detected between subtree nodes");

        // this node reference is required by the node removal operation as after balancing it will recur starting from resulting root in search for another unbalanced node (search stops when reaching TREE ROOT)
        resultingSubtreeRoot = parent;

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
    }
    else
    {
        assert(grandparent && "Null subtree root detected");
        assert(parent && "Null subtree root child detected");
        assert(child && "Null subtree root grandchild detected");
    }

    return resultingSubtreeRoot;
}

AVLTree::AVLNode::AVLNode(int key, std::string value)
    : Node{key, value}
    , m_Height{1} // the subtree height includes the node itself (we assume it is not connected to anything when first created)
{
}

void AVLTree::AVLNode::updateHeight()
{
    spAVLNode const leftChild{dynamic_pointer_cast<AVLNode>(getLeftChild())};
    spAVLNode const rightChild{dynamic_pointer_cast<AVLNode>(getRightChild())};

    const short c_LeftSubtreeHeight{leftChild ? leftChild->m_Height : short{0}};
    const short c_RightSubtreeHeight{rightChild ? rightChild->m_Height : short{0}};

    m_Height = 1 + std::max(c_LeftSubtreeHeight, c_RightSubtreeHeight);
}

bool AVLTree::AVLNode::isBalanced() const
{
    spAVLNode const leftChild{dynamic_pointer_cast<AVLNode>(getLeftChild())};
    spAVLNode const rightChild{dynamic_pointer_cast<AVLNode>(getRightChild())};

    const short c_LeftSubtreeHeight{leftChild ? leftChild->m_Height : short{0}};
    const short c_RightSubtreeHeight{rightChild ? rightChild->m_Height : short{0}};

    const bool c_IsBalanced{std::abs(c_LeftSubtreeHeight - c_RightSubtreeHeight) <= 1};

    return c_IsBalanced;
}

AVLTree::spAVLNode AVLTree::AVLNode::getGreaterHeightChild() const
{
    spAVLNode const leftChild{dynamic_pointer_cast<AVLNode>(getLeftChild())};
    spAVLNode const rightChild{dynamic_pointer_cast<AVLNode>(getRightChild())};
    spAVLNode const result{!leftChild || (rightChild && leftChild->m_Height < rightChild->m_Height) ? rightChild : leftChild};

    return result;
}
