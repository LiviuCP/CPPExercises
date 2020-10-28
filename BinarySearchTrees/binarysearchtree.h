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

    virtual void printTree() const;

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

        void copyInOrderSuccessorKeyAndValue();
        Node* getInOrderSuccessor() const;

        // required for implementing red-black, AVL trees etc.
        void rotateLeft();
        void rotateRight();

        Node* getParent() const;
        Node* getSibling() const;
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
    Node* _removeSingleChildedOrLeafNode(Node* nodeToRemove);

    void _deleteAllTreeNodes();

    Node* _findNode(int key) const;
    void _convertTreeToArray(std::vector<Node*>& nodes) const;

    void _rotateNodeLeft(Node* node);
    void _rotateNodeRight(Node* node);

    void _printNodeRelatives(const Node* node) const;

    /* We need this function to be virtual so future node types can be created too (e.g. red-black nodes)
       It creates a dynamically allocated node object of type Node or a derived type
       It is used by the base _doAddOrUpdateNode() method for adding a new node to the tree
       It should be implemented by each class that requires a node type different from Node (this type should be derived from Node)
       A tree class is supposed to have nodes of the same type so a static_cast conversion (Node* -> [DerivedNode]*) would be possible for accessing the derived nodes specific properties/methods
       It is recommended to use covariant return type in derived class for better readability
    */
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
