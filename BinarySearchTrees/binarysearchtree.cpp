#include <iostream>
#include <cassert>

#include "binarysearchtree.h"

BinarySearchTree::BinarySearchTree(const std::string& defaultNullValue)
    : m_Root{nullptr}
    , m_DefaultNullValue{defaultNullValue}
    , m_Size{0}
{
}

BinarySearchTree::BinarySearchTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& defaultNullValue)
    : BinarySearchTree{defaultNullValue}
{
    if (inputKeys.size() != 0 && defaultValue != defaultNullValue)
    {
        for (std::vector<int>::const_iterator it{inputKeys.cbegin()}; it != inputKeys.cend(); ++it)
        {
            bool newNodeAdded{false};
            _doAddOrUpdateNode(*it, defaultValue, newNodeAdded);
            if (!newNodeAdded)
            {
                std::clog << "Warning: duplicate key found: " << *it << std::endl;
            }
        }
    }
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& sourceTree)
    : m_Root{nullptr}
    , m_DefaultNullValue{sourceTree.m_DefaultNullValue}
    , m_Size{0}
{
    _copyTreeNodes(sourceTree);
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& sourceTree)
    : m_Root{sourceTree.m_Root}
    , m_DefaultNullValue{sourceTree.m_DefaultNullValue}
    , m_Size{sourceTree.m_Size}
{
    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

BinarySearchTree::~BinarySearchTree()
{
    _deleteTreeNodes();
}

bool BinarySearchTree::addOrUpdateNode(int key, const std::string& value)
{
    bool newNodeAdded{false};

    if (value != m_DefaultNullValue)
    {
        _doAddOrUpdateNode(key, value, newNodeAdded);
    }

    return newNodeAdded;
}

bool BinarySearchTree::deleteNode(int key)
{
    Node* nodeToDelete{_findNode(key)};

    auto removeNodeWithAtMostOneChild = [nodeToDelete, this](Node* child)
    {
        if (nodeToDelete->isLeftChild())
        {
            nodeToDelete->getParent()->setLeftChild(child);
        }
        else if (nodeToDelete->isRightChild())
        {
            nodeToDelete->getParent()->setRightChild(child);
        }
        else
        {
            m_Root = child;

            if (child != nullptr)
            {
                if (child->isLeftChild())
                {
                    nodeToDelete->setLeftChild(nullptr);
                }
                else
                {
                    nodeToDelete->setRightChild(nullptr);
                }
            }
        }
    };

    auto replaceWithInOrderSuccessor = [nodeToDelete, this]()
    {
        // get in order successor
        Node* inOrderSuccessor{nodeToDelete->getRightChild()};
        Node* successorChild{inOrderSuccessor->getLeftChild()};

        while (successorChild != nullptr)
        {
            inOrderSuccessor = successorChild;
            successorChild = successorChild->getLeftChild();
        }

        // handle removed node and successor node children accordingly (successor can only have a right child) - successor becomes childless and parentless
        if (inOrderSuccessor->isLeftChild())
        {
            inOrderSuccessor->getParent()->setLeftChild(inOrderSuccessor->getRightChild());
        }
        else
        {
            inOrderSuccessor->getParent()->setRightChild(inOrderSuccessor->getRightChild());
        }

        // set successor parent node (deleted node becomes parentless)
        if (nodeToDelete->isLeftChild())
        {
            nodeToDelete->getParent()->setLeftChild(inOrderSuccessor);
        }
        else if (nodeToDelete->isRightChild())
        {
            nodeToDelete->getParent()->setRightChild(inOrderSuccessor);
        }
        else
        {
            m_Root = inOrderSuccessor;
        }

        // move any left children from removed node to successor (successor replaces deleted node)
        inOrderSuccessor->setLeftChild(nodeToDelete->getLeftChild());
        inOrderSuccessor->setRightChild(nodeToDelete->getRightChild());
    };

    bool deleted{false};

    if (nodeToDelete != nullptr)
    {
        if (nodeToDelete->getLeftChild() == nullptr && nodeToDelete->getRightChild() == nullptr)
        {
            removeNodeWithAtMostOneChild(nullptr);
        }
        else if (nodeToDelete->getLeftChild() == nullptr)
        {
            removeNodeWithAtMostOneChild(nodeToDelete->getRightChild());
        }
        else if (nodeToDelete->getRightChild() == nullptr)
        {
            removeNodeWithAtMostOneChild(nodeToDelete->getLeftChild());
        }
        else
        {
            replaceWithInOrderSuccessor();
        }

        delete nodeToDelete;
        nodeToDelete = nullptr;
        --m_Size;
        deleted = true;
    }

    return deleted;
}

void BinarySearchTree::mergeTree(BinarySearchTree& sourceTree)
{
    assert(m_DefaultNullValue == sourceTree.m_DefaultNullValue && "Default null values of trees don't match");

    if (this != &sourceTree)
    {
        _copyTreeNodes(sourceTree);
        sourceTree._deleteTreeNodes();
    }
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            _deleteTreeNodes();
        }

        m_DefaultNullValue = sourceTree.m_DefaultNullValue;
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
            _deleteTreeNodes();
        }

        m_Root = sourceTree.m_Root;
        m_Size = sourceTree.m_Size;
        sourceTree.m_Root = nullptr;
        sourceTree.m_Size = 0;
        m_DefaultNullValue = sourceTree.m_DefaultNullValue;
    }

    return *this;
}

