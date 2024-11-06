/* This class implements an AVL binary search tree
   The AVL is a balanced tree that has following property: the height difference of any two leaf nodes is maximum 1
*/

#ifndef AVLTREE_H
#define AVLTREE_H

#include "binarysearchtree.h"


class AVLTree final: public BinarySearchTree
{
public:
    AVLTree(const std::string& nullValue = "");
    AVLTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue = "");
    AVLTree(const AVLTree& sourceTree);
    AVLTree(AVLTree&& sourceTree);

    AVLTree& operator=(const AVLTree& sourceTree);
    AVLTree& operator=(AVLTree&& sourceTree);

private:
    class AVLNode : public Node
    {
    public:
        using spAVLNode = std::shared_ptr<AVLNode>;

        AVLNode(int key, std::string value);

        void updateHeight();

        bool isBalanced() const;
        spAVLNode getGreaterHeightChild() const;

    private:
        short m_Height;
    };

    using spAVLNode = AVLNode::spAVLNode;

    // design decision: any assignment operator to work only between trees of same type
    using BinarySearchTree::operator=;

    spNode _doAddOrUpdateNode(int key, const std::string& value) override;
    spNode _removeSingleChildedOrLeafNode(spNode const nodeToRemove) override;
    spNode _createNewNode(int key, const std::string& value) override;

    // update all ancestors of a specific node up to the root
    void _updateAncestorHeights(AVLNode::spAVLNode const node);

    spAVLNode _balanceSubtree(spAVLNode const grandparent, spAVLNode const parent, spAVLNode const child);
};

#endif // AVLTREE_H
