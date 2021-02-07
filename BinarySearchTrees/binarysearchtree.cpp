#include <iostream>
#include <sstream>

#include <cassert>

#include "binarysearchtree.h"

bool BinarySearchTree::sLoggingEnabled = false;

BinarySearchTree::BinarySearchTree(const std::string& nullValue)
    : m_Root{nullptr}
    , m_NullValue{nullValue}
    , m_Size{0}
{
}

BinarySearchTree::BinarySearchTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
    : BinarySearchTree{nullValue}
{
    if (inputKeys.size() != 0 && defaultValue != nullValue)
    {
        _createTreeStructure(inputKeys, defaultValue, nullValue);
    }
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& sourceTree)
    : m_Root{nullptr}
    , m_NullValue{sourceTree.m_NullValue}
    , m_Size{0}
{
    _copyTreeNodes(sourceTree);
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& sourceTree)
    : m_Root{sourceTree.m_Root}
    , m_NullValue{sourceTree.m_NullValue}
    , m_Size{sourceTree.m_Size}
{
    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

BinarySearchTree::~BinarySearchTree()
{
    _doClearTreeContent();
}

bool BinarySearchTree::addOrUpdateNode(int key, const std::string& value)
{
    bool newNodeAdded{false};

    if (value != m_NullValue)
    {
        const Node* addedNode{_doAddOrUpdateNode(key, value)};

        if (addedNode != nullptr)
        {
            newNodeAdded = true;
        }
    }

    return newNodeAdded;
}

bool BinarySearchTree::removeNode(int key)
{
    bool removed{false};
    Node* nodeToRemove{_findNode(key)};

    if (nodeToRemove != nullptr)
    {
        // for two-children node to be removed: the in-order successor content will be copied to the node; then the successor will be recursively removed
        if (nodeToRemove->getLeftChild() != nullptr && nodeToRemove->getRightChild() != nullptr)
        {
            nodeToRemove->copyInOrderSuccessorKeyAndValue();
            nodeToRemove = nodeToRemove->getInOrderSuccessor();
        }

        // when using the standard BST remove method, replacing node can be NULL (removed node is leaf) or the child of the removed node (removed node has a single child)
        Node* replacingNode{_removeSingleChildedOrLeafNode(nodeToRemove)};
        (void)replacingNode; // replacing node is not used here, is used instead by derived classes (AVL, RB trees) when calling the BinarySearchTree::_removeSingleChildedOrLeafNode() method

        delete nodeToRemove;
        nodeToRemove = nullptr;
        removed = true;
    }

    return removed;
}

void BinarySearchTree::mergeTree(BinarySearchTree& sourceTree)
{
    assert(m_NullValue == sourceTree.m_NullValue && "Null values of trees don't match");

    if (this != &sourceTree)
    {
        _copyTreeNodes(sourceTree);
        sourceTree._doClearTreeContent();
    }
}

void BinarySearchTree::clear()
{
    _doClearTreeContent();
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _doClearTreeContent();
        }

        m_NullValue = sourceTree.m_NullValue;
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _doClearTreeContent();
        }

        /* It is required to do a copy assignment and then erase the source tree content.
           The reason is that it is possible to apply this operator to trees of different types by dereferencing BinarySearchTree pointers to derived tree objects.
           In this case using the move assignment operator by simply copying the m_Root pointer value of the source object into the destination pointer and setting the m_Root source pointer to null would cause an error.
        */
        m_NullValue = sourceTree.m_NullValue;
        _copyTreeNodes(sourceTree);
        sourceTree._doClearTreeContent();
    }

    return *this;
}

bool BinarySearchTree::operator==(const BinarySearchTree& tree) const
{
    bool areEqualTrees{_isEqualTo(tree)};
    return areEqualTrees;
}

bool BinarySearchTree::operator!=(const BinarySearchTree& tree) const
{
    bool areNotEqualTrees{!_isEqualTo(tree)};
    return areNotEqualTrees;
}

