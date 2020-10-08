#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <string>
#include <vector>

class SimpleBST
{
public:
    SimpleBST(std::string defaultNullValue = "");
    ~SimpleBST();

    bool addOrUpdateNode(int key, const std::string& value); // true if node added (number of nodes increased)
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

    Node* _findNode(int key) const;
    void _convertTreeToArray(std::vector<Node*>& nodes) const;

    Node* m_Root;
    std::string m_DefaultNullValue;
};



#endif // BINARYSEARCHTREE_H
