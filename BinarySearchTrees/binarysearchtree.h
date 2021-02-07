#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <string>
#include <vector>

class BinarySearchTree
{
public:
    BinarySearchTree(const std::string& nullValue = "");

    // these constructors shouldn't be called from derived classes that use node objects which are derived from Node
    BinarySearchTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue = "");
    BinarySearchTree(const BinarySearchTree& sourceTree);
    BinarySearchTree(BinarySearchTree&& sourceTree);

    /* created virtual destructor (to follow best practices in inheritance) although there is no need to implement destructors
       for derived tree classes (Node objects and their derived versions are supposed to use the default destructor so _doClearTreeContent() can do all the work when destroying a tree)
    */
    virtual ~BinarySearchTree();

    /* It is possible to use these two functions (and mergeTree() too) without needing to reimplement them but instead only reimplement the protected virtual functions they are calling:
       - _doAddOrUpdateNode()
       - _removeSingleChildedOrLeafNode()
       This provides increased flexibility.
    */
    virtual bool addOrUpdateNode(int key, const std::string& value); // in actual implementation(s) true is returned if new node is added (number of nodes increases)
    virtual bool removeNode(int key); // in actual implementation(s) true is returned if the node with the given key exists within tree structure (and thus is removed)

    /* it is possible to merge different types of trees but the resulting tree should be of the type of the calling object while the source tree should have all elements removed
       (currently the derived classes use the base version but it is possible to create own implementations if required)
    */
    virtual void mergeTree(BinarySearchTree& sourceTree);

    /* This function has been left unvirtualized as the clear() operation should occur the same way for all base and derived search tree objects */
    void clear();

    // these operators should be rendered private by derived classes as they should be replaced by operators with class specific arguments (similar to constructors)
    BinarySearchTree& operator=(const BinarySearchTree& sourceTree);
    BinarySearchTree& operator=(BinarySearchTree&& sourceTree);

    // these operators can be used as they are by derived classes and it should be possible to check different search tree types for equivalence (or the lack of it)
    bool operator==(const BinarySearchTree& tree) const;
    bool operator!=(const BinarySearchTree& tree) const;

    std::string getNodeValue(int key) const;
    std::string getNullValue() const;
    int getSize() const;

    virtual void printTree() const;
    virtual std::string getTreeAsString(bool areNodeValuesRequired) const;

    static void enableLogging(bool enable);

protected:
    class Node
    {
    public:
        Node() = delete;
        Node(int key, std::string value);

        virtual ~Node(); // not necessary here but good practice

        int getKey() const;

        void setValue(const std::string& value);
        std::string getValue() const;

        bool isLeftChild() const;
        bool isRightChild() const;

        void setLeftChild(Node* const leftChild);
        Node* getLeftChild() const;

        void setRightChild(Node* const rightChild);
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

        bool operator!=(const Node& node) const;

    protected:
        Node* m_Parent;
        Node* m_LeftChild;
        Node* m_RightChild;

    private:
        int m_Key;
        std::string m_Value;
    };


    // used by constructors of base and derived classes, specific behavior implemented by _doAddOrUpdateNode()
    void _createTreeStructure(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue);

    // used by RB and AVL trees as well (generic function), specific logic implemented by _doAddOrUpdateNode()
    void _copyTreeNodes(const BinarySearchTree& sourceTree);

    virtual Node* _doAddOrUpdateNode(int key, const std::string& value);
    virtual Node* _removeSingleChildedOrLeafNode(Node* const nodeToRemove);

    /* We need this function to be virtual so future node types can be created too (e.g. red-black nodes) even when derived classes only use BinarySearchTree::_doAddOrUpdateNode()
       It creates a dynamically allocated node object of type Node or a derived type
       It is used by the base _doAddOrUpdateNode() method for adding a new node to the tree
       It should be implemented by each class that requires a node type different from Node (this type should be derived from Node)
       A tree class is supposed to have nodes of the same type so a static_cast conversion (Node* -> [DerivedNode]*) would be possible for accessing the derived nodes specific properties/methods
       It is recommended to use covariant return type in derived class for better readability
    */
    virtual Node* _createNewNode(int key, const std::string& value);

    /* required for destructive operations where the whole tree content needs to be erased (tree destruction, assignment operations); can also safely delete
       node objects derived from Node unless they have a non-default destructor (currently not the case)
    */
    void _doClearTreeContent();

    Node* _findNode(int key) const;
    void _convertTreeToArray(std::vector<Node*>& nodes) const;

    void _rotateNodeLeft(Node* node);
    void _rotateNodeRight(Node* node);

    void _printNodeRelatives(const Node* node) const;
    virtual std::string _getNodeAsString(const Node* node, bool isValueRequired) const;

    Node* m_Root;
    std::string m_NullValue; // value that each key that is NOT contained within tree corresponds to
    int m_Size; // used for easy retrieval of the number of nodes (to avoid tree traversal)

    static bool sLoggingEnabled;

private:
    // equivalence logic remains the same for all classes, no need for this function to be used by derived classes
    bool _isEqualTo(const BinarySearchTree& tree) const;
};

#endif // BINARYSEARCHTREE_H