std::string BinarySearchTree::getNodeValue(int key) const
{
    std::string result{m_NullValue};

    Node* foundNode{_findNode(key)};

    if (foundNode != nullptr)
    {
        result = foundNode->getValue();
    }

    return result;
}

std::string BinarySearchTree::getNullValue() const
{
    return m_NullValue;
}

int BinarySearchTree::getSize() const
{
    return m_Size;
}

void BinarySearchTree::printTree() const
{
    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<Node*>::const_iterator it{nodesArray.cbegin()}; it != nodesArray.cend(); ++it)
    {
        std::cout << "Node: " << (*it)->getKey();
        std::cout << " / Is child: " << ((*it)->isLeftChild() ? "L" : (*it)->isRightChild() ? "R" : "N");

        _printNodeRelatives(*it);

        std::cout << std::endl;
    }

    if (sLoggingEnabled && nodesArray.size() == 0)
    {
        std::clog << "Warning: tree has no nodes" << std::endl;
    }
}

std::string BinarySearchTree::getTreeAsString(bool areNodeValuesRequired) const
{
    std::string result;

    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<Node*>::const_iterator it{nodesArray.cbegin()}; it != nodesArray.cend(); ++it)
    {
        result += _getNodeAsString(*it, areNodeValuesRequired);
        result += "/";
    }

    // trim the resulting string
    if (result.size() > 0)
    {
        result.pop_back();
    }

    return result;
}

void BinarySearchTree::enableLogging(bool enable)
{
    if (sLoggingEnabled != enable)
    {
        sLoggingEnabled = enable;
    }
}

