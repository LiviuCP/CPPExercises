#include <QtTest>

#include "redblacktree.h"

class RedBlackTreesTests : public QObject
{
    Q_OBJECT

public:
    RedBlackTreesTests();
    ~RedBlackTreesTests();

private slots:
    void testAddNodes();
    void testRemoveNodes();
    void testUpdateNodeValue();
    void testMoveSemantics();
    void testPrintTree(); // only required for improving code coverage

private:
    void _reset();
    bool _areExpectedTreeValuesMet(const RedBlackTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered = false) const;

    RedBlackTree* mpSearchTree;
    RedBlackTree* mpAuxSearchTree;

    static const std::string scDefaultValue;
    static const std::string scDefaultNullValue;
    static const std::string scCustomNullValue;
    static const std::string scEmptyTreeString;
};

const std::string RedBlackTreesTests::scDefaultValue{"DF"};
const std::string RedBlackTreesTests::scDefaultNullValue{""};
const std::string RedBlackTreesTests::scCustomNullValue{"/0"};
const std::string RedBlackTreesTests::scEmptyTreeString{""};

RedBlackTreesTests::RedBlackTreesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

RedBlackTreesTests::~RedBlackTreesTests()
{
    _reset();
}

void RedBlackTreesTests::testAddNodes()
{
    _reset();

    bool newNodeAdded{false};

    mpSearchTree = new RedBlackTree;

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT:BK", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT:BK/8:-5:RD", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:RD/8:-1:RD", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/2:8:RD", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-2:-5:RD/2:8:RD", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-2:-5:RD/2:7:RD/8:7:RD", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2:RD", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2:RD", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2:RD/16:8:RD", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/14:7:BK/0:2:RD/8:14:RD/16:14:RD", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:RD/7:-1:RD/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:BK/-23:-9:RD/0:2:RD/8:14:RD/16:14:RD", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-9:RD/0:2:RD/8:14:BK/16:14:BK/17:16:RD", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:RD/-9:-16:RD/0:2:RD/8:14:BK/16:14:BK/17:16:RD", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2:RD/8:14:BK/16:14:BK/-12:-9:RD/17:16:RD", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode(19, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree,"-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2:RD/8:14:BK/17:14:BK/-12:-9:RD/16:17:RD/19:17:RD", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-15, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-12:-16:BK/0:2:RD/8:14:BK/17:14:BK/-15:-12:RD/-9:-12:RD/16:17:RD/19:17:RD", 16));

    // adding nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue};

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

    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/14:-1:BK/-16:-5:RD/-2:-5:BK/7:14:RD/17:14:RD/-23:-16:BK/-12:-16:BK/2:7:BK/8:7:BK/16:17:BK/19:17:BK/-15:-12:RD/-9:-12:RD/0:2:RD/25:19:RD", 17));

    // some additional (corner) cases
    delete mpSearchTree;
    mpSearchTree = new RedBlackTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT:BK/-16:-12:BK/-1:-12:RD/-23:-16:BK/-15:-16:BK/-5:-1:BK/8:-1:BK/-9:-5:BK/-2:-5:BK/2:8:RD/16:8:RD/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17:RD", 16));

    delete mpSearchTree;
    mpSearchTree = new RedBlackTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT:BK/0:14:RD/17:14:BK/-9:0:BK/7:0:BK/16:17:BK/19:17:BK/-15:-9:RD/-2:-9:RD/2:7:BK/8:7:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/-23:-16:RD", 16));

    delete mpSearchTree;
    mpSearchTree = new RedBlackTree{std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT:BK/-16:-12:BK/14:-12:RD/-23:-16:BK/-15:-16:BK/-1:14:BK/17:14:BK/-5:-1:RD/7:-1:RD/16:17:BK/19:17:BK/-9:-5:BK/-2:-5:BK/2:7:BK/8:7:BK/0:2:RD", 16));

    mpSearchTree->clear();

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators
    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-5, 2, -3, 2, 4, 0, 1}, scDefaultValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-5, -3, 2, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree); // in this particular case due to RB tree construction rules the trees become equal when third element is being added (unlike the basic BST)

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-5, 2, -3, 4, 1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-3, -5, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-3, 2, -5, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{}, scDefaultValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    delete mpSearchTree;
    mpSearchTree = new RedBlackTree{scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
}

void RedBlackTreesTests::testRemoveNodes()
{
    _reset();

    bool nodeDeleted{false};

    mpSearchTree = new RedBlackTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue};
    mpAuxSearchTree = new RedBlackTree{*mpSearchTree};

    nodeDeleted = mpSearchTree->deleteNode(-16);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-15:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-15:BK/-12:-15:BK/0:2:RD/8:14:BK/17:14:BK/-9:-12:RD/16:17:RD/19:17:RD", 15));

    nodeDeleted = mpSearchTree->deleteNode(8);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-15:-5:RD/-2:-5:BK/2:7:BK/17:7:RD/-23:-15:BK/-12:-15:BK/0:2:RD/14:17:BK/19:17:BK/-9:-12:RD/16:14:RD", 14));

    nodeDeleted = mpSearchTree->deleteNode(0);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-15:-5:RD/-2:-5:BK/2:7:BK/17:7:RD/-23:-15:BK/-12:-15:BK/14:17:BK/19:17:BK/-9:-12:RD/16:14:RD", 13));

    nodeDeleted = mpSearchTree->deleteNode(-23);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-12:-5:RD/-2:-5:BK/2:7:BK/17:7:RD/-15:-12:BK/-9:-12:BK/14:17:BK/19:17:BK/16:14:RD", 12));

    nodeDeleted = mpSearchTree->deleteNode(19);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-12:-5:RD/-2:-5:BK/2:7:BK/16:7:BK/-15:-12:BK/-9:-12:BK/14:16:RD/17:16:RD", 11));

    nodeDeleted = mpSearchTree->deleteNode(17);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-12:-5:RD/-2:-5:BK/2:7:BK/16:7:BK/-15:-12:BK/-9:-12:BK/14:16:RD", 10));

    nodeDeleted = mpSearchTree->deleteNode(-5);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-12:-1:BK/7:-1:BK/-15:-12:BK/-2:-12:BK/2:7:BK/16:7:BK/-9:-2:RD/14:16:RD", 9));

    nodeDeleted = mpSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-12:-1:BK/7:-1:BK/-15:-12:BK/-2:-12:BK/2:7:BK/16:7:BK/14:16:RD", 8));

    nodeDeleted = mpSearchTree->deleteNode(-15);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-12:-1:BK/7:-1:RD/-2:-12:RD/2:7:BK/16:7:BK/14:16:RD", 7));

    nodeDeleted = mpSearchTree->deleteNode(14);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-12:-1:BK/7:-1:RD/-2:-12:RD/2:7:BK/16:7:BK", 6));

    nodeDeleted = mpSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT:BK/-12:2:BK/7:2:BK/-2:-12:RD/16:7:RD", 5));

    nodeDeleted = mpSearchTree->deleteNode(-2);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT:BK/-12:2:BK/7:2:BK/16:7:RD", 4));

    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "7:ROOT:BK/2:7:BK/16:7:BK", 3));

    nodeDeleted = mpSearchTree->deleteNode(16);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "7:ROOT:BK/2:7:RD", 2));

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "7:ROOT:BK", 1));

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(!nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->deleteNode(25);
    QVERIFY(!nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // we will stop checking the nodeDeleted variable (obvious enough)
    (void)mpAuxSearchTree->deleteNode(-15);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-12:-16:BK/0:2:RD/8:14:BK/17:14:BK/-9:-12:RD/16:17:RD/19:17:RD", 15));

    (void)mpAuxSearchTree->deleteNode(19);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-12:-16:BK/0:2:RD/8:14:BK/17:14:BK/-9:-12:RD/16:17:RD", 14));

    (void)mpAuxSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2:RD/8:14:BK/17:14:BK/16:17:RD", 13));

    (void)mpAuxSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-9:RD/0:2:RD/8:14:BK/17:14:BK/16:17:RD", 12));

    (void)mpAuxSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-9:RD/0:2:RD/8:14:BK/16:14:BK", 11));

    (void)mpAuxSearchTree->deleteNode(-23);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/0:2:RD/8:14:BK/16:14:BK", 10));

    (void)mpAuxSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/16:7:BK/0:2:RD/8:16:RD", 9));

    (void)mpAuxSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/8:7:BK/0:2:RD", 8));

    (void)mpAuxSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2:RD", 7));

    (void)mpAuxSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-2:-5:RD/2:7:BK/8:7:BK", 6));

    (void)mpAuxSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-2:-5:RD/2:8:RD", 5));

    (void)mpAuxSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/2:8:RD", 4));

    (void)mpAuxSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK", 3));

    (void)mpAuxSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "8:ROOT:BK/-5:8:RD", 2));

    (void)mpAuxSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT:BK", 1));

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-16:-1:BK/7:-1:BK/-23:-16:BK/-12:-16:RD/2:7:BK/14:7:RD/-15:-12:BK/-2:-12:BK/0:2:RD/8:14:BK/17:14:BK/-9:-2:RD/16:17:RD/19:17:RD", 15));

    (void)mpSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-16:-1:BK/7:-1:BK/-23:-16:BK/-12:-16:RD/2:7:BK/17:7:RD/-15:-12:BK/-2:-12:BK/0:2:RD/14:17:BK/19:17:BK/-9:-2:RD/16:14:RD", 14));

    (void)mpSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT:BK/-16:0:BK/7:0:BK/-23:-16:BK/-12:-16:RD/2:7:BK/17:7:RD/-15:-12:BK/-2:-12:BK/14:17:BK/19:17:BK/-9:-2:RD/16:14:RD", 13));

    (void)mpSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT:BK/-16:0:BK/17:0:BK/-23:-16:BK/-12:-16:RD/14:17:RD/19:17:BK/-15:-12:BK/-2:-12:BK/7:14:BK/16:14:BK/-9:-2:RD", 12));

    (void)mpSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT:BK/-16:0:BK/17:0:BK/-23:-16:BK/-12:-16:RD/14:17:RD/19:17:BK/-15:-12:BK/-9:-12:BK/7:14:BK/16:14:BK", 11));

    (void)mpSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT:BK/-16:0:BK/17:0:BK/-23:-16:BK/-12:-16:RD/14:17:BK/19:17:BK/-15:-12:BK/-9:-12:BK/16:14:RD", 10));

    (void)mpSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT:BK/-16:14:BK/17:14:BK/-23:-16:BK/-12:-16:RD/16:17:BK/19:17:BK/-15:-12:BK/-9:-12:BK", 9));

    (void)mpSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT:BK/-16:14:BK/17:14:BK/-23:-16:BK/-12:-16:BK/16:17:BK/19:17:BK/-15:-12:RD", 8));

    (void)mpSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT:BK/-16:14:RD/17:14:BK/-23:-16:BK/-12:-16:BK/19:17:RD/-15:-12:RD", 7));

    (void)mpSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:ROOT:BK/-16:17:RD/19:17:BK/-23:-16:BK/-12:-16:BK/-15:-12:RD", 6));

    (void)mpSearchTree->deleteNode(-23);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:ROOT:BK/-15:17:BK/19:17:BK/-16:-15:RD/-12:-15:RD", 5));

    (void)mpSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-15:ROOT:BK/-16:-15:BK/19:-15:BK/-12:19:RD", 4));

    (void)mpSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT:BK/-15:-12:BK/19:-12:BK", 3));

    (void)mpSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "19:ROOT:BK/-15:19:RD", 2));

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    (void)mpSearchTree->deleteNode(-23);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-12:-1:BK/8:-1:BK/-16:-12:BK/-5:-12:RD/2:8:RD/16:8:RD/-15:-16:RD/-9:-5:BK/-2:-5:BK/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17:RD", 15));

    (void)mpSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-12:-1:BK/8:-1:BK/-15:-12:BK/-5:-12:RD/2:8:RD/16:8:RD/-9:-5:BK/-2:-5:BK/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17:RD", 14));

    (void)mpSearchTree->deleteNode(-15);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-12:-5:BK/-2:-5:BK/2:8:RD/16:8:RD/-9:-12:RD/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17:RD", 13));

    (void)mpSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-9:-5:BK/-2:-5:BK/2:8:RD/16:8:RD/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17:RD", 12));

    (void)mpSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT:BK/-1:8:BK/16:8:BK/-5:-1:BK/2:-1:RD/14:16:BK/17:16:BK/-2:-5:RD/0:2:BK/7:2:BK/19:17:RD", 11));

    (void)mpSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT:BK/-1:8:BK/16:8:BK/-2:-1:BK/2:-1:RD/14:16:BK/17:16:BK/0:2:BK/7:2:BK/19:17:RD", 10));

    (void)mpSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT:BK/2:8:BK/16:8:BK/-1:2:BK/7:2:BK/14:16:BK/17:16:BK/0:-1:RD/19:17:RD", 9));

    (void)mpSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT:BK/2:8:BK/16:8:BK/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17:RD", 8));

    (void)mpSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT:BK/2:8:BK/16:8:RD/7:2:RD/14:16:BK/17:16:BK/19:17:RD", 7));

    (void)mpSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT:BK/7:8:BK/16:8:RD/14:16:BK/17:16:BK/19:17:RD", 6));

    (void)mpSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "16:ROOT:BK/8:16:BK/17:16:BK/14:8:RD/19:17:RD", 5));

    (void)mpSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "16:ROOT:BK/14:16:BK/17:16:BK/19:17:RD", 4));

    (void)mpSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:ROOT:BK/16:17:BK/19:17:BK", 3));

    (void)mpSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:ROOT:BK/19:17:RD", 2));

    (void)mpSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "19:ROOT:BK", 1));

    mpAuxSearchTree = new RedBlackTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue, scCustomNullValue};

    (void)mpAuxSearchTree->deleteNode(19);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT:BK/-9:0:BK/14:0:BK/-15:-9:RD/-2:-9:RD/7:14:RD/17:14:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/2:7:BK/8:7:BK/16:17:RD/-23:-16:RD", 15));

    (void)mpAuxSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT:BK/-9:0:BK/14:0:BK/-15:-9:RD/-2:-9:RD/7:14:RD/16:14:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/2:7:BK/8:7:BK/-23:-16:RD", 14));

    (void)mpAuxSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT:BK/-9:0:BK/7:0:BK/-15:-9:RD/-2:-9:RD/2:7:BK/14:7:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/8:14:RD/-23:-16:RD", 13));

    (void)mpAuxSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT:BK/-9:0:BK/7:0:BK/-15:-9:RD/-2:-9:RD/2:7:BK/8:7:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/-23:-16:RD", 12));

    (void)mpAuxSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT:BK/-15:-9:BK/0:-9:BK/-16:-15:BK/-12:-15:BK/-2:0:RD/7:0:BK/-23:-16:RD/-5:-2:BK/-1:-2:BK/2:7:RD", 11));

    (void)mpAuxSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT:BK/-15:-9:BK/0:-9:BK/-16:-15:BK/-12:-15:BK/-2:0:RD/2:0:BK/-23:-16:RD/-5:-2:BK/-1:-2:BK", 10));

    (void)mpAuxSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT:BK/-15:-9:BK/-2:-9:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/0:-2:BK/-23:-16:RD/-1:0:RD", 9));

    (void)mpAuxSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT:BK/-15:-9:BK/-2:-9:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/-23:-16:RD", 8));

    (void)mpAuxSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT:BK/-15:-9:RD/-2:-9:BK/-16:-15:BK/-12:-15:BK/-5:-2:RD/-23:-16:RD", 7));

    (void)mpAuxSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT:BK/-15:-9:RD/-5:-9:BK/-16:-15:BK/-12:-15:BK/-23:-16:RD", 6));

    (void)mpAuxSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT:BK/-16:-15:BK/-9:-15:BK/-23:-16:RD/-12:-9:RD", 5));

    (void)mpAuxSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT:BK/-16:-15:BK/-12:-15:BK/-23:-16:RD", 4));

    (void)mpAuxSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-16:ROOT:BK/-23:-16:BK/-15:-16:BK", 3));

    (void)mpAuxSearchTree->deleteNode(-15);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-16:ROOT:BK/-23:-16:RD", 2));

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT:BK/-16:-12:BK/-1:-12:RD/-23:-16:BK/-15:-16:BK/-5:-1:BK/16:-1:BK/-9:-5:BK/-2:-5:BK/7:16:RD/17:16:BK/2:7:BK/8:7:BK/19:17:RD/0:2:RD", 15));

    (void)mpSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT:BK/-16:-12:BK/7:-12:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/16:7:RD/-5:-1:BK/2:-1:BK/8:16:BK/17:16:BK/-2:-5:RD/0:2:RD/19:17:RD", 14));

    (void)mpSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT:BK/-16:-12:BK/7:-12:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/17:7:RD/-5:-1:BK/2:-1:BK/16:17:BK/19:17:BK/-2:-5:RD/0:2:RD", 13));

    (void)mpSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT:BK/-16:-5:BK/7:-5:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/17:7:RD/-2:-1:BK/2:-1:BK/16:17:BK/19:17:BK/0:2:RD", 12));

    (void)mpSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT:BK/-16:-5:BK/7:-5:BK/-23:-16:BK/-15:-16:BK/-1:7:RD/17:7:BK/-2:-1:BK/2:-1:BK/19:17:RD/0:2:RD", 11));

    (void)mpSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT:BK/-16:-2:BK/7:-2:BK/-23:-16:BK/-15:-16:BK/0:7:BK/17:7:BK/-1:0:RD/2:0:RD/19:17:RD", 10));

    (void)mpSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT:BK/-16:-2:BK/17:-2:BK/-23:-16:BK/-15:-16:BK/0:17:BK/19:17:BK/-1:0:RD/2:0:RD", 9));

    (void)mpSearchTree->deleteNode(-15);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT:BK/-16:-2:BK/17:-2:RD/-23:-16:RD/0:17:BK/19:17:BK/-1:0:RD/2:0:RD", 8));

    (void)mpSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT:BK/-16:-2:BK/0:-2:RD/-23:-16:RD/-1:0:BK/19:0:BK/2:19:RD", 7));

    (void)mpSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-16:-1:BK/2:-1:BK/-23:-16:RD/0:2:RD/19:2:RD", 6));

    (void)mpSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-16:-1:BK/19:-1:BK/-23:-16:RD/0:19:RD", 5));

    (void)mpSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-23:-1:BK/19:-1:BK/0:19:RD", 4));

    (void)mpSearchTree->deleteNode(19);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-23:-1:BK/0:-1:BK", 3));

    (void)mpSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT:BK/-23:0:RD", 2));

    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-3, -5, 2}, scDefaultValue};

    (void)mpAuxSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-3:ROOT:BK/2:-3:RD", 2));

    (void)mpAuxSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-3:ROOT:BK", 1));

    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-3, -5, 2}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-3:ROOT:BK/-5:-3:RD", 2));

    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-3, -5, 2}, scDefaultValue};

    (void)mpAuxSearchTree->deleteNode(-3);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT:BK/-5:2:RD", 2));
}

