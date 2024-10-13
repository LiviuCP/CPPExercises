#include <iostream>
#include <sstream>

#include <cassert>

#include "alternativebinarysearchtree.h"

bool AlternativeBinarySearchTree::sLoggingEnabled = false;

AlternativeBinarySearchTree::AlternativeBinarySearchTree(const std::string& nullValue)
    : m_Root{nullptr}
    , m_NullValue{nullValue}
    , m_Size{0}
{
}

AlternativeBinarySearchTree::AlternativeBinarySearchTree(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
    : AlternativeBinarySearchTree{nullValue}
{
    if (defaultValue != nullValue)
    {
        _createTreeStructure(inputKeys, defaultValue, nullValue);
    }
}

AlternativeBinarySearchTree::AlternativeBinarySearchTree(const AlternativeBinarySearchTree& sourceTree)
    : m_Root{nullptr}
    , m_NullValue{sourceTree.m_NullValue}
    , m_Size{0}
{
    _copyTreeNodes(sourceTree);
}

AlternativeBinarySearchTree::AlternativeBinarySearchTree(AlternativeBinarySearchTree&& sourceTree)
    : m_Root{sourceTree.m_Root}
    , m_NullValue{sourceTree.m_NullValue}
    , m_Size{sourceTree.m_Size}
{
    sourceTree.m_Root = nullptr;
    sourceTree.m_Size = 0;
}

bool AlternativeBinarySearchTree::addOrUpdateNode(int key, const std::string& value)
{
    bool newNodeAdded{false};

    if (value != m_NullValue)
    {
        spNode const addedNode{_doAddOrUpdateNode(key, value)};
        newNodeAdded = addedNode != nullptr;
    }

    return newNodeAdded;
}

bool AlternativeBinarySearchTree::removeNode(int key)
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

bool AlternativeBinarySearchTree::mergeTree(AlternativeBinarySearchTree& sourceTree)
{
    bool merged{false};

    if (m_NullValue == sourceTree.m_NullValue && this != &sourceTree)
    {
        _moveTreeNodes(sourceTree);
        merged = true;
    }

    return merged;
}

void AlternativeBinarySearchTree::clear()
{
    m_Root.reset();
    m_Size = 0;
}

AlternativeBinarySearchTree& AlternativeBinarySearchTree::operator=(const AlternativeBinarySearchTree& sourceTree)
{
    if (this != &sourceTree)
    {
        if (m_Root != nullptr)
        {
            clear();
        }

        m_NullValue = sourceTree.m_NullValue;
        _copyTreeNodes(sourceTree);
    }

    return *this;
}

AlternativeBinarySearchTree& AlternativeBinarySearchTree::operator=(AlternativeBinarySearchTree&& sourceTree)
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

bool AlternativeBinarySearchTree::operator==(const AlternativeBinarySearchTree& tree) const
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

std::string AlternativeBinarySearchTree::getNodeValue(int key) const
{
    spNode const foundNode{_findNode(key)};
    const std::string c_Result{foundNode ? foundNode->getValue() : m_NullValue};

    return c_Result;
}

std::string AlternativeBinarySearchTree::getNullValue() const
{
    return m_NullValue;
}

int AlternativeBinarySearchTree::getSize() const
{
    return m_Size;
}

void AlternativeBinarySearchTree::printTree() const
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

std::string AlternativeBinarySearchTree::getTreeAsString(bool areNodeValuesRequired) const
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

void AlternativeBinarySearchTree::enableLogging(bool enable)
{
    if (sLoggingEnabled != enable)
    {
        sLoggingEnabled = enable;
    }
}

void AlternativeBinarySearchTree::_createTreeStructure(const std::vector<int>& inputKeys, const std::string& defaultValue, const std::string& nullValue)
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

void AlternativeBinarySearchTree::_copyTreeNodes(const AlternativeBinarySearchTree& sourceTree)
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

void AlternativeBinarySearchTree::_moveTreeNodes(AlternativeBinarySearchTree& sourceTree)
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

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::_doAddOrUpdateNode(int key, const std::string& value)
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

void AlternativeBinarySearchTree::_removeSingleChildedOrLeafNode(AlternativeBinarySearchTree::spNode nodeToRemove)
{
    if (nodeToRemove)
    {
        spNode const leftChild{nodeToRemove->getLeftChild()};
        spNode const rightChild{nodeToRemove->getRightChild()};

        if (!leftChild || !rightChild)
        {
            // determine replacing node (null if leaf node is removed)
            spNode const replacingNode{leftChild ? leftChild : rightChild ? rightChild : nullptr};

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
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::_createNewNode(int key, const std::string& value)
{
    return std::make_shared<Node>(key, value);
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::_findNode(int key) const
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

void AlternativeBinarySearchTree::_convertTreeToArray(std::vector<AlternativeBinarySearchTree::spNode>& nodes) const
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

void AlternativeBinarySearchTree::_printNodeRelatives(const AlternativeBinarySearchTree::spNode node) const
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

std::string AlternativeBinarySearchTree::_getNodeAsString(const spNode node, bool isValueRequired) const
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

AlternativeBinarySearchTree::InOrderForwardIterator AlternativeBinarySearchTree::begin()
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

AlternativeBinarySearchTree::InOrderForwardIterator AlternativeBinarySearchTree::end()
{
    return InOrderForwardIterator{nullptr, m_NullValue};
}

AlternativeBinarySearchTree::InOrderForwardIterator AlternativeBinarySearchTree::find(int key)
{
    spNode const currentNode{_findNode(key)};
    return InOrderForwardIterator{currentNode, m_NullValue};
}

AlternativeBinarySearchTree::InOrderForwardIterator AlternativeBinarySearchTree::root()
{
    return InOrderForwardIterator{m_Root, m_NullValue};
}

AlternativeBinarySearchTree::Node::Node(int key, std::string value)
    : m_LeftChild{nullptr}
    , m_RightChild{nullptr}
    , m_Key{key}
    , m_Value{value}
{
    m_Parent.reset();
}

int AlternativeBinarySearchTree::Node::getKey() const
{
    return m_Key;
}

void AlternativeBinarySearchTree::Node::setValue(const std::string& value)
{
    m_Value = value;
}

std::string AlternativeBinarySearchTree::Node::getValue() const
{
    return m_Value;
}

bool AlternativeBinarySearchTree::Node::isLeftChild() const
{
    spNode const parent{m_Parent.lock()};
    const bool c_IsNodeLeftChild{parent && parent->m_LeftChild.get() == this};

    return c_IsNodeLeftChild;
}

bool AlternativeBinarySearchTree::Node::isRightChild() const
{
    spNode const parent{m_Parent.lock()};
    const bool c_IsNodeRightChild{parent && parent->m_RightChild.get() == this};

    return c_IsNodeRightChild;
}

/* It is the responsibility of the tree object to ensure that the correct node is added as left child and that the tree rules are followed
   (e.g. don't add root as left child of a sub-node prior to decoupling it from its children */
void AlternativeBinarySearchTree::Node::setLeftChild(AlternativeBinarySearchTree::spNode leftChild)
{
    if (m_LeftChild != leftChild)
    {
        // ensure old left child gets decoupled from parent
        if (m_LeftChild)
        {
            m_LeftChild->m_Parent.reset();
        }

        // ensure added left child gets decoupled from parent (if any) and coupled to actual parent
        if (leftChild)
        {
            spNode const parent{leftChild->m_Parent.lock()};

            if (parent)
            {
                if (parent->m_LeftChild == leftChild)
                {
                    parent->m_LeftChild = nullptr;
                }
                else
                {
                    parent->m_RightChild = nullptr;
                }
            }
        }

        m_LeftChild = leftChild;
    }
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::Node::getLeftChild() const
{
    return m_LeftChild;
}

/* It is the responsibility of the tree object to ensure that the correct node is added as right child and that the tree rules are followed
   (e.g. don't add root as right child of a sub-node prior to decoupling it from its children */
void AlternativeBinarySearchTree::Node::setRightChild(spNode rightChild)
{
    if (m_RightChild != rightChild)
    {
        // ensure old right child gets decoupled from parent
        if (m_RightChild)
        {
            m_RightChild->m_Parent.reset();
        }

        // ensure added right child gets decoupled from old parent (if any) and coupled to actual parent
        if (rightChild)
        {
            spNode const parent{rightChild->m_Parent.lock()};

            if (parent)
            {
                if (parent->m_LeftChild == rightChild)
                {
                    parent->m_LeftChild = nullptr;
                }
                else
                {
                    parent->m_RightChild = nullptr;
                }
            }
        }

        m_RightChild = rightChild;
    }
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::Node::getRightChild() const
{
    return m_RightChild;
}

void AlternativeBinarySearchTree::Node::copyInOrderSuccessorKeyAndValue()
{
    spNode const successor{getInOrderSuccessor()};

    if (successor)
    {
        m_Key = successor->m_Key;
        m_Value = successor->m_Value;
    }
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::Node::getInOrderSuccessor() const
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

void AlternativeBinarySearchTree::Node::setParent(spNode parent)
{
    if (parent)
    {
        if (parent->m_LeftChild.get() == this || parent->m_RightChild.get() == this)
        {
            m_Parent = parent;
        }
        else
        {
            m_Parent.reset();
            assert(false && "Invalid parent, current node is none of its children!");
        }
    }
    else
    {
        m_Parent = parent;
    }
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::Node::getParent() const
{
    return m_Parent.lock();
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::Node::getSibling() const
{
    spNode result{nullptr};

    if (spNode const parent{m_Parent.lock()}; parent)
    {
        result = parent->m_LeftChild.get() != this ? parent->m_LeftChild : parent->m_RightChild;
    }

    return result;
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::Node::getUncle() const
{
    spNode result{nullptr};

    if (spNode const grandParent{getGrandparent()}; grandParent)
    {
        result = grandParent->m_LeftChild.get() != m_Parent.lock().get() ? grandParent->m_LeftChild: grandParent->m_RightChild;
    }

    return result;
}

AlternativeBinarySearchTree::spNode AlternativeBinarySearchTree::Node::getGrandparent() const
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
bool AlternativeBinarySearchTree::Node::operator==(const AlternativeBinarySearchTree::Node& node) const
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

AlternativeBinarySearchTree::InOrderForwardIterator::InOrderForwardIterator()
{
    m_Node.reset();
}

AlternativeBinarySearchTree::InOrderForwardIterator::InOrderForwardIterator(spNode node, const std::string& nullValue)
    : m_NullValue{nullValue}
{
    m_Node = node;
}

void AlternativeBinarySearchTree::InOrderForwardIterator::next()
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

int AlternativeBinarySearchTree::InOrderForwardIterator::getKey() const
{
    int result{std::numeric_limits<int>::max()};

    if (spNode const node{m_Node.lock()}; node)
    {
        result = node->getKey();
    }

    return result;
}

void AlternativeBinarySearchTree::InOrderForwardIterator::setValue(const std::string& value)
{
    if (spNode const node{m_Node.lock()}; node)
    {
        node->setValue(value);
    }
}

std::string AlternativeBinarySearchTree::InOrderForwardIterator::getValue() const
{
    std::string result{m_NullValue};

    if (spNode const node{m_Node.lock()}; node)
    {
        result = node->getValue();
    }

    return result;
}

AlternativeBinarySearchTree::InOrderForwardIterator& AlternativeBinarySearchTree::InOrderForwardIterator::operator=(const InOrderForwardIterator& other)
{
    m_Node = other.m_Node;
    m_NullValue = other.m_NullValue;

    return *this;
}

bool AlternativeBinarySearchTree::InOrderForwardIterator::operator==(const InOrderForwardIterator& other) const
{
    return m_Node.lock() == other.m_Node.lock() && m_NullValue == other.m_NullValue;
}
