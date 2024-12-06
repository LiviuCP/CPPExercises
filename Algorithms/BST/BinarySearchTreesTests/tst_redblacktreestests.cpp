#include <QtTest>
#include <algorithm>

#include "testutils.h"
#include "redblacktree.h"

using namespace TestUtils;
using IntStrRedBlackTree = RedBlackTree<int, std::string>;
using IntStrRBTIterator = IntStrRedBlackTree::InOrderForwardIterator;
using upIntStrRedBlackTree = std::unique_ptr<IntStrRedBlackTree>;

class RedBlackTreesTests : public QObject
{
    Q_OBJECT

public:
    RedBlackTreesTests();

private slots:
    void init();
    void cleanup();

    void testAddNodes();
    void testRemoveNodes();
    void testUpdateNodeValue();
    void testMoveSemantics();
    void testMergeTrees();
    void testInOrderForwardIterators();
    void testPrintTree(); // only required for improving code coverage

private:
    upIntStrRedBlackTree mpSearchTree;
    upIntStrRedBlackTree mpAuxSearchTree;
};

RedBlackTreesTests::RedBlackTreesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
    IntStrRedBlackTree::enableLogging(false);
}

void RedBlackTreesTests::init()
{
    QVERIFY(!mpSearchTree && !mpAuxSearchTree);
}

void RedBlackTreesTests::cleanup()
{
    mpSearchTree.reset();
    mpAuxSearchTree.reset();

    IntStrRedBlackTree::enableLogging(false);
}

void RedBlackTreesTests::testAddNodes()
{
    bool newNodeAdded{false};

    mpSearchTree = std::make_unique<IntStrRedBlackTree>();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT:BK", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT:BK/8:-5R:RD", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:RD/8:-1:RD", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/8:-1:BK/2:8L:RD", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-2:-5R:RD/2:8L:RD", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    qDebug("lpopa: tree is: %s", mpSearchTree->getTreeAsString().c_str());
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-2:-5R:RD/2:7:RD/8:7:RD", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-2:-5R:RD/2:7:BK/8:7:BK/0:2L:RD", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2L:RD", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2L:RD/16:8R:RD", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/14:7:BK/0:2L:RD/8:14:RD/16:14:RD", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:RD/7:-1:RD/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:BK/-23:-9L:RD/0:2L:RD/8:14:RD/16:14:RD", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-9L:RD/0:2L:RD/8:14:BK/16:14:BK/17:16R:RD", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:RD/-9:-16:RD/0:2L:RD/8:14:BK/16:14:BK/17:16R:RD", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2L:RD/8:14:BK/16:14:BK/-12:-9L:RD/17:16R:RD", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode(19, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(),"-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2L:RD/8:14:BK/17:14:BK/-12:-9L:RD/16:17:RD/19:17:RD", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-15, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-12:-16:BK/0:2L:RD/8:14:BK/17:14:BK/-15:-12:RD/-9:-12:RD/16:17:RD/19:17:RD", 16));

    // adding nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            *mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(-25, scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            *mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, scDefaultValue);
    QVERIFY(newNodeAdded &&
            *mpAuxSearchTree != *mpSearchTree);

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(25, scDefaultValue);
    QVERIFY(newNodeAdded &&
            *mpAuxSearchTree == *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/14:-1:BK/-16:-5:RD/-2:-5:BK/7:14:RD/17:14:RD/-23:-16:BK/-12:-16:BK/2:7:BK/8:7:BK/16:17:BK/19:17:BK/-15:-12:RD/-9:-12:RD/0:2L:RD/25:19R:RD", 17));

    // some additional (corner) cases
    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-12:ROOT:BK/-16:-12:BK/-1:-12:RD/-23:-16:BK/-15:-16:BK/-5:-1:BK/8:-1:BK/-9:-5:BK/-2:-5:BK/2:8:RD/16:8:RD/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17R:RD", 16));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "14:ROOT:BK/0:14:RD/17:14:BK/-9:0:BK/7:0:BK/16:17:BK/19:17:BK/-15:-9:RD/-2:-9:RD/2:7:BK/8:7:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/-23:-16L:RD", 16));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-12:ROOT:BK/-16:-12:BK/14:-12:RD/-23:-16:BK/-15:-16:BK/-1:14:BK/17:14:BK/-5:-1:RD/7:-1:RD/16:17:BK/19:17:BK/-9:-5:BK/-2:-5:BK/2:7:BK/8:7:BK/0:2L:RD", 16));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 2, -3, 2, 4, 0, 1}, scDefaultValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, -3, 2, 4, 0, 1}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree); // in this particular case due to RB tree construction rules the trees become equal when third element is being added (unlike the basic BST)

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 2, -3, 4, 1, 0}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-3, -5, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-3, 2, -5, 4, 0, 1}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{}, scDefaultValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
}

