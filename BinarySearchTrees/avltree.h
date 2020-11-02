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

    virtual bool addOrUpdateNode(int key, const std::string& value) override; // true if node added (number of nodes increased)
    virtual bool deleteNode(int key) override;

    void mergeTree(AVLTree& sourceTree);

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

    // design decision: any assignment operator or merge method to work only between trees of same type
    using BinarySearchTree::mergeTree;
    using BinarySearchTree::operator=;

    AVLNode* _doAddOrUpdateNode(int key, const std::string& value);
    void _removeSingleChildedOrLeafNode(AVLNode* nodeToRemove);

    void _copyTreeNodes(const AVLTree& sourceTree);

    // update all ancestors of a specific node up to the root
    void _updateAncestorHeights(AVLNode* node);

    AVLNode* _balanceSubtree(AVLNode* grandparent, AVLNode* parent, AVLNode* child);

    virtual AVLNode* _createNewNode(int key, const std::string& value) override;
};

#endif // AVLTREE_H