void BinarySearchTree::_createTreeStructure(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
{
    assert(inputKeys.size() != 0 && "No keys provided for creating tree structure");
    assert(defaultValue != nullValue && "Null value set as default value for tree keys");

    for (std::vector<int>::const_iterator it{inputKeys.cbegin()}; it != inputKeys.cend(); ++it)
    {
        Node* addedNode{_doAddOrUpdateNode(*it, defaultValue)};

        if (sLoggingEnabled && addedNode == nullptr)
        {
            std::clog << "Warning: duplicate key found: " << *it << std::endl;
        }
    }
}

void BinarySearchTree::_copyTreeNodes(const BinarySearchTree& sourceTree)
{
    std::vector<Node*> sourceTreeArray;
    sourceTree._convertTreeToArray(sourceTreeArray);

    for (std::vector<Node*>::const_iterator it{sourceTreeArray.cbegin()}; it != sourceTreeArray.cend(); ++it)
    {
        const Node* addedNode{_doAddOrUpdateNode((*it)->getKey(), (*it)->getValue())};

        if (sLoggingEnabled && addedNode == nullptr)
        {
            std::clog << "Warning: node " << (*it)->getKey() << " already present. Value overridden" << std::endl;
        }
    }
}

BinarySearchTree::Node* BinarySearchTree::_doAddOrUpdateNode(int key, const std::string& value)
{
    Node* addedNode{nullptr};
    if (m_Root == nullptr)
    {
        m_Root = _createNewNode(key, value);
        addedNode = m_Root;
    }
    else
    {
        Node* currentNode{m_Root};
        bool entryPointFound{false};

        while (!entryPointFound)
        {
            const int c_CurrentNodeKey{currentNode->getKey()};

            if (key < c_CurrentNodeKey)
            {
                Node* leftChild{currentNode->getLeftChild()};
                if (leftChild == nullptr)
                {
                    addedNode = _createNewNode(key, value);
                    currentNode->setLeftChild(addedNode);
                    entryPointFound = true;
                }
                else
                {
                    currentNode = leftChild;
                }
            }
            else if (key > c_CurrentNodeKey)
            {
                Node* rightChild{currentNode->getRightChild()};
                if (rightChild == nullptr)
                {
                    addedNode = _createNewNode(key, value);
                    currentNode->setRightChild(addedNode);
                    entryPointFound = true;
                }
                else
                {
                    currentNode = rightChild;
                }
            }
            else
            {
                currentNode->setValue(value);
                entryPointFound = true;
            }
        }
    }

    if (addedNode != nullptr)
    {
        ++m_Size;
    }

    return addedNode;
}

BinarySearchTree::Node* BinarySearchTree::_removeSingleChildedOrLeafNode(BinarySearchTree::Node* nodeToRemove)
{
    Node* replacingNode{nullptr};

    if (nodeToRemove != nullptr)
    {
        Node* leftChild{nodeToRemove->getLeftChild()};
        Node* rightChild{nodeToRemove->getRightChild()};

        assert((leftChild == nullptr || rightChild == nullptr) && "Attempt to use the remove function on a node with two children");

        // determine replacing node
        if (leftChild != nullptr && rightChild == nullptr)
        {
            replacingNode = leftChild;
        }
        else if (leftChild == nullptr && rightChild != nullptr)
        {
            replacingNode = rightChild;
        }
        else // do nothing, leaf node (defensive programming)
        {
        }

        // remove and replace
        if (nodeToRemove->isLeftChild())
        {
            nodeToRemove->getParent()->setLeftChild(replacingNode);
        }
        else if (nodeToRemove->isRightChild())
        {
            nodeToRemove->getParent()->setRightChild(replacingNode);
        }
        else
        {
            m_Root = replacingNode;

            if (replacingNode != nullptr)
            {
                if (replacingNode->isLeftChild())
                {
                    nodeToRemove->setLeftChild(nullptr);
                }
                else
                {
                    nodeToRemove->setRightChild(nullptr);
                }
            }
        }

        --m_Size;
    }

    return replacingNode;
}

BinarySearchTree::Node* BinarySearchTree::_createNewNode(int key, const std::string& value)
{
    Node* newNode{new Node{key, value}};
    return newNode;
}

void BinarySearchTree::_doClearTreeContent()
{
    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    // no need to unlink the nodes from each other as all tree nodes are being deleted
    for (std::vector<Node*>::iterator it{nodesArray.begin()}; it != nodesArray.end(); ++it)
    {
        delete(*it);
        *it = nullptr;
    }

    m_Root = nullptr;
    m_Size = 0;
}

BinarySearchTree::Node* BinarySearchTree::_findNode(int key) const
{
    Node* foundNode{nullptr};
    Node* currentNode{m_Root};

    while (currentNode != nullptr)
    {
        const int c_CurrentNodeKey{currentNode->getKey()};

        if (key < c_CurrentNodeKey)
        {
            currentNode = currentNode->getLeftChild();
        }
        else if (key > c_CurrentNodeKey)
        {
            currentNode = currentNode->getRightChild();
        }
        else
        {
            foundNode = currentNode;
            break;
        }
    }

    return foundNode;
}

void BinarySearchTree::_convertTreeToArray(std::vector<BinarySearchTree::Node*>& nodes) const
{
    if (m_Root != nullptr)
    {
        nodes.clear();
        std::vector<Node*> parentsToCheck{m_Root};

        while (parentsToCheck.size() != 0)
        {
            nodes.insert(nodes.end(), parentsToCheck.begin(), parentsToCheck.end());
            std::vector<Node*> currentParentsToCheck{std::move(parentsToCheck)};

            for (std::vector<Node*>::const_iterator it{currentParentsToCheck.cbegin()}; it != currentParentsToCheck.cend(); ++it)
            {
                Node* leftChild{(*it)->getLeftChild()};
                if (leftChild != nullptr)
                {
                    parentsToCheck.push_back(leftChild);
                }

                Node* rightChild{(*it)->getRightChild()};
                if (rightChild != nullptr)
                {
                    parentsToCheck.push_back(rightChild);
                }
            }
        }
    }
}

// a right child node is required for left rotation
void BinarySearchTree::_rotateNodeLeft(BinarySearchTree::Node* node)
{
    if (node != nullptr)
    {
        Node* rightChild{node->getRightChild()};

        if (rightChild != nullptr)
        {
            node->rotateLeft();

            if (m_Root == node)
            {
                m_Root = rightChild;
            }
        }
    }
}

// a left child node is required for right rotation
void BinarySearchTree::_rotateNodeRight(BinarySearchTree::Node* node)
{
    if (node != nullptr)
    {
        Node* leftChild{node->getLeftChild()};

        if (leftChild != nullptr)
        {
            node->rotateRight();

            if (m_Root == node)
            {
                m_Root = leftChild;
            }
        }
    }
}

void BinarySearchTree::_printNodeRelatives(const BinarySearchTree::Node* node) const
{
    auto printNodeRelativeInfo = [](const Node* node, const std::string& relativeName)
    {
        std::cout << " / " << relativeName << ": ";

        if (node != nullptr)
        {
            std::cout << node->getKey();
        }
        else
        {
            std::cout << "N";
        }
    };

    printNodeRelativeInfo(node->getLeftChild(), "Left child");
    printNodeRelativeInfo(node->getRightChild(), "Right child");
    printNodeRelativeInfo(node->getParent(), "Parent");
    printNodeRelativeInfo(node->getSibling(), "Sibling");
    printNodeRelativeInfo(node->getUncle(), "Uncle");
    printNodeRelativeInfo(node->getGrandparent(), "Grandparent");
}

std::string BinarySearchTree::_getNodeAsString(const BinarySearchTree::Node* node, bool isValueRequired) const
{
    std::string result{"NULL"};

    if (node != nullptr)
    {
        std::ostringstream nodeData;
        const Node* const parent{node->getParent()};

        nodeData << node->getKey() << ":";

        if (isValueRequired)
        {
            nodeData << node->getValue() << ":";
        }

        if (parent != nullptr)
        {
            nodeData << parent->getKey();

            if (node->getSibling() == nullptr)
            {
                const bool isNodeLeftChild{node->isLeftChild()};

                assert(isNodeLeftChild || node->isRightChild()); // defensive programming, node should not be root

                if (isNodeLeftChild)
                {
                    nodeData << "L";
                }
                else
                {
                    nodeData << "R";
                }
            }
        }
        else
        {
            nodeData << "ROOT";
        }

        result = nodeData.str();
    }

    return result;
}

bool BinarySearchTree::_isEqualTo(const BinarySearchTree& tree) const
{
    bool areEqualTrees{true};

    if (this != &tree)
    {
        std::vector<Node*> nodes;
        std::vector<Node*> treeNodes;

        _convertTreeToArray(nodes);
        tree._convertTreeToArray(treeNodes);

        if (nodes.size() != treeNodes.size())
        {
            areEqualTrees = false;
        }
        else
        {
            for (size_t index{0}; index < nodes.size(); ++index)
            {
                if (*(nodes[index]) != *(treeNodes[index]))
                {
                    areEqualTrees = false;
                    break;
                }
            }
        }
    }

    return areEqualTrees;
}

BinarySearchTree::Node::Node(int key, std::string value)
    : m_Parent{nullptr}
    , m_LeftChild{nullptr}
    , m_RightChild{nullptr}
    , m_Key{key}
    , m_Value{value}
{
}

BinarySearchTree::Node::~Node()
{
}

int BinarySearchTree::Node::getKey() const
{
    return m_Key;
}

void BinarySearchTree::Node::setValue(const std::string& value)
{
    m_Value = value;
}

std::string BinarySearchTree::Node::getValue() const
{
    return m_Value;
}

bool BinarySearchTree::Node::isLeftChild() const
{
    return (m_Parent != nullptr && m_Parent->m_LeftChild == this);
}

bool BinarySearchTree::Node::isRightChild() const
{
    return (m_Parent != nullptr && m_Parent->m_RightChild == this);
}

/* It is the responsibility of the tree object to ensure that the correct node is added as left child and that the tree rules are followed
   (e.g. don't add root as left child of a sub-node prior to decoupling it from its children */
void BinarySearchTree::Node::setLeftChild(Node* leftChild)
{
    if (m_LeftChild != leftChild)
    {
        // ensure old left child gets decoupled from parent
        if (m_LeftChild != nullptr)
        {
            m_LeftChild->m_Parent = nullptr;
        }

        // ensure added left child gets decoupled from parent (if any) and coupled to actual parent
        if (leftChild != nullptr)
        {
            if (leftChild->m_Parent != nullptr)
            {
                if (leftChild->m_Parent->m_LeftChild == leftChild)
                {
                    leftChild->m_Parent->m_LeftChild = nullptr;
                }
                else
                {
                    leftChild->m_Parent->m_RightChild = nullptr;
                }
            }

            leftChild->m_Parent = this;
        }

        m_LeftChild = leftChild;
    }
}

BinarySearchTree::Node* BinarySearchTree::Node::getLeftChild() const
{
    return m_LeftChild;
}

/* It is the responsibility of the tree object to ensure that the correct node is added as right child and that the tree rules are followed
   (e.g. don't add root as right child of a sub-node prior to decoupling it from its children */
void BinarySearchTree::Node::setRightChild(BinarySearchTree::Node* rightChild)
{
    if (m_RightChild != rightChild)
    {
        // ensure old right child gets decoupled from parent
        if (m_RightChild != nullptr)
        {
            m_RightChild->m_Parent = nullptr;
        }

        // ensure added right child gets decoupled from old parent (if any) and coupled to actual parent
        if (rightChild != nullptr)
        {
            if (rightChild->m_Parent != nullptr)
            {
                if (rightChild->m_Parent->m_LeftChild == rightChild)
                {
                    rightChild->m_Parent->m_LeftChild = nullptr;
                }
                else
                {
                    rightChild->m_Parent->m_RightChild = nullptr;
                }
            }

            rightChild->m_Parent = this;
        }

        m_RightChild = rightChild;
    }
}

BinarySearchTree::Node *BinarySearchTree::Node::getRightChild() const
{
    return m_RightChild;
}

void BinarySearchTree::Node::copyInOrderSuccessorKeyAndValue()
{
    Node* successor{getInOrderSuccessor()};

    if (successor != nullptr)
    {
        m_Key = successor->m_Key;
        m_Value = successor->m_Value;
    }
}

BinarySearchTree::Node* BinarySearchTree::Node::getInOrderSuccessor() const
{
    Node* inOrderSuccessor{m_RightChild};

    if (m_RightChild != nullptr)
    {
        Node* successorChild{inOrderSuccessor->m_LeftChild};

        while (successorChild != nullptr)
        {
            inOrderSuccessor = successorChild;
            successorChild = inOrderSuccessor->m_LeftChild;
        }
    }

    return inOrderSuccessor;
}

void BinarySearchTree::Node::rotateLeft()
{
    assert(m_RightChild != nullptr && "Attempt to left rotate node without right child");

    Node* rightChild{m_RightChild};
    Node* rightLeftChild{m_RightChild->m_LeftChild};
    Node* parent{m_Parent};

    // current node becomes left child of its actual right child
    m_RightChild->m_LeftChild = this;
    m_Parent = rightChild;

    // left child of actual right child becomes right child of current node
    if (rightLeftChild != nullptr)
    {
        rightLeftChild->m_Parent = this;
    }

    m_RightChild = rightLeftChild;

    // parent of current node (if any) becomes parent of actual right child (the new child remains same type of child for parent as before)
    if (parent != nullptr)
    {
        if (parent->m_LeftChild == this)
        {
            parent->m_LeftChild = rightChild;
            rightChild->m_Parent = parent;
        }
        else
        {
            parent->m_RightChild = rightChild;
            rightChild->m_Parent = parent;
        }
    }
    else
    {
        rightChild->m_Parent = nullptr;
    }
}

void BinarySearchTree::Node::rotateRight()
{
    assert(m_LeftChild != nullptr && "Attempt to right rotate node without left child");

    Node* leftChild{m_LeftChild};
    Node* leftRightChild{m_LeftChild->m_RightChild};
    Node* parent{m_Parent};

    // current node becomes right child of its actual left child
    m_LeftChild->m_RightChild = this;
    m_Parent = leftChild;

    // right child of actual left child becomes left child of current node
    if (leftRightChild != nullptr)
    {
        leftRightChild->m_Parent = this;
    }

    m_LeftChild = leftRightChild;

    // parent of current node (if any) becomes parent of actual left child (the new child remains same type of child for parent as before)
    if (parent != nullptr)
    {
        if (parent->m_LeftChild == this)
        {
            parent->m_LeftChild = leftChild;
            leftChild->m_Parent = parent;
        }
        else
        {
            parent->m_RightChild = leftChild;
            leftChild->m_Parent = parent;
        }
    }
    else
    {
        leftChild->m_Parent = nullptr;
    }
}

BinarySearchTree::Node* BinarySearchTree::Node::getParent() const
{
    return m_Parent;
}

BinarySearchTree::Node* BinarySearchTree::Node::getSibling() const
{
    Node* result{nullptr};

    if (m_Parent != nullptr)
    {
        if (m_Parent->m_LeftChild == this)
        {
            result = m_Parent->m_RightChild;
        }
        else
        {
            result = m_Parent->m_LeftChild;
        }
    }

    return result;
}

BinarySearchTree::Node* BinarySearchTree::Node::getUncle() const
{
    Node* result{nullptr};

    const Node* c_GrandParent{getGrandparent()};

    if (c_GrandParent)
    {
        if (c_GrandParent->m_LeftChild == m_Parent)
        {
            result = c_GrandParent->m_RightChild;
        }
        else
        {
            result = c_GrandParent->m_LeftChild;
        }
    }

    return result;
}

BinarySearchTree::Node* BinarySearchTree::Node::getGrandparent() const
{
    Node* result{nullptr};

    if (m_Parent != nullptr)
    {
        result = m_Parent->getParent();
    }

    return result;
}

bool BinarySearchTree::Node::operator!=(const BinarySearchTree::Node& node) const
{
    bool areNotEqualNodes{false};

    if (m_Parent != nullptr)
    {
        assert(node.m_Parent != nullptr); // defensive programming, a non-root node should only be compared with another non-root node

        if (m_Parent->m_Key != node.m_Parent->m_Key)
        {
            areNotEqualNodes = true;
        }
        else
        {
            assert(m_Parent->m_Value == node.m_Parent->m_Value); // defensive programming, it is presumed that the parent nodes have already been compared and have equal keys and values

            const bool isCurrentNodeLeftChild{isLeftChild()};
            const bool isOtherNodeLeftChild{node.isLeftChild()};

            assert((isCurrentNodeLeftChild || isRightChild()) && (isOtherNodeLeftChild || node.isRightChild())); // defensive programming, neither node should be root

            if ((isCurrentNodeLeftChild && !isOtherNodeLeftChild) || (!isCurrentNodeLeftChild && isOtherNodeLeftChild) || (m_Key != node.m_Key || m_Value != node.m_Value))
            {
                areNotEqualNodes = true;
            }
        }
    }
    else
    {
        assert(node.m_Parent == nullptr); // defensive programming, the root node should only be compared with another root

        if (m_Key != node.m_Key || m_Value != node.m_Value)
        {
            areNotEqualNodes = true;
        }
    }

    return areNotEqualNodes;
}
