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
    if (defaultValue != nullValue)
    {
        _createTreeStructure(inputKeys, defaultValue, nullValue);
    }
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& sourceTree)
    : BinarySearchTree{sourceTree.m_NullValue}
{
    // temporary object is required in order to avoid directly calling _copyTreeNodes() which contains calls to virtual methods
    BinarySearchTree temp;
    temp = sourceTree;

    // move temporary object to current object
    *this = std::move(temp);
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& sourceTree)
    : m_Root{sourceTree.m_Root}
    , m_NullValue{sourceTree.m_NullValue}
    , m_Size{sourceTree.m_Size}
{
    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

bool BinarySearchTree::addOrUpdateNode(int key, const std::string& value)
{
    bool newNodeAdded{false};

    if (value != m_NullValue)
    {
        spNode const addedNode{_doAddOrUpdateNode(key, value)};
        newNodeAdded = addedNode != nullptr;
    }

    return newNodeAdded;
}

bool BinarySearchTree::removeNode(int key)
{
    bool removed{false};
    spNode nodeToRemove{_findNode(key)};

    if (nodeToRemove)
    {
        // for two-children node to be removed: the in-order successor content will be copied to the node; then the successor will be recursively removed
        if (nodeToRemove->getLeftChild() && nodeToRemove->getRightChild())
        {
            nodeToRemove->copyInOrderSuccessorKeyAndValue();
            nodeToRemove = nodeToRemove->getInOrderSuccessor();
        }

        _removeSingleChildedOrLeafNode(nodeToRemove);
        removed = true;
    }

    return removed;
}

bool BinarySearchTree::mergeTree(BinarySearchTree& sourceTree)
{
    bool merged{false};

    if (m_NullValue == sourceTree.m_NullValue && this != &sourceTree)
    {
        _moveTreeNodes(sourceTree);
        merged = true;
    }

    return merged;
}

void BinarySearchTree::clear()
{
    m_Root.reset();
    m_Size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root)
        {
            clear();
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
        if (m_Root)
        {
            clear();
        }

        m_NullValue = sourceTree.m_NullValue;
        _moveTreeNodes(sourceTree);
    }

    return *this;
}

bool BinarySearchTree::operator==(const BinarySearchTree& tree) const
{
    bool areEqualTrees{true};

    if (this != &tree)
    {
        std::vector<spNode> nodes;
        std::vector<spNode> treeNodes;

        _convertTreeToArray(nodes);
        tree._convertTreeToArray(treeNodes);

        areEqualTrees = std::equal(nodes.cbegin(), nodes.cend(), treeNodes.cbegin(), treeNodes.cend(),
                                   [](const spNode& node, const spNode& treeNode) {return *node == *treeNode;});
    }

    return areEqualTrees;
}

