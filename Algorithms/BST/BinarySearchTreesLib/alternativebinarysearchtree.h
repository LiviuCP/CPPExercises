#ifndef ALTERNATIVEBINARYSEARCHTREE_H
#define ALTERNATIVEBINARYSEARCHTREE_H

#include <string>
#include <vector>
#include <memory>

class AlternativeBinarySearchTree final
{
public:
    AlternativeBinarySearchTree(const std::string& nullValue = "");
    AlternativeBinarySearchTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue = "");
    AlternativeBinarySearchTree(const AlternativeBinarySearchTree& sourceTree);
    AlternativeBinarySearchTree(AlternativeBinarySearchTree&& sourceTree);

    bool addOrUpdateNode(int key, const std::string& value); // in actual implementation(s) true is returned if new node is added (number of nodes increases)
    bool removeNode(int key); // in actual implementation(s) true is returned if the node with the given key exists within tree structure (and thus is removed)

    bool mergeTree(AlternativeBinarySearchTree& sourceTree);
    void clear();

    AlternativeBinarySearchTree& operator=(const AlternativeBinarySearchTree& sourceTree);
    AlternativeBinarySearchTree& operator=(AlternativeBinarySearchTree&& sourceTree);

    bool operator==(const AlternativeBinarySearchTree& tree) const;

    std::string getNodeValue(int key) const;
    std::string getNullValue() const;
    int getSize() const;

    void printTree() const;
    std::string getTreeAsString(bool areNodeValuesRequired = false) const;

    static void enableLogging(bool enable);

protected:
    class Node final
    {
    public:
        using spNode = std::shared_ptr<Node>;
        using wpNode = std::weak_ptr<Node>;

        Node(int key, std::string value);

        int getKey() const;

        void setValue(const std::string& value);
        std::string getValue() const;

        bool isLeftChild() const;
        bool isRightChild() const;

        void setLeftChild(spNode leftChild);
        spNode getLeftChild() const;

        void setRightChild(spNode rightChild);
        spNode getRightChild() const;

        void copyInOrderSuccessorKeyAndValue();
        spNode getInOrderSuccessor() const;

        /* Should always be used after the current node is set as left/right child of the parent
           Due to weak pointer constraints it is no longer possible to have the parent set by the setLeftChild()/setRightChild() methods
        */
        void setParent(spNode parent);

        spNode getParent() const;
        spNode getSibling() const;
        spNode getUncle() const;
        spNode getGrandparent() const;

        bool operator==(const Node& node) const;

    private:
        wpNode m_Parent;
        spNode m_LeftChild;
        spNode m_RightChild;
        int m_Key;
        std::string m_Value;
    };

    using spNode = Node::spNode;
    using wpNode = Node::wpNode;

    void _createTreeStructure(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue);
    void _copyTreeNodes(const AlternativeBinarySearchTree& sourceTree);
    void _moveTreeNodes(AlternativeBinarySearchTree& sourceTree);

    spNode _doAddOrUpdateNode(int key, const std::string& value);
    void _removeSingleChildedOrLeafNode(spNode nodeToRemove);

    spNode _createNewNode(int key, const std::string& value);

    spNode _findNode(int key) const;
    void _convertTreeToArray(std::vector<spNode>& nodes) const;

    void _printNodeRelatives(const spNode node) const;
    std::string _getNodeAsString(const spNode node, bool isValueRequired) const;

    spNode m_Root;
    std::string m_NullValue; // value that each key that is NOT contained within tree corresponds to
    int m_Size; // used for easy retrieval of the number of nodes (to avoid tree traversal)

    static bool sLoggingEnabled;

public:
    class InOrderForwardIterator final
    {
        friend class AlternativeBinarySearchTree;

    public:
        InOrderForwardIterator();

        void next();
        int getKey() const;

        void setValue(const std::string& value);
        std::string getValue() const;

        InOrderForwardIterator& operator=(const InOrderForwardIterator& other);
        bool operator==(const InOrderForwardIterator& other) const;

    private:
        InOrderForwardIterator(spNode node, const std::string& nullValue = "");

        wpNode m_Node;
        std::string m_NullValue;
    };

    InOrderForwardIterator begin();
    InOrderForwardIterator end();
    InOrderForwardIterator find(int key);
    InOrderForwardIterator root();
};

#endif // ALTERNATIVEBINARYSEARCHTREE_H