void RedBlackTreesTests::testRemoveNodes()
{
    bool nodeDeleted{false};

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(*mpSearchTree);

    nodeDeleted = mpSearchTree->removeNode(-16);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-15:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-15:BK/-12:-15:BK/0:2L:RD/8:14:BK/17:14:BK/-9:-12R:RD/16:17:RD/19:17:RD", 15));

    nodeDeleted = mpSearchTree->removeNode(8);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-15:-5:RD/-2:-5:BK/2:7:BK/17:7:RD/-23:-15:BK/-12:-15:BK/0:2L:RD/14:17:BK/19:17:BK/-9:-12R:RD/16:14R:RD", 14));

    nodeDeleted = mpSearchTree->removeNode(0);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-15:-5:RD/-2:-5:BK/2:7:BK/17:7:RD/-23:-15:BK/-12:-15:BK/14:17:BK/19:17:BK/-9:-12R:RD/16:14R:RD", 13));

    nodeDeleted = mpSearchTree->removeNode(-23);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-12:-5:RD/-2:-5:BK/2:7:BK/17:7:RD/-15:-12:BK/-9:-12:BK/14:17:BK/19:17:BK/16:14R:RD", 12));

    nodeDeleted = mpSearchTree->removeNode(19);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-12:-5:RD/-2:-5:BK/2:7:BK/16:7:BK/-15:-12:BK/-9:-12:BK/14:16:RD/17:16:RD", 11));

    nodeDeleted = mpSearchTree->removeNode(17);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-12:-5:RD/-2:-5:BK/2:7:BK/16:7:BK/-15:-12:BK/-9:-12:BK/14:16L:RD", 10));

    nodeDeleted = mpSearchTree->removeNode(-5);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-12:-1:BK/7:-1:BK/-15:-12:BK/-2:-12:BK/2:7:BK/16:7:BK/-9:-2L:RD/14:16L:RD", 9));

    nodeDeleted = mpSearchTree->removeNode(-9);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-12:-1:BK/7:-1:BK/-15:-12:BK/-2:-12:BK/2:7:BK/16:7:BK/14:16L:RD", 8));

    nodeDeleted = mpSearchTree->removeNode(-15);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-12:-1:BK/7:-1:RD/-2:-12R:RD/2:7:BK/16:7:BK/14:16L:RD", 7));

    nodeDeleted = mpSearchTree->removeNode(14);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-12:-1:BK/7:-1:RD/-2:-12R:RD/2:7:BK/16:7:BK", 6));

    nodeDeleted = mpSearchTree->removeNode(-1);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT:BK/-12:2:BK/7:2:BK/-2:-12R:RD/16:7R:RD", 5));

    nodeDeleted = mpSearchTree->removeNode(-2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT:BK/-12:2:BK/7:2:BK/16:7R:RD", 4));

    nodeDeleted = mpSearchTree->removeNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "7:ROOT:BK/2:7:BK/16:7:BK", 3));

    nodeDeleted = mpSearchTree->removeNode(16); // root and two black children, erase right child
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "7:ROOT:BK/2:7L:RD", 2));

    nodeDeleted = mpSearchTree->removeNode(2); // root and left child, erase left child
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "7:ROOT:BK", 1));

    nodeDeleted = mpSearchTree->removeNode(7);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode(7);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode(25);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // we will stop checking the nodeDeleted variable (obvious enough)
    (void)mpAuxSearchTree->removeNode(-15);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-12:-16:BK/0:2L:RD/8:14:BK/17:14:BK/-9:-12R:RD/16:17:RD/19:17:RD", 15));

    (void)mpAuxSearchTree->removeNode(19);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-12:-16:BK/0:2L:RD/8:14:BK/17:14:BK/-9:-12R:RD/16:17L:RD", 14));

    (void)mpAuxSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2L:RD/8:14:BK/17:14:BK/16:17L:RD", 13));

    (void)mpAuxSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-9L:RD/0:2L:RD/8:14:BK/17:14:BK/16:17L:RD", 12));

    (void)mpAuxSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-9L:RD/0:2L:RD/8:14:BK/16:14:BK", 11));

    (void)mpAuxSearchTree->removeNode(-23);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/0:2L:RD/8:14:BK/16:14:BK", 10));

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/16:7:BK/0:2L:RD/8:16L:RD", 9));

    (void)mpAuxSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/8:7:BK/0:2L:RD", 8));

    (void)mpAuxSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-2:-5R:RD/2:7:BK/8:7:BK/0:2L:RD", 7));

    (void)mpAuxSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-2:-5R:RD/2:7:BK/8:7:BK", 6));

    (void)mpAuxSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-2:-5R:RD/2:8L:RD", 5));

    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/8:-1:BK/2:8L:RD", 4));

    (void)mpAuxSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/8:-1:BK", 3));

    (void)mpAuxSearchTree->removeNode(-1); // root and two black children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "8:ROOT:BK/-5:8L:RD", 2));

    (void)mpAuxSearchTree->removeNode(8); // root and left child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-5:ROOT:BK", 1));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-16:-1:BK/7:-1:BK/-23:-16:BK/-12:-16:RD/2:7:BK/14:7:RD/-15:-12:BK/-2:-12:BK/0:2L:RD/8:14:BK/17:14:BK/-9:-2L:RD/16:17:RD/19:17:RD", 15));

    (void)mpSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-16:-1:BK/7:-1:BK/-23:-16:BK/-12:-16:RD/2:7:BK/17:7:RD/-15:-12:BK/-2:-12:BK/0:2L:RD/14:17:BK/19:17:BK/-9:-2L:RD/16:14R:RD", 14));

    (void)mpSearchTree->removeNode(-1);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "0:ROOT:BK/-16:0:BK/7:0:BK/-23:-16:BK/-12:-16:RD/2:7:BK/17:7:RD/-15:-12:BK/-2:-12:BK/14:17:BK/19:17:BK/-9:-2L:RD/16:14R:RD", 13));

    (void)mpSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "0:ROOT:BK/-16:0:BK/17:0:BK/-23:-16:BK/-12:-16:RD/14:17:RD/19:17:BK/-15:-12:BK/-2:-12:BK/7:14:BK/16:14:BK/-9:-2L:RD", 12));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "0:ROOT:BK/-16:0:BK/17:0:BK/-23:-16:BK/-12:-16:RD/14:17:RD/19:17:BK/-15:-12:BK/-9:-12:BK/7:14:BK/16:14:BK", 11));

    (void)mpSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "0:ROOT:BK/-16:0:BK/17:0:BK/-23:-16:BK/-12:-16:RD/14:17:BK/19:17:BK/-15:-12:BK/-9:-12:BK/16:14R:RD", 10));

    (void)mpSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "14:ROOT:BK/-16:14:BK/17:14:BK/-23:-16:BK/-12:-16:RD/16:17:BK/19:17:BK/-15:-12:BK/-9:-12:BK", 9));

    (void)mpSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "14:ROOT:BK/-16:14:BK/17:14:BK/-23:-16:BK/-12:-16:BK/16:17:BK/19:17:BK/-15:-12L:RD", 8));

    (void)mpSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "14:ROOT:BK/-16:14:RD/17:14:BK/-23:-16:BK/-12:-16:BK/19:17R:RD/-15:-12L:RD", 7));

    (void)mpSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "17:ROOT:BK/-16:17:RD/19:17:BK/-23:-16:BK/-12:-16:BK/-15:-12L:RD", 6));

    (void)mpSearchTree->removeNode(-23);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "17:ROOT:BK/-15:17:BK/19:17:BK/-16:-15:RD/-12:-15:RD", 5));

    (void)mpSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-15:ROOT:BK/-16:-15:BK/19:-15:BK/-12:19L:RD", 4));

    (void)mpSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-12:ROOT:BK/-15:-12:BK/19:-12:BK", 3));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue);

    (void)mpSearchTree->removeNode(-23);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-12:-1:BK/8:-1:BK/-16:-12:BK/-5:-12:RD/2:8:RD/16:8:RD/-15:-16R:RD/-9:-5:BK/-2:-5:BK/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17R:RD", 15));

    (void)mpSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-12:-1:BK/8:-1:BK/-15:-12:BK/-5:-12:RD/2:8:RD/16:8:RD/-9:-5:BK/-2:-5:BK/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17R:RD", 14));

    (void)mpSearchTree->removeNode(-15);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-12:-5:BK/-2:-5:BK/2:8:RD/16:8:RD/-9:-12R:RD/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17R:RD", 13));

    (void)mpSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-9:-5:BK/-2:-5:BK/2:8:RD/16:8:RD/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17R:RD", 12));

    (void)mpSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "8:ROOT:BK/-1:8:BK/16:8:BK/-5:-1:BK/2:-1:RD/14:16:BK/17:16:BK/-2:-5R:RD/0:2:BK/7:2:BK/19:17R:RD", 11));

    (void)mpSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "8:ROOT:BK/-1:8:BK/16:8:BK/-2:-1:BK/2:-1:RD/14:16:BK/17:16:BK/0:2:BK/7:2:BK/19:17R:RD", 10));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "8:ROOT:BK/2:8:BK/16:8:BK/-1:2:BK/7:2:BK/14:16:BK/17:16:BK/0:-1R:RD/19:17R:RD", 9));

    (void)mpSearchTree->removeNode(-1);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "8:ROOT:BK/2:8:BK/16:8:BK/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17R:RD", 8));

    (void)mpSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "8:ROOT:BK/2:8:BK/16:8:RD/7:2R:RD/14:16:BK/17:16:BK/19:17R:RD", 7));

    (void)mpSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "8:ROOT:BK/7:8:BK/16:8:RD/14:16:BK/17:16:BK/19:17R:RD", 6));

    (void)mpSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "16:ROOT:BK/8:16:BK/17:16:BK/14:8R:RD/19:17R:RD", 5));

    (void)mpSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "16:ROOT:BK/14:16:BK/17:16:BK/19:17R:RD", 4));

    (void)mpSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "17:ROOT:BK/16:17:BK/19:17:BK", 3));

    (void)mpSearchTree->removeNode(16); // root and two black children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "17:ROOT:BK/19:17R:RD", 2));

    (void)mpSearchTree->removeNode(17); // root and right child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "19:ROOT:BK", 1));

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue, scCustomNullValue);

    (void)mpAuxSearchTree->removeNode(19);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "0:ROOT:BK/-9:0:BK/14:0:BK/-15:-9:RD/-2:-9:RD/7:14:RD/17:14:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/2:7:BK/8:7:BK/16:17L:RD/-23:-16L:RD", 15));

    (void)mpAuxSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "0:ROOT:BK/-9:0:BK/14:0:BK/-15:-9:RD/-2:-9:RD/7:14:RD/16:14:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/2:7:BK/8:7:BK/-23:-16L:RD", 14));

    (void)mpAuxSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "0:ROOT:BK/-9:0:BK/7:0:BK/-15:-9:RD/-2:-9:RD/2:7:BK/14:7:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/8:14L:RD/-23:-16L:RD", 13));

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "0:ROOT:BK/-9:0:BK/7:0:BK/-15:-9:RD/-2:-9:RD/2:7:BK/8:7:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/-23:-16L:RD", 12));

    (void)mpAuxSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-9:ROOT:BK/-15:-9:BK/0:-9:BK/-16:-15:BK/-12:-15:BK/-2:0:RD/7:0:BK/-23:-16L:RD/-5:-2:BK/-1:-2:BK/2:7L:RD", 11));

    (void)mpAuxSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-9:ROOT:BK/-15:-9:BK/0:-9:BK/-16:-15:BK/-12:-15:BK/-2:0:RD/2:0:BK/-23:-16L:RD/-5:-2:BK/-1:-2:BK", 10));

    (void)mpAuxSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-9:ROOT:BK/-15:-9:BK/-2:-9:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/0:-2:BK/-23:-16L:RD/-1:0L:RD", 9));

    (void)mpAuxSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-9:ROOT:BK/-15:-9:BK/-2:-9:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/-23:-16L:RD", 8));

    (void)mpAuxSearchTree->removeNode(-1);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-9:ROOT:BK/-15:-9:RD/-2:-9:BK/-16:-15:BK/-12:-15:BK/-5:-2L:RD/-23:-16L:RD", 7));

    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-9:ROOT:BK/-15:-9:RD/-5:-9:BK/-16:-15:BK/-12:-15:BK/-23:-16L:RD", 6));

    (void)mpAuxSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-15:ROOT:BK/-16:-15:BK/-9:-15:BK/-23:-16L:RD/-12:-9L:RD", 5));

    (void)mpAuxSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-15:ROOT:BK/-16:-15:BK/-12:-15:BK/-23:-16L:RD", 4));

    (void)mpAuxSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-16:ROOT:BK/-23:-16:BK/-15:-16:BK", 3));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-12:ROOT:BK/-16:-12:BK/-1:-12:RD/-23:-16:BK/-15:-16:BK/-5:-1:BK/16:-1:BK/-9:-5:BK/-2:-5:BK/7:16:RD/17:16:BK/2:7:BK/8:7:BK/19:17R:RD/0:2L:RD", 15));

    (void)mpSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-12:ROOT:BK/-16:-12:BK/7:-12:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/16:7:RD/-5:-1:BK/2:-1:BK/8:16:BK/17:16:BK/-2:-5R:RD/0:2L:RD/19:17R:RD", 14));

    (void)mpSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-12:ROOT:BK/-16:-12:BK/7:-12:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/17:7:RD/-5:-1:BK/2:-1:BK/16:17:BK/19:17:BK/-2:-5R:RD/0:2L:RD", 13));

    (void)mpSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT:BK/-16:-5:BK/7:-5:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/17:7:RD/-2:-1:BK/2:-1:BK/16:17:BK/19:17:BK/0:2L:RD", 12));

    (void)mpSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-5:ROOT:BK/-16:-5:BK/7:-5:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/17:7:BK/-2:-1:BK/2:-1:BK/19:17R:RD/0:2L:RD", 11));

    (void)mpSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT:BK/-16:-2:BK/7:-2:BK/-23:-16:BK/-15:-16:BK/0:7:BK/17:7:BK/-1:0:RD/2:0:RD/19:17R:RD", 10));

    (void)mpSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT:BK/-16:-2:BK/17:-2:BK/-23:-16:BK/-15:-16:BK/0:17:BK/19:17:BK/-1:0:RD/2:0:RD", 9));

    (void)mpSearchTree->removeNode(-15);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT:BK/-16:-2:BK/17:-2:RD/-23:-16L:RD/0:17:BK/19:17:BK/-1:0:RD/2:0:RD", 8));

    (void)mpSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-2:ROOT:BK/-16:-2:BK/0:-2:RD/-23:-16L:RD/-1:0:BK/19:0:BK/2:19L:RD", 7));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-16:-1:BK/2:-1:BK/-23:-16L:RD/0:2:RD/19:2:RD", 6));

    (void)mpSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-16:-1:BK/19:-1:BK/-23:-16L:RD/0:19L:RD", 5));

    (void)mpSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-23:-1:BK/19:-1:BK/0:19L:RD", 4));

    (void)mpSearchTree->removeNode(19);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-1:ROOT:BK/-23:-1:BK/0:-1:BK", 3));

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-3, -5, 2}, scDefaultValue);

    (void)mpAuxSearchTree->removeNode(-5); // root and two red children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-3:ROOT:BK/2:-3R:RD", 2));

    (void)mpAuxSearchTree->removeNode(2); // root and right child, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "-3:ROOT:BK", 1));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-3, -5, 2}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->removeNode(2); // root and two red children, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "-3:ROOT:BK/-5:-3L:RD", 2));

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-3, -5, 2}, scDefaultValue);

    (void)mpAuxSearchTree->removeNode(-3); // root and two red children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "2:ROOT:BK/-5:2L:RD", 2));

    // deleting null node from custom null value tree
    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue, scCustomNullValue);

    nodeDeleted = mpSearchTree->removeNode(-5);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT:BK/-1:2:BK/3:2:BK/-2:-1L:RD/4:3R:RD", 5));

    // deleting same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue);

    nodeDeleted = mpSearchTree->removeNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeDeleted = mpAuxSearchTree->removeNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "2:ROOT:BK/-1:2:BK/4:2:BK/-2:-1L:RD", 4));
}