void RedBlackTreesTests::testUpdateNodeValue()
{
    _reset();

    mpSearchTree = new RedBlackTree;

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

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1:-1:BK/7:f6:-1:BK/-16:m13:-5:RD/-2:e5:-5:BK/2:d4:7:BK/14:j10:7:RD/-23:k11:-16:BK/-9:h8:-16:BK/0:g7:2:RD/8:b2:14:BK/16:i9:14:BK/-12:n14:-9:RD/17:l12:16:RD", 14, true));

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

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    nodeDeleted = mpSearchTree->deleteNode(-28);
    QVERIFY(!nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-28));

    // delete and re-add node with different value
    nodeDeleted = mpSearchTree->deleteNode(16);
    newNodeAdded = mpSearchTree->addOrUpdateNode(16, "i9_new");

    QVERIFY(nodeDeleted &&
            newNodeAdded &&
            "i9_new" == mpSearchTree->getNodeValue(16));

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1:-1:BK/8:b2:-1:BK/-16:m13:-5:RD/-2:e5:-5:BK/2:d4_1:8:BK/17:L12:8:RD/-23:k11:-16:BK/-9:b2:-16:BK/0:g7:2:RD/14:j10:17:BK/25:o15:17:BK/-12:n14:-9:RD/16:i9_new:14:RD", 14, true));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    // test with same value for all nodes
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:DF:ROOT:BK/-5:DF:-1:BK/7:DF:-1:BK/-16:DF:-5:RD/-2:DF:-5:BK/2:DF:7:BK/14:DF:7:RD/-23:DF:-16:BK/-9:DF:-16:BK/0:DF:2:RD/8:DF:14:BK/16:DF:14:BK/-12:DF:-9:RD/17:DF:16:RD", 14, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue(-5) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(0) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(16));

    // test updating custom null value tree node values
    _reset();

    mpSearchTree = new RedBlackTree{scCustomNullValue};

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-8, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(0, "g7");
    (void)mpSearchTree->addOrUpdateNode(-9, "h8");

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1::ROOT:BK/-5:a1:-1:RD/7:f6:-1:RD/-8::-5:BK/-2:e5:-5:BK/2:d4:7:BK/8:b2:7:BK/-9:h8:-8:RD/0:g7:2:RD", 9, true));
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

    nodeDeleted = mpSearchTree->deleteNode(-25);
    QVERIFY(!nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(-25));

    nodeDeleted = mpSearchTree->deleteNode(-8);
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(-8));

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(2));

    nodeDeleted = mpSearchTree->deleteNode(-1);
    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultNullValue);

    QVERIFY(nodeDeleted &&
            newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-1));

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:g7:ROOT:BK/-5:a1:0:RD/7:f6:0:BK/-7:i9:-5:BK/-2::-5:BK/8:b2:7:RD/-9:h8:-7:RD/-1::-2:RD", 8, true));

    mpAuxSearchTree = new RedBlackTree{*mpSearchTree};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(17, "df");
    QVERIFY(newNodeAdded &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees
    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue};

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue(4));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "14:DF:ROOT:BK/-9:DF:14:RD/16:DF:14:BK/-16:DF:-9:BK/7::-9:BK/17:DF:16:RD/-23:DF:-16:RD/-12:DF:-16:RD", 8, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void RedBlackTreesTests::testMoveSemantics()
{
    _reset();

    mpSearchTree = new RedBlackTree;

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");

    mpAuxSearchTree = new RedBlackTree{std::move(*mpSearchTree)};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-5:a1:-1:BK/8:b2:-1:BK/-2:e5:-5:RD/2:d4:8:RD", 5, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->addOrUpdateNode(17, "f6");
    mpSearchTree->addOrUpdateNode(0, "g7");
    mpSearchTree->addOrUpdateNode(-2, "e5_1");
    mpSearchTree->addOrUpdateNode(3, "h8");

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:g7:ROOT:BK/-2:e5_1:0:BK/17:f6:0:BK/3:h8:17:RD", 4, true));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:g7:ROOT:BK/-2:e5_1:0:BK/17:f6:0:BK/3:h8:17:RD", 4, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:g7:ROOT:BK/-2:e5_1:0:BK/17:f6:0:BK/3:h8:17:RD", 4, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value
    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->addOrUpdateNode(5, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(4, "newval");

    mpAuxSearchTree = new RedBlackTree{std::move(*mpSearchTree)};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "4:newval:ROOT:BK/-1:DF:4:BK/5::4:BK/-2:DF:-1:RD/0:DF:-1:RD", 5, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values
    _reset();

    mpSearchTree = new RedBlackTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue};

    (void)mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "4:/0:ROOT:BK/-1:DF:4:BK/5:DF:4:BK/-2:DF:-1:RD/0:DF:-1:RD", 5, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void RedBlackTreesTests::testPrintTree()
{
    _reset();

    qInfo("Creating new tree");
    mpSearchTree = new RedBlackTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue};

    QVERIFY(16 == mpSearchTree->getSize());

    qInfo("The tree nodes are: ");
    mpSearchTree->printTree();

    qInfo("Clearing tree content");
    mpSearchTree->clear();

    QVERIFY(0 == mpSearchTree->getSize());

    mpSearchTree->printTree();
}

void RedBlackTreesTests::_reset()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

bool RedBlackTreesTests::_areExpectedTreeValuesMet(const RedBlackTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered) const
{
    bool areValuesMet{false};

    if (searchTree != nullptr)
    {
        areValuesMet = (treeString == searchTree->getTreeAsString(areNodeValuesConsidered) &&
                        treeSize == searchTree->getSize());
    }

    return areValuesMet;
}

QTEST_APPLESS_MAIN(RedBlackTreesTests)

#include "tst_redblacktreestests.moc"
