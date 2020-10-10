#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <string>
#include <vector>

class SimpleBST
{
public:
    SimpleBST(const std::string& defaultNullValue = "");
    SimpleBST(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& defaultNullValue = "");
    SimpleBST(const SimpleBST& sourceTree);
    SimpleBST(SimpleBST&& sourceTree);

    virtual ~SimpleBST();

    bool addOrUpdateNode(int key, const std::string& value); // true if node added (number of nodes increased)
    bool deleteNode(int key);

    SimpleBST& operator=(const SimpleBST& sourceTree);
    SimpleBST& operator=(SimpleBST&& sourceTree);

    std::string getNodeValue(int key) const;

    void printNodesInfo() const;

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

        void setLeftChild(Node* left);
        Node* getLeftChild() const;

        void setRightChild(Node* right);
        Node* getRightChild() const;

        Node* getParent() const;
        Node* getUncle() const;
        Node* getGrandparent() const;

    protected:
        Node* m_Parent;
        Node* m_LeftChild;
        Node* m_RightChild;

    private:
        int m_Key;
        std::string m_Value;
    };

    void _doAddOrUpdateNode(int key, const std::string& value, bool& newNodeAdded);
    Node* _findNode(int key) const;
    void _convertTreeToArray(std::vector<Node*>& nodes) const;
    void _deleteTreeNodes();
    void _copyTreeNodes(const SimpleBST &sourceTree);

    Node* m_Root;
    std::string m_DefaultNullValue;
};



#endif // BINARYSEARCHTREE_H