void RedBlackTreesTests::testUpdateNodeValue()
{
    mpSearchTree = std::make_unique<IntStrRedBlackTree>();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(0, "g7");
    (void)mpSearchTree->addOrUpdateNode(-9, "h8");
    (void)mpSearchTree->addOrUpdateNode(16, "i9");
    (void)mpSearchTree->addOrUpdateNode(14, "j10");
    (void)mpSearchTree->addOrUpdateNode(-23, "k11");
    (void)mpSearchTree->addOrUpdateNode(17, "l12");
    (void)mpSearchTree->addOrUpdateNode(-16, "m13");
    (void)mpSearchTree->addOrUpdateNode(-12, "n14");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1:-1:BK/7:f6:-1:BK/-16:m13:-5:RD/-2:e5:-5:BK/2:d4:7:BK/14:j10:7:RD/-23:k11:-16:BK/-9:h8:-16:BK/0:g7:2L:RD/8:b2:14:BK/16:i9:14:BK/-12:n14:-9L:RD/17:l12:16R:RD", 14));

    QVERIFY("a1" == mpSearchTree->getNodeValue(-5) &&
            "g7" == mpSearchTree->getNodeValue(0) &&
            "i9" == mpSearchTree->getNodeValue(16));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(25));

    // update by adding/modifying node
    bool newNodeAdded{false};

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, "d4_1");
    QVERIFY(!newNodeAdded &&
            "d4_1" == mpSearchTree->getNodeValue(2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, "b2");
    QVERIFY(!newNodeAdded &&
            "b2" == mpSearchTree->getNodeValue(-9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, "j10");
    QVERIFY(!newNodeAdded &&
            "j10" == mpSearchTree->getNodeValue(14));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, "L12");
    QVERIFY(!newNodeAdded &&
            "L12" == mpSearchTree->getNodeValue(17));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            "n14" == mpSearchTree->getNodeValue(-12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(25));

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, "o15");
    QVERIFY(newNodeAdded &&
            "o15" == mpSearchTree->getNodeValue(25));

    // update by deleting node
    bool nodeDeleted{false};

    nodeDeleted = mpSearchTree->removeNode(7);
    QVERIFY(nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    nodeDeleted = mpSearchTree->removeNode(-28);
    QVERIFY(!nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-28));

    // delete and re-add node with different value
    nodeDeleted = mpSearchTree->removeNode(16);
    newNodeAdded = mpSearchTree->addOrUpdateNode(16, "i9_new");

    QVERIFY(nodeDeleted &&
            newNodeAdded &&
            "i9_new" == mpSearchTree->getNodeValue(16));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1:-1:BK/8:b2:-1:BK/-16:m13:-5:RD/-2:e5:-5:BK/2:d4_1:8:BK/17:L12:8:RD/-23:k11:-16:BK/-9:b2:-16:BK/0:g7:2L:RD/14:j10:17:BK/25:o15:17:BK/-12:n14:-9L:RD/16:i9_new:14R:RD", 14));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    // test with same value for all nodes
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:DF:ROOT:BK/-5:DF:-1:BK/7:DF:-1:BK/-16:DF:-5:RD/-2:DF:-5:BK/2:DF:7:BK/14:DF:7:RD/-23:DF:-16:BK/-9:DF:-16:BK/0:DF:2L:RD/8:DF:14:BK/16:DF:14:BK/-12:DF:-9L:RD/17:DF:16R:RD", 14));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue(-5) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(0) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(16));

    // test updating custom null value tree node values

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-8, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(0, "g7");
    (void)mpSearchTree->addOrUpdateNode(-9, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1::ROOT:BK/-5:a1:-1:RD/7:f6:-1:RD/-8::-5:BK/-2:e5:-5:BK/2:d4:7:BK/8:b2:7:BK/-9:h8:-8L:RD/0:g7:2L:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, "d4_1");
    QVERIFY(!newNodeAdded &&
            "d4_1" == mpSearchTree->getNodeValue(2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, "c3");
    QVERIFY(!newNodeAdded &&
            "c3" == mpSearchTree->getNodeValue(-1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            "f6" == mpSearchTree->getNodeValue(7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            scCustomNullValue == mpSearchTree->getNodeValue(25));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-7, "i9");
    QVERIFY(newNodeAdded &&
            "i9" == mpSearchTree->getNodeValue(-7));

    nodeDeleted = mpSearchTree->removeNode(-25);
    QVERIFY(!nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(-25));

    nodeDeleted = mpSearchTree->removeNode(-8);
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(-8));

    nodeDeleted = mpSearchTree->removeNode(2);
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(2));

    nodeDeleted = mpSearchTree->removeNode(-1);
    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultNullValue);

    QVERIFY(nodeDeleted &&
            newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-1));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "0:g7:ROOT:BK/-5:a1:0:RD/7:f6:0:BK/-7:i9:-5:BK/-2::-5:BK/8:b2:7R:RD/-9:h8:-7L:RD/-1::-2R:RD", 8));

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(*mpSearchTree);

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(17, "df");
    QVERIFY(newNodeAdded &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue);

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue(4));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "14:DF:ROOT:BK/-9:DF:14:RD/16:DF:14:BK/-16:DF:-9:BK/7::-9:BK/17:DF:16R:RD/-23:DF:-16:RD/-12:DF:-16:RD", 8));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void RedBlackTreesTests::testMoveSemantics()
{
    mpSearchTree = std::make_unique<IntStrRedBlackTree>();

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1:-1:BK/8:b2:-1:BK/-2:e5:-5R:RD/2:d4:8L:RD", 5));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->addOrUpdateNode(17, "f6");
    mpSearchTree->addOrUpdateNode(0, "g7");
    mpSearchTree->addOrUpdateNode(-2, "e5_1");
    mpSearchTree->addOrUpdateNode(3, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "0:g7:ROOT:BK/-2:e5_1:0:BK/17:f6:0:BK/3:h8:17L:RD", 4));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "0:g7:ROOT:BK/-2:e5_1:0:BK/17:f6:0:BK/3:h8:17L:RD", 4));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "0:g7:ROOT:BK/-2:e5_1:0:BK/17:f6:0:BK/3:h8:17L:RD", 4));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue, scCustomNullValue);

    (void)mpSearchTree->addOrUpdateNode(5, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(4, "newval");

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::move(*mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "4:newval:ROOT:BK/-1:DF:4:BK/5::4:BK/-2:DF:-1:RD/0:DF:-1:RD", 5));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue);

    (void)mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "4:NULL:ROOT:BK/-1:DF:4:BK/5:DF:4:BK/-2:DF:-1:RD/0:DF:-1:RD", 5));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void RedBlackTreesTests::testMergeTrees()
{
    mpSearchTree = std::make_unique<IntStrRedBlackTree>();

    (void)mpSearchTree->addOrUpdateNode(-5, "a1_1");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-23, "k11");
    (void)mpSearchTree->addOrUpdateNode(17, "l12");
    (void)mpSearchTree->addOrUpdateNode(-12, "n14");
    (void)mpSearchTree->addOrUpdateNode(16, "i9_1");
    (void)mpSearchTree->addOrUpdateNode(0, "g7_1");

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>();

    (void)mpAuxSearchTree->addOrUpdateNode(8, "b2");
    (void)mpAuxSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpAuxSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpAuxSearchTree->addOrUpdateNode(0, "g7_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-5, "a1_2");
    (void)mpAuxSearchTree->addOrUpdateNode(16, "i9_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-9, "h8");
    (void)mpAuxSearchTree->addOrUpdateNode(14, "j10");
    (void)mpAuxSearchTree->addOrUpdateNode(-16, "m13");

    const IntStrRedBlackTree searchTreeCopy{*mpSearchTree};
    const IntStrRedBlackTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    // first (main) merge
    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:BK/16:i9_2:2:BK/-23:k11:-12:BK/-1:c3:-12:RD/8:b2:16:BK/17:l12:16:BK/-16:m13:-23R:RD/-5:a1_2:-1:BK/0:g7_2:-1:BK/7:f6:8:RD/14:j10:8:RD/-9:h8:-5:RD/-2:e5:-5:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    const IntStrRedBlackTree mainTreeAfterFirstMerge{*mpSearchTree};

    // merge empty tree into unempty tree
    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(*mpSearchTree == mainTreeAfterFirstMerge);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    // merge unempty tree with itself
    merged = mpSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(*mpSearchTree == mainTreeAfterFirstMerge);

    // merge unempty tree into empty tree
    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(*mpAuxSearchTree == mainTreeAfterFirstMerge);

    // merge empty tree with itself
    merged = mpSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));

    // merge two empty trees
    mpAuxSearchTree->clear();
    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    // do inverse merge operation comparing to first merge
    *mpSearchTree = searchTreeCopy;
    *mpAuxSearchTree = searchTreeAuxCopy;

    QVERIFY(*mpSearchTree == searchTreeCopy &&      // just a(n additional) check that the copy assignment operator and == operator work correctly
            *mpAuxSearchTree == searchTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_1:-1:BK/8:b2:-1:BK/-12:n14:-5:RD/-2:e5:-5:BK/2:d4:8:BK/16:i9_1:8:BK/-16:m13:-12:BK/-9:h8:-12:BK/0:g7_1:2:RD/7:f6:2:RD/14:j10:16:RD/17:l12:16:RD/-23:k11:-16L:RD", 14));
    QVERIFY(*mpAuxSearchTree != mainTreeAfterFirstMerge); // test the != operator too
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // merge trees with (same) custom null value

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(-8, "e5");
    (void)mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(0, "g7");
    (void)mpSearchTree->addOrUpdateNode(-9, "h8");
    (void)mpSearchTree->addOrUpdateNode(-7, "i9");
    (void)mpSearchTree->removeNode(-8);
    (void)mpSearchTree->removeNode(2);

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode(-9, scDefaultNullValue);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1:-1:BK/7:DF:-1:BK/-9::-5:RD/-2::-5:BK/0:g7:7:BK/14:DF:7:RD/-16:DF:-9:BK/-7:i9:-9:BK/8:b2:14:BK/16:DF:14:BK/-23:DF:-16:RD/-12:DF:-16:RD/17:DF:16R:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. default)

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{5, 10, -2, 9, 4, 2, 7, -8}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12, 1}, scDefaultValue, scDefaultNullValue);
    mpSearchTree->addOrUpdateNode(9, "abc");
    mpAuxSearchTree->addOrUpdateNode(7, "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "5:DF:ROOT:BK/2:DF:5:RD/9:abc:5:BK/-2:DF:2:BK/4:DF:2:BK/7:DF:9:RD/10:DF:9:RD/-8:DF:-2L:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "14:DF:ROOT:BK/-9:DF:14:RD/16:DF:14:BK/-16:DF:-9:BK/7:xyz:-9:BK/17:DF:16R:RD/-23:DF:-16:RD/-12:DF:-16:RD/1:DF:7L:RD", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "14:DF:ROOT:BK/-9:DF:14:RD/16:DF:14:BK/-16:DF:-9:BK/7:xyz:-9:BK/17:DF:16R:RD/-23:DF:-16:RD/-12:DF:-16:RD/1:DF:7L:RD", 9));

    mpAuxSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // (attempt to) merge trees with different null values (custom vs. custom)

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{5, 10, -2, 9, 4, 2, 7, -8}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12, 1}, scDefaultValue, scCustomNullValue + "1");
    mpSearchTree->addOrUpdateNode(9, "abc");
    mpAuxSearchTree->addOrUpdateNode(7, "xyz");

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "5:DF:ROOT:BK/2:DF:5:RD/9:abc:5:BK/-2:DF:2:BK/4:DF:2:BK/7:DF:9:RD/10:DF:9:RD/-8:DF:-2L:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "14:DF:ROOT:BK/-9:DF:14:RD/16:DF:14:BK/-16:DF:-9:BK/7:xyz:-9:BK/17:DF:16R:RD/-23:DF:-16:RD/-12:DF:-16:RD/1:DF:7L:RD", 9));

    mpSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "14:DF:ROOT:BK/-9:DF:14:RD/16:DF:14:BK/-16:DF:-9:BK/7:xyz:-9:BK/17:DF:16R:RD/-23:DF:-16:RD/-12:DF:-16:RD/1:DF:7L:RD", 9));

    mpAuxSearchTree->clear();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue + "1" == mpAuxSearchTree->getNullValue());
}

