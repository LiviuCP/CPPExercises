#include <iostream>

#include "binarysearchtree.h"

SimpleBST::SimpleBST(std::string defaultNullValue)
    : m_Root{nullptr}
    , m_DefaultNullValue{defaultNullValue}
{
}

SimpleBST::~SimpleBST()
{
    std::vector<Node*> nodesArray;
    _convertTreeToArray(nodesArray);

    for (std::vector<Node*>::iterator it{nodesArray.begin()}; it != nodesArray.end(); ++it)
    {
        delete(*it);
        *it = nullptr;
    }

    m_Root = nullptr;
}

bool SimpleBST::addOrUpdateNode(int key, const std::string& value)
{
    bool newNodeAdded{false};

    if (value != m_DefaultNullValue)
    {
        if (m_Root == nullptr)
        {
            m_Root = new Node{key, value};
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
    }

    return newNodeAdded;
}

std::string SimpleBST::getNodeValue(int key) const
{
    std::string result{m_DefaultNullValue};

    Node* foundNode{_findNode(key)};

    if (foundNode != nullptr)
    {
        result = foundNode->getValue();
    }

    return result;
}

void SimpleBST::printNodesInfo() const
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
}

SimpleBST::Node* SimpleBST::_findNode(int key) const
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

void SimpleBST::_convertTreeToArray(std::vector<SimpleBST::Node*>& nodes) const
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

SimpleBST::Node::Node(int key, std::string value)
    : m_Parent{nullptr}
    , m_LeftChild{nullptr}
    , m_RightChild{nullptr}
    , m_Key{key}
    , m_Value{value}
{
}

int SimpleBST::Node::getKey() const
{
    return m_Key;
}

void SimpleBST::Node::setValue(const std::string& value)
{
    m_Value = value;
}

std::string SimpleBST::Node::getValue() const
{
    return m_Value;
}

bool SimpleBST::Node::isLeftChild() const
{
    return (m_Parent && (m_Parent->m_LeftChild == this));
}

bool SimpleBST::Node::isRightChild() const
{
    return (m_Parent && (m_Parent->m_RightChild == this));
}

void SimpleBST::Node::setLeftChild(Node* left)
{
    m_LeftChild = left;

    if (m_LeftChild)
    {
        m_LeftChild->m_Parent  = this;
    }
}

SimpleBST::Node* SimpleBST::Node::getLeftChild() const
{
    return m_LeftChild;
}

void SimpleBST::Node::setRightChild(SimpleBST::Node* right)
{
    m_RightChild = right;

    if (m_RightChild)
    {
        m_RightChild->m_Parent = this;
    }
}

SimpleBST::Node *SimpleBST::Node::getRightChild() const
{
    return m_RightChild;
}

SimpleBST::Node* SimpleBST::Node::getParent() const
{
    return m_Parent;
}

SimpleBST::Node* SimpleBST::Node::getUncle() const
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

SimpleBST::Node* SimpleBST::Node::getGrandparent() const
{
    Node* result{nullptr};

    if (m_Parent)
    {
        result = m_Parent->getParent();
    }

    return result;
}