std::string BinarySearchTree::getNodeValue(int key) const
{
    spNode const foundNode{_findNode(key)};
    const std::string c_Result{foundNode ? foundNode->getValue() : m_NullValue};

    return c_Result;
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
    std::vector<spNode> nodesArray;
    _convertTreeToArray(nodesArray);

    for (const auto& node: nodesArray)
    {
        std::cout << "Node: " << node->getKey();
        std::cout << " / Is child: " << (node->isLeftChild() ? "L" : node->isRightChild() ? "R" : "N");

        _printNodeRelatives(node);

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

    std::vector<spNode> nodesArray;
    _convertTreeToArray(nodesArray);

    for (const auto& node : nodesArray)
    {
        result += _getNodeAsString(node, areNodeValuesRequired);
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
    if (defaultValue != nullValue)
    {
        for (const auto& key : inputKeys)
        {
            spNode const addedNode{_doAddOrUpdateNode(key, defaultValue)};

            if (sLoggingEnabled && !addedNode)
            {
                std::clog << "Warning: duplicate key found: " << key << std::endl;
            }
        }
    }
    else
    {
        assert(false && "Null value set as default value for tree keys");
    }
}

void BinarySearchTree::_copyTreeNodes(const BinarySearchTree& sourceTree)
{
    if (m_NullValue == sourceTree.m_NullValue)
    {
        std::vector<spNode> sourceTreeArray;
        sourceTree._convertTreeToArray(sourceTreeArray);

        for (const auto& node : sourceTreeArray)
        {
            spNode const addedNode{_doAddOrUpdateNode(node->getKey(), node->getValue())};

            if (sLoggingEnabled && !addedNode)
            {
                std::clog << "Warning: node " << node->getKey() << " already present. Value overridden" << std::endl;
            }
        }
    }
    else
    {
        assert(false && "Null values of trees don't match");
    }
}

void BinarySearchTree::_moveTreeNodes(BinarySearchTree& sourceTree)
{
    if (m_NullValue == sourceTree.m_NullValue)
    {
        if (this != &sourceTree)
        {
            _copyTreeNodes(sourceTree);
            sourceTree.clear();
        }
        else
        {
            assert(false && "The tree cannot move nodes within itself!");
        }
    }
    else
    {
        assert(false && "Null values of trees don't match");
    }
}

BinarySearchTree::spNode BinarySearchTree::_doAddOrUpdateNode(int key, const std::string& value)
{
    spNode addedNode{nullptr};

    if (m_Root)
    {
        spNode currentNode{m_Root};
        bool entryPointFound{false};

        while (!entryPointFound)
        {
            const int c_CurrentNodeKey{currentNode->getKey()};

            if (key < c_CurrentNodeKey)
            {
                if (spNode const leftChild{currentNode->getLeftChild()}; leftChild)
                {
                    currentNode = leftChild;
                    continue;
                }

                addedNode = _createNewNode(key, value);
                currentNode->setLeftChild(addedNode);
                addedNode->setParent(currentNode);
                entryPointFound = true;
            }
            else if (key > c_CurrentNodeKey)
            {
                if (spNode const rightChild{currentNode->getRightChild()}; rightChild)
                {
                    currentNode = rightChild;
                    continue;
                }

                addedNode = _createNewNode(key, value);
                currentNode->setRightChild(addedNode);
                addedNode->setParent(currentNode);
                entryPointFound = true;
            }
            else
            {
                currentNode->setValue(value);
                entryPointFound = true;
            }
        }
    }
    else
    {
        m_Root = _createNewNode(key, value);
        addedNode = m_Root;
    }

    if (addedNode)
    {
        ++m_Size;
    }

    return addedNode;
}

BinarySearchTree::spNode BinarySearchTree::_removeSingleChildedOrLeafNode(spNode nodeToRemove)
{
    spNode replacingNode{nullptr};

    if (nodeToRemove)
    {
        spNode const leftChild{nodeToRemove->getLeftChild()};
        spNode const rightChild{nodeToRemove->getRightChild()};

        if (!leftChild || !rightChild)
        {
            // determine replacing node (null if leaf node is removed)
            replacingNode = leftChild ? leftChild : rightChild ? rightChild : nullptr;

            // parent of removed node to be stored in advance in order to make re-parenting of the replacing node possible (it gets decoupled from the node to remove when setting new child)
            spNode const nodeToRemoveParent{nodeToRemove->getParent()};

            // remove and replace
            if (nodeToRemove->isLeftChild())
            {
                nodeToRemoveParent->setLeftChild(replacingNode);

                if (replacingNode)
                {
                    replacingNode->setParent(nodeToRemoveParent);
                }
            }
            else if (nodeToRemove->isRightChild())
            {
                nodeToRemoveParent->setRightChild(replacingNode);

                if (replacingNode)
                {
                    replacingNode->setParent(nodeToRemoveParent);
                }
            }
            else
            {
                m_Root = replacingNode;

                if (replacingNode)
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
        else
        {
            assert(false && "Attempt to use the remove function on a node with two children");
        }
    }

    return replacingNode;
}

BinarySearchTree::spNode BinarySearchTree::_createNewNode(int key, const std::string& value)
{
    return std::make_shared<Node>(key, value);
}

BinarySearchTree::spNode BinarySearchTree::_findNode(int key) const
{
    spNode foundNode{nullptr};
    spNode currentNode{m_Root};

    while (currentNode)
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

void BinarySearchTree::_convertTreeToArray(std::vector<spNode>& nodes) const
{
    nodes.clear();

    if (m_Root)
    {
        nodes.push_back(m_Root);
        std::vector<spNode> parentsToCheck{m_Root};

        while (parentsToCheck.size() != 0)
        {
            std::vector<spNode> childrenToCheck;

            for (const auto& parent : parentsToCheck)
            {
                if (spNode const leftChild{parent->getLeftChild()}; leftChild)
                {
                    childrenToCheck.push_back(leftChild);
                }

                if (spNode const rightChild{parent->getRightChild()}; rightChild)
                {
                    childrenToCheck.push_back(rightChild);
                }
            }

            nodes.insert(nodes.end(), childrenToCheck.begin(), childrenToCheck.end());
            parentsToCheck = std::move(childrenToCheck);
        }
    }
}

// a right child node is required for left rotation
void BinarySearchTree::_rotateNodeLeft(spNode node)
{
    if (node)
    {
        spNode const rightChild{node->getRightChild()};

        if (rightChild)
        {
            spNode const rightLeftChild{rightChild->getLeftChild()};
            spNode const parent{node->getParent()};

            // current node becomes left child of its actual right child
            rightChild->setLeftChild(node);
            node->setParent(rightChild);

            // left child of actual right child becomes right child of current node
            node->setRightChild(rightLeftChild);

            if (rightLeftChild)
            {
                rightLeftChild->setParent(node);
            }

            // parent of current node (if any) becomes parent of actual right child (the new child remains same type of child for parent as before)
            if (parent)
            {
                if (parent->getLeftChild() == node)
                {
                    parent->setLeftChild(rightChild);
                    rightChild->setParent(parent);
                }
                else
                {
                    parent->setRightChild(rightChild);
                    rightChild->setParent(parent);
                }
            }
            else
            {
                rightChild->setParent(nullptr);
            }

            if (m_Root == node)
            {
                m_Root = rightChild;
            }
        }
    }
}

// a left child node is required for right rotation
void BinarySearchTree::_rotateNodeRight(spNode node)
{
    if (node)
    {
        spNode const leftChild{node->getLeftChild()};

        if (leftChild)
        {
            spNode const leftRightChild{leftChild->getRightChild()};
            spNode const parent{node->getParent()};

            // current node becomes right child of its actual left child
            leftChild->setRightChild(node);
            node->setParent(leftChild);

            // right child of actual left child becomes left child of current node
            node->setLeftChild(leftRightChild);

            if (leftRightChild)
            {
                leftRightChild->setParent(node);
            }

            // parent of current node (if any) becomes parent of actual left child (the new child remains same type of child for parent as before)
            if (parent)
            {
                if (parent->getLeftChild() == node)
                {
                    parent->setLeftChild(leftChild);
                    leftChild->setParent(parent);
                }
                else
                {
                    parent->setRightChild(leftChild);
                    leftChild->setParent(parent);
                }
            }
            else
            {
                leftChild->setParent(nullptr);
            }

            if (m_Root == node)
            {
                m_Root = leftChild;
            }
        }
    }
}

void BinarySearchTree::_printNodeRelatives(spNode node) const
{
    if (node)
    {
        auto printNodeRelativeInfo = [](const spNode node, const std::string& relativeName)
        {
            std::cout << " / " << relativeName << ": ";

            if (node)
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
    else
    {
        assert(false);
    }
}

std::string BinarySearchTree::_getNodeAsString(spNode node, bool isValueRequired) const
{
    std::string result{"NULL"};

    if (node)
    {
        std::ostringstream nodeData;
        spNode const parent{node->getParent()};

        nodeData << node->getKey() << ":";

        if (isValueRequired)
        {
            nodeData << node->getValue() << ":";
        }

        if (parent)
        {
            nodeData << parent->getKey();

            if (!node->getSibling())
            {
                nodeData << (node->isLeftChild() ? "L" : "R");
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

BinarySearchTree::InOrderForwardIterator BinarySearchTree::begin()
{
    spNode startingNode{m_Root};

    if (startingNode)
    {
        spNode currentLeftChild{startingNode->getLeftChild()};

        while (currentLeftChild)
        {
            startingNode = currentLeftChild;
            currentLeftChild = startingNode->getLeftChild();
        }
    }

    return InOrderForwardIterator{startingNode, m_NullValue};
}

BinarySearchTree::InOrderForwardIterator BinarySearchTree::end()
{
    return InOrderForwardIterator{nullptr, m_NullValue};
}

BinarySearchTree::InOrderForwardIterator BinarySearchTree::find(int key)
{
    spNode const currentNode{_findNode(key)};
    return InOrderForwardIterator{currentNode, m_NullValue};
}

BinarySearchTree::InOrderForwardIterator BinarySearchTree::root()
{
    return InOrderForwardIterator{m_Root, m_NullValue};
}

BinarySearchTree::Node::Node(int key, std::string value)
    : m_LeftChild{nullptr}
    , m_RightChild{nullptr}
    , m_Key{key}
    , m_Value{value}
{
    m_Parent.reset();
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
    spNode const parent{m_Parent.lock()};
    const bool c_IsNodeLeftChild{parent && parent->m_LeftChild.get() == this};

    return c_IsNodeLeftChild;
}

bool BinarySearchTree::Node::isRightChild() const
{
    spNode const parent{m_Parent.lock()};
    const bool c_IsNodeRightChild{parent && parent->m_RightChild.get() == this};

    return c_IsNodeRightChild;
}

/* It is the responsibility of the tree object to ensure that the correct node is added as left child and that the tree rules are followed
   (e.g. don't add root as left child of a sub-node prior to decoupling it from its children */
void BinarySearchTree::Node::setLeftChild(spNode leftChild)
{
    m_LeftChild = leftChild;
}

BinarySearchTree::spNode BinarySearchTree::Node::getLeftChild() const
{
    return m_LeftChild;
}

/* It is the responsibility of the tree object to ensure that the correct node is added as right child and that the tree rules are followed
   (e.g. don't add root as right child of a sub-node prior to decoupling it from its children */
void BinarySearchTree::Node::setRightChild(spNode rightChild)
{
    m_RightChild = rightChild;
}

BinarySearchTree::spNode BinarySearchTree::Node::getRightChild() const
{
    return m_RightChild;
}

void BinarySearchTree::Node::copyInOrderSuccessorKeyAndValue()
{
    spNode const successor{getInOrderSuccessor()};

    if (successor)
    {
        m_Key = successor->m_Key;
        m_Value = successor->m_Value;
    }
}

BinarySearchTree::spNode BinarySearchTree::Node::getInOrderSuccessor() const
{
    spNode inOrderSuccessor{m_RightChild};

    if (m_RightChild)
    {
        spNode successorChild{inOrderSuccessor->m_LeftChild};

        while (successorChild)
        {
            inOrderSuccessor = successorChild;
            successorChild = inOrderSuccessor->m_LeftChild;
        }
    }

    return inOrderSuccessor;
}

void BinarySearchTree::Node::setParent(spNode parent)
{
    m_Parent = parent;
}

BinarySearchTree::spNode BinarySearchTree::Node::getParent() const
{
    return m_Parent.lock();
}

BinarySearchTree::spNode BinarySearchTree::Node::getSibling() const
{
    spNode result{nullptr};

    if (spNode const parent{m_Parent.lock()}; parent)
    {
        result = parent->m_LeftChild.get() != this ? parent->m_LeftChild : parent->m_RightChild;
    }

    return result;
}

BinarySearchTree::spNode BinarySearchTree::Node::getUncle() const
{
    spNode result{nullptr};

    if (spNode const grandParent{getGrandparent()}; grandParent)
    {
        result = grandParent->m_LeftChild.get() != m_Parent.lock().get() ? grandParent->m_LeftChild: grandParent->m_RightChild;
    }

    return result;
}

BinarySearchTree::spNode BinarySearchTree::Node::getGrandparent() const
{
    spNode result{nullptr};

    if (spNode const parent{m_Parent.lock()}; parent)
    {
        result = parent->getParent();
    }

    return result;
}

/* Two nodes are equal if all below conditions are fulfilled:
- they have the same key and value
- they are both in the same ordering position: root, left child or right child
- if they are both parented: the parent has the same key (not necessarily the same value)
*/
bool BinarySearchTree::Node::operator==(const BinarySearchTree::Node& node) const
{
    bool areEqualNodes{false};

    if (m_Key == node.m_Key && m_Value == node.m_Value)
    {
        spNode const parent{m_Parent.lock()};
        spNode const nodeParent{node.m_Parent.lock()};

        areEqualNodes = (!parent && !nodeParent) || (parent && nodeParent && (parent->m_Key == nodeParent->m_Key) && (isLeftChild() == node.isLeftChild()));
    }

    return areEqualNodes;
}

BinarySearchTree::InOrderForwardIterator::InOrderForwardIterator()
{
    m_Node.reset();
}

BinarySearchTree::InOrderForwardIterator::InOrderForwardIterator(spNode node, const std::string& nullValue)
    : m_NullValue{nullValue}
{
    m_Node = node;
}

void BinarySearchTree::InOrderForwardIterator::next()
{
    if (spNode const node{m_Node.lock()}; node)
    {
        spNode nextNode{nullptr};
        spNode currentChild{node->getRightChild()};

        // first check downwards by moving to the right child and then checking on the left side until no more left child is found
        while (currentChild)
        {
            nextNode = currentChild;
            currentChild = nextNode->getLeftChild();
        }

        // if no suitable child was found check upwards until a suitable parent (to which current node is right child) is found
        if (!nextNode)
        {
            spNode currentNode{node};
            spNode currentParent{node->getParent()};

            // if no suitable parent is found then the end of the tree has been reached
            while(currentParent)
            {
                if (currentNode->isLeftChild())
                {
                    nextNode = currentParent;
                    break;
                }

                currentNode = currentParent;
                currentParent = currentNode->getParent();
            }
        }

        m_Node = nextNode;
    }
}

int BinarySearchTree::InOrderForwardIterator::getKey() const
{
    int result{std::numeric_limits<int>::max()};

    if (spNode const node{m_Node.lock()}; node)
    {
        result = node->getKey();
    }

    return result;
}

void BinarySearchTree::InOrderForwardIterator::setValue(const std::string& value)
{
    if (spNode const node{m_Node.lock()}; node)
    {
        node->setValue(value);
    }
}

std::string BinarySearchTree::InOrderForwardIterator::getValue() const
{
    std::string result{m_NullValue};

    if (spNode const node{m_Node.lock()}; node)
    {
        result = node->getValue();
    }

    return result;
}

BinarySearchTree::InOrderForwardIterator& BinarySearchTree::InOrderForwardIterator::operator=(const InOrderForwardIterator& other)
{
    m_Node = other.m_Node;
    m_NullValue = other.m_NullValue;

    return *this;
}

bool BinarySearchTree::InOrderForwardIterator::operator==(const InOrderForwardIterator& other) const
{
    return m_Node.lock() == other.m_Node.lock() && m_NullValue == other.m_NullValue;
}
