#include <cassert>

#include "avltree.h"

AVLTree::AVLTree(const std::string& nullValue)
    : BinarySearchTree(nullValue)
{
}

/* This AVL constructor does not call the "same arguments" base BST class constructor in init list
   The reason is that the base constructor creates Node objects (instead of AVLNode) and uses the simple BST logic for connecting them with each other
   Consequently the "empty tree" base constructor is called instead
*/
AVLTree::AVLTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
    : BinarySearchTree{nullValue}
{
    if (inputKeys.size() != 0 && defaultValue != nullValue)
    {
        _createTreeStructure(inputKeys, defaultValue, nullValue);
    }
}

AVLTree::AVLTree(const AVLTree& sourceTree)
    : BinarySearchTree{sourceTree.m_NullValue} // the "empty tree" base constructor is called for the above mentioned reasons
{
    _copyTreeNodes(sourceTree);
}

AVLTree::AVLTree(AVLTree&& sourceTree)
{
    m_Root = sourceTree.m_Root;
    m_NullValue = sourceTree.m_NullValue;
    m_Size = sourceTree.m_Size;

    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

AVLTree& AVLTree::operator=(const AVLTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root)
        {
            clear();
        }

        m_NullValue = sourceTree.m_NullValue;
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

AVLTree& AVLTree::operator=(AVLTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root)
        {
            clear();
        }

        m_Root = sourceTree.m_Root;
        m_Size = sourceTree.m_Size;
        sourceTree.m_Root = nullptr;
        sourceTree.m_Size = 0;
        m_NullValue = sourceTree.m_NullValue;
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
            else
            {
                child = parent;
                parent = grandparent;
                grandparent = dynamic_pointer_cast<AVLNode>(grandparent->getParent());
            }
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
AVLTree::spNode AVLTree::_removeSingleChildedOrLeafNode(spNode const nodeToRemove)
{
    if (nodeToRemove)
    {
        assert((!nodeToRemove->getLeftChild() || !nodeToRemove->getRightChild()) && "Node to be removed has more than one child");

        spAVLNode const parent{dynamic_pointer_cast<AVLNode>(nodeToRemove->getParent())};

        /* reason for using (void): the replacing node is not relevant as the removed node parent will be the first node to be checked whether being unbalanced
           (no matter if removed node is leaf or has one child */
        (void)BinarySearchTree::_removeSingleChildedOrLeafNode(nodeToRemove);

        if (parent)
        {
            parent->updateHeight();
            _updateAncestorHeights(parent);

            spAVLNode currentNode{parent};

            while (currentNode)
            {
                if (!currentNode->isBalanced())
                {
                    spAVLNode const currentNodeChild{currentNode->getGreaterHeightChild()};

                    if (currentNodeChild)
                    {
                        spAVLNode const currentNodeGrandchild{currentNodeChild->getGreaterHeightChild()};

                        if (currentNodeGrandchild)
                        {
                            currentNode = _balanceSubtree(currentNode, currentNodeChild, currentNodeGrandchild);
                        }
                        else
                        {
                            currentNode = nullptr;
                            assert(false && "Null larger height grandchild found for unbalanced node"); // defensive programming, normally this issue should not occur
                        }
                    }
                    else
                    {
                        currentNode = nullptr;
                        assert(false && "Null larger height child found for unbalanced node"); // defensive programming, normally this issue should not occur
                    }
                }
                else
                {
                    currentNode = dynamic_pointer_cast<AVLNode>(currentNode->getParent());
                }
            }
        }
    }
    else
    {
        assert(false && "Attempt to remove a null node for AVL tree");
    }

    return nullptr; // no replacing node required for AVL nodes (return value only for signature purposes)
}

AVLTree::spNode AVLTree::_createNewNode(int key, const std::string& value)
{
    return std::make_shared<AVLNode>(key, value);
}

void AVLTree::_updateAncestorHeights(spAVLNode const node)
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

AVLTree::spAVLNode AVLTree::_balanceSubtree(spAVLNode const grandparent, spAVLNode const parent, spAVLNode const child)
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
