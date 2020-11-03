/* This class implements an AVL binary search tree
   The AVL is a balanced tree that has following property: the height difference of any two leaf nodes is maximum 1
*/

#ifndef AVLTREE_H
#define AVLTREE_H

#include "binarysearchtree.h"


class AVLTree final: public BinarySearchTree
{
public:
    AVLTree(const std::string& defaultNullValue = "");
    AVLTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& defaultNullValue = "");
    AVLTree(const AVLTree& sourceTree);
    AVLTree(AVLTree&& sourceTree);

    AVLTree& operator=(const AVLTree& sourceTree);
    AVLTree& operator=(AVLTree&& sourceTree);

private:
    class AVLNode : public Node
    {
    public:
        AVLNode() = delete;
        AVLNode(int key, std::string value);

        void updateHeight();

        bool isBalanced() const;
        AVLNode* getGreaterHeightChild() const;

    private:
        short m_Height;
    };

    // design decision: any assignment operator to work only between trees of same type
    using BinarySearchTree::operator=;

    virtual AVLNode* _doAddOrUpdateNode(int key, const std::string& value) override;

    // Node* instead of AVLNode* is required as argument for signature purposes (but return type can be covariant)
    virtual AVLNode* _removeSingleChildedOrLeafNode(Node* nodeToRemove) override;

    virtual AVLNode* _createNewNode(int key, const std::string& value) override;

    // update all ancestors of a specific node up to the root
    void _updateAncestorHeights(AVLNode* node);

    AVLNode* _balanceSubtree(AVLNode* grandparent, AVLNode* parent, AVLNode* child);
};

#endif // AVLTREE_H