bool BinarySearchTree::operator==(const BinarySearchTree& tree) const
{
    bool areEqualTrees{_isEqualTo(tree)};
    return areEqualTrees;
}

bool BinarySearchTree::operator!=(const BinarySearchTree &tree) const
{
    bool areNotEqualTrees{!_isEqualTo(tree)};
    return areNotEqualTrees;
}

std::string BinarySearchTree::getNodeValue(int key) const
{
    std::string result{m_DefaultNullValue};

    Node* foundNode{_findNode(key)};

    if (foundNode != nullptr)
    {
        result = foundNode->getValue();
    }

    return result;
}

int BinarySearchTree::getSize() const
{
    return m_Size;
}

void BinarySearchTree::printNodesInfo() const
{
    auto displayRelative = [](const Node* node, const std::string& relativeName)
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

    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<Node*>::const_iterator it{nodesArray.cbegin()}; it != nodesArray.cend(); ++it)
    {
        Node* currentNode{*it};

        std::cout << "Node: " << currentNode->getKey();
        std::cout << " / Is child: " << (currentNode->isLeftChild() ? "L" : currentNode->isRightChild() ? "R" : "N");

        displayRelative(currentNode->getLeftChild(), "Left child");
        displayRelative(currentNode->getRightChild(), "Right child");
        displayRelative(currentNode->getParent(), "Parent");
        displayRelative(currentNode->getUncle(), "Uncle");
        displayRelative(currentNode->getGrandparent(), "Grandparent");

        std::cout << std::endl;
    }

    if (nodesArray.size() == 0)
    {
        std::clog << "Warning: tree has no nodes" << std::endl;
    }
}

void BinarySearchTree::_doAddOrUpdateNode(int key, const std::string& value, bool& newNodeAdded)
{
    if (m_Root == nullptr)
    {
        m_Root = new Node{key, value};
        newNodeAdded = true;
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
                    currentNode->setLeftChild(new Node{key, value});
                    entryPointFound = true;
                    newNodeAdded = true;
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
                    currentNode->setRightChild(new Node{key, value});
                    entryPointFound = true;
                    newNodeAdded = true;
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

    if (newNodeAdded)
    {
        ++m_Size;
    }
}

BinarySearchTree::Node* BinarySearchTree::_findNode(int key) const
{
    Node* result{nullptr};
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
            result = currentNode;
            break;
        }
    }

    return result;
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

void BinarySearchTree::_deleteTreeNodes()
{
    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<Node*>::iterator it{nodesArray.begin()}; it != nodesArray.end(); ++it)
    {
        delete(*it);
        *it = nullptr;
    }

    m_Root = nullptr;
    m_Size = 0;
}

