#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <string>
#include <vector>

class BinarySearchTree
{
public:
    BinarySearchTree(const std::string& defaultNullValue = "");
    BinarySearchTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& defaultNullValue = "");
    BinarySearchTree(const BinarySearchTree& sourceTree);
    BinarySearchTree(BinarySearchTree&& sourceTree);

    virtual ~BinarySearchTree();

    virtual bool addOrUpdateNode(int key, const std::string& value); // true if node added (number of nodes increased)
    virtual bool deleteNode(int key);

    // this function should be rendered private by derived classes as only same type trees are supposed to be merged
    void mergeTree(BinarySearchTree& sourceTree);

    // these operators should be rendered private by derived classes as they should be replaced by operators with class specific arguments (similar to constructors)
    BinarySearchTree& operator=(const BinarySearchTree& sourceTree);
    BinarySearchTree& operator=(BinarySearchTree&& sourceTree);

    // these operators can be used as they are by derived classes and it should be possible to check different search tree types for equivalence (or the lack of it)
    bool operator==(const BinarySearchTree& tree) const;
    bool operator!=(const BinarySearchTree& tree) const;

    std::string getNodeValue(int key) const;
    int getSize() const;

    virtual void printNodesInfo() const;

protected:
    class Node
    {
    public:
        Node() = delete;
        Node(int key, std::string value);

        int getKey() const;

        void setValue(const std::string& value);
        std::string getValue() const;

        bool isLeftChild() const;
        bool isRightChild() const;

        void setLeftChild(Node* leftChild);
        Node* getLeftChild() const;

        void setRightChild(Node* rightChild);
        Node* getRightChild() const;

        Node* getParent() const;
        Node* getUncle() const;
        Node* getGrandparent() const;

        bool operator==(const Node& node) const;
        bool operator!=(const Node& node) const;

    protected:
        Node* m_Parent;
        Node* m_LeftChild;
        Node* m_RightChild;

        bool _isEqualTo(const Node& node) const;

    private:
        int m_Key;
        std::string m_Value;
    };


    Node* _doAddOrUpdateNode(int key, const std::string& value);
    void _removeNodeFromTree(Node* nodeToRemove);

    void _deleteAllTreeNodes();

    Node* _findNode(int key) const;
    void _convertTreeToArray(std::vector<Node*>& nodes) const;

    void _printNodeRelativesInfo(const Node* node) const;

    // we need this function to be virtual so future node types can be created too (e.g. red-black nodes)
    virtual Node* _createNewNode(int key, const std::string& value);

    Node* m_Root;
    std::string m_DefaultNullValue;
    int m_Size;

private:
    // should only be used in current (base) class as it only implements that "simple" BST logic
    void _copyTreeNodes(const BinarySearchTree& sourceTree);

    // equivalence logic remains the same for all classes, no need for this function to be used by derived classes
    bool _isEqualTo(const BinarySearchTree& tree) const;
};



#endif // BINARYSEARCHTREE_H