void RedBlackTreesTests::testInOrderForwardIterators()
{
    mpSearchTree = std::make_unique<IntStrRedBlackTree>();

    (void)mpSearchTree->addOrUpdateNode(-5, "b");
    (void)mpSearchTree->addOrUpdateNode(8, "z");
    (void)mpSearchTree->addOrUpdateNode(-1, "_ca");
    (void)mpSearchTree->addOrUpdateNode(2, "q1");
    (void)mpSearchTree->addOrUpdateNode(-2, "55");
    (void)mpSearchTree->addOrUpdateNode(7, "a");
    (void)mpSearchTree->addOrUpdateNode(0, "fq");
    (void)mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    (void)mpSearchTree->addOrUpdateNode(16, "cCc");
    (void)mpSearchTree->addOrUpdateNode(14, "abab");
    (void)mpSearchTree->addOrUpdateNode(-23, "-c");
    (void)mpSearchTree->addOrUpdateNode(17, "b");
    (void)mpSearchTree->addOrUpdateNode(-16, "qa");
    (void)mpSearchTree->addOrUpdateNode(-12, "dev");
    (void)mpSearchTree->addOrUpdateNode(19, "_ca");
    (void)mpSearchTree->addOrUpdateNode(-15, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(),
                                     "-1:_ca:ROOT:BK/-5:b:-1:BK/7:a:-1:BK/-16:qa:-5:RD/-2:55:-5:BK/2:q1:7:BK/14:abab:7:RD/"
                                     "-23:-c:-16:BK/-12:dev:-16:BK/0:fq:2L:RD/8:z:14:BK/17:b:14:BK/-15:DF:-12:RD/-9:DF:-12:RD/16:cCc:17:RD/19:_ca:17:RD",
                                     16));

    IntStrRBTIterator it{mpSearchTree->begin()};
    QVERIFY(it.getKey() == -23 && it.getValue() == "-c");

    it = mpSearchTree->root();
    QVERIFY(it.getKey() == -1 && it.getValue() == "_ca");

    it = mpSearchTree->find(-2);
    QVERIFY(it.getKey() == -2 && it.getValue() == "55");

    QVERIFY(mpSearchTree->find(-23) == mpSearchTree->begin());
    QVERIFY(mpSearchTree->find(-1) == mpSearchTree->root());
    QVERIFY(mpSearchTree->find(12) == mpSearchTree->end());

    std::vector<std::pair<std::optional<int>, std::string>> traversedElements;
    const std::vector<std::pair<std::optional<int>, std::string>> c_TraversedElementsRef{{-23, "-c"}, {-16, "qa"}, {-15, "DF"}, {-12, "dev"}, {-9, "DF"}, {-5, "b"}, {-2, "55"}, {-1, "_ca"},
                                                                          {0, "fq"}, {2, "q1"}, {7, "a"}, {8, "z"}, {14, "abab"}, {16, "cCc"}, {17, "b"}, {19, "_ca"}};

    for (IntStrRBTIterator it{mpSearchTree->begin()}; it != mpSearchTree->end(); it.next())
    {
        traversedElements.push_back({it.getKey(), it.getValue()});
    }

    QVERIFY(std::equal(traversedElements.cbegin(), traversedElements.cend(), c_TraversedElementsRef.cbegin()));

    (void)mpSearchTree->addOrUpdateNode(14, "BaBa");
    it = mpSearchTree->find(14);

    QVERIFY(it.getValue() == "BaBa");

    it.next();
    QVERIFY(it.getKey() == 16);

    it.setValue("bCCC");
    QVERIFY(mpSearchTree->getNodeValue(16) == "bCCC");

    it = mpSearchTree->find(19);
    it.next();

    QVERIFY(it == mpSearchTree->end());
    QVERIFY(!it.getKey().has_value() && it.getValue().empty());

    QVERIFY(mpSearchTree->find(2) != mpSearchTree->end());
    (void)mpSearchTree->removeNode(2);
    QVERIFY(mpSearchTree->find(2) == mpSearchTree->end());

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>("NullVal");
    QVERIFY(mpAuxSearchTree->begin() == mpAuxSearchTree->end() && mpAuxSearchTree->root() == mpAuxSearchTree->end() && mpAuxSearchTree->find(14) == mpAuxSearchTree->end());

    IntStrRBTIterator itAux;
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue().empty() && itAux == mpSearchTree->end() && itAux != mpAuxSearchTree->end());

    itAux = mpAuxSearchTree->end();
    QVERIFY(!itAux.getKey().has_value() && itAux.getValue() == "NullVal");
}

void RedBlackTreesTests::testPrintTree()
{
    qInfo("Creating new tree");
    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue);

    QVERIFY(16 == mpSearchTree->getSize());

    qInfo("The tree nodes are: ");
    mpSearchTree->printTree();

    qInfo("Clearing tree content");
    mpSearchTree->clear();

    QVERIFY(0 == mpSearchTree->getSize());

    mpSearchTree->printTree();
    qInfo("The red-black tree has no nodes");
}

QTEST_APPLESS_MAIN(RedBlackTreesTests)

#include "tst_redblacktreestests.moc"