void BinarySearchTree::_copyTreeNodes(const BinarySearchTree& sourceTree)
{
    std::vector<Node*> sourceTreeArray;
    sourceTree._convertTreeToArray(sourceTreeArray);

    for (std::vector<Node*>::const_iterator it{sourceTreeArray.cbegin()}; it != sourceTreeArray.cend(); ++it)
    {
        bool nodeAdded{false};
        _doAddOrUpdateNode((*it)->getKey(), (*it)->getValue(), nodeAdded);

        if (!nodeAdded)
        {
            std::clog << "Warning: node " << (*it)->getKey() << " already present. Value overridden" << std::endl;
        }
    }
}

bool BinarySearchTree::_isEqualTo(const BinarySearchTree &tree) const
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

void BinarySearchTree::Node::setLeftChild(Node* left)
{
    // ensure old left child gets decoupled from parent
    if (m_LeftChild != nullptr)
    {
        m_LeftChild->m_Parent = nullptr;
    }

    // ensure added left child gets decoupled from parent (if any) and coupled to actual parent
    if (left != nullptr)
    {
        if (left->m_Parent != nullptr)
        {
            if (left->m_Parent->m_LeftChild == left)
            {
                left->m_Parent->m_LeftChild = nullptr;
            }
            else
            {
                left->m_Parent->m_RightChild = nullptr;
            }
        }

        left->m_Parent = this;
    }

    m_LeftChild = left;
}

BinarySearchTree::Node* BinarySearchTree::Node::getLeftChild() const
{
    return m_LeftChild;
}

void BinarySearchTree::Node::setRightChild(BinarySearchTree::Node* right)
{
    // ensure old right child gets decoupled from parent
    if (m_RightChild != nullptr)
    {
        m_RightChild->m_Parent = nullptr;
    }

    // ensure added right child gets decoupled from old parent (if any) and coupled to actual parent
    if (right != nullptr)
    {
        if (right->m_Parent != nullptr)
        {
            if (right->m_Parent->m_LeftChild == right)
            {
                right->m_Parent->m_LeftChild = nullptr;
            }
            else
            {
                right->m_Parent->m_RightChild = nullptr;
            }
        }

        right->m_Parent = this;
    }

    m_RightChild = right;
}

BinarySearchTree::Node *BinarySearchTree::Node::getRightChild() const
{
    return m_RightChild;
}

BinarySearchTree::Node* BinarySearchTree::Node::getParent() const
{
    return m_Parent;
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

bool BinarySearchTree::Node::operator==(const BinarySearchTree::Node& node) const
{
    bool areEqualNodes{_isEqualTo(node)};
    return areEqualNodes;
}

bool BinarySearchTree::Node::operator!=(const BinarySearchTree::Node& node) const
{
    bool areNotEqualNodes{!_isEqualTo(node)};
    return areNotEqualNodes;
}

bool BinarySearchTree::Node::_isEqualTo(const BinarySearchTree::Node &node) const
{
    bool areEqualNodes{true};

    if (m_Key != node.m_Key || m_Value != node.m_Value)
    {
        areEqualNodes = false;
    }
    else if ((m_Parent == nullptr && node.m_Parent != nullptr) || (m_Parent != nullptr && node.m_Parent == nullptr))
    {
        areEqualNodes = false;
    }
    else if ((m_Parent != nullptr) && (m_Parent->m_Key != node.m_Parent->m_Key || m_Parent->m_Value != node.m_Parent->m_Value))
    {
        areEqualNodes = false;
    }
    else
    {
        const bool isLeft{isLeftChild()};
        const bool isRight{isRightChild()};
        const bool isNodeLeft{node.isLeftChild()};
        const bool isNodeRight{node.isRightChild()};

        if ((isLeft && !isNodeLeft) || (!isLeft && isNodeLeft) || (isRight && !isNodeRight) || (!isRight && isNodeRight))
        {
            areEqualNodes = false;
        }
    }

    return areEqualNodes;
}
