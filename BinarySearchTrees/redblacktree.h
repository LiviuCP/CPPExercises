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

#include "binarysearchtree.h"

class RedBlackTree final : public BinarySearchTree
{
public:
    RedBlackTree(const std::string& nullValue = "");
    RedBlackTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue = "");
    RedBlackTree(const RedBlackTree& sourceTree);
    RedBlackTree(RedBlackTree&& sourceTree);

    RedBlackTree& operator=(const RedBlackTree& sourceTree);
    RedBlackTree& operator=(RedBlackTree&& sourceTree);

    virtual void printTree() const override;

private:
    class RedBlackNode : public Node
    {
    public:
        RedBlackNode() = delete;
        RedBlackNode(int key, std::string value);

        void setBlack(bool isBlackRequired);
        bool isBlack() const;

    private:
        bool m_IsBlack;
    };

    // design decision: any assignment operator to work only between trees of same type
    using BinarySearchTree::operator=;

    virtual RedBlackNode* _doAddOrUpdateNode(int key, const std::string& value) override;

    // Node* instead of RedBlackNode* is required as argument for signature purposes (but return type can be covariant)
    virtual RedBlackNode* _removeSingleChildedOrLeafNode(Node* nodeToRemove) override;

    virtual RedBlackNode* _createNewNode(int key, const std::string& value) override;

    virtual std::string _getNodeAsString(const Node* node, bool isValueRequired) const override;
};

#endif // REDBLACKTREE_H
