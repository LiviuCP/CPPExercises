#include <QtTest>

#include "testutils.h"
#include "binarysearchtree.h"

using namespace TestUtils;

class SimpleBSTTests : public QObject
{
    Q_OBJECT

public:
    SimpleBSTTests();
    ~SimpleBSTTests();

private slots:
    void init();
    void cleanup();

    void testAddNodes();
    void testRemoveNodes();
    void testUpdateNodeValue();
    void testMoveSemantics();
    void testMergeTrees();
    void testPrintTree(); // only required for improving code coverage

private:
    void _resetTreeObjects();

    BinarySearchTree* mpSearchTree;
    BinarySearchTree* mpAuxSearchTree;
};

SimpleBSTTests::SimpleBSTTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
    BinarySearchTree::enableLogging(false);
}

SimpleBSTTests::~SimpleBSTTests()
{
    // not implemented as cleanup() does all the job
}

void SimpleBSTTests::init()
{
    QVERIFY(nullptr == mpSearchTree && nullptr == mpAuxSearchTree);
}

void SimpleBSTTests::cleanup()
{
    _resetTreeObjects();
    BinarySearchTree::enableLogging(false);
}

void SimpleBSTTests::testAddNodes()
{
    bool newNodeAdded{false};

    mpSearchTree = new BinarySearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5R", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5R/-1:8L", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5R/-1:8L/2:-1R", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5R/-1:8L/-2:-1/2:-1", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5R/-1:8L/-2:-1/2:-1/7:2R", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5R/-1:8L/-2:-1/2:-1/0:2/7:2", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8L/-2:-1/2:-1/0:2/7:2", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/0:2/7:2", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/14:16L/0:2/7:2", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-2:-1/2:-1/14:16L/0:2/7:2", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-2:-1/2:-1/14:16/17:16/0:2/7:2", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode(19, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2/19:17R", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-15, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2/19:17R/-15:-12L", 16));

    // adding nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue};

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

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-16:-23R/-2:-1/2:-1/14:16/17:16/-12:-16R/0:2/7:2/19:17R/-15:-12L/25:19R", 17));

    // some additional (corner) cases
    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-23:ROOT/-16:-23R/-15:-16R/-12:-15R/-9:-12R/-5:-9R/-2:-5R/-1:-2R/0:-1R/2:0R/7:2R/8:7R/14:8R/16:14R/17:16R/19:17R", 16));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "19:ROOT/17:19L/16:17L/14:16L/8:14L/7:8L/2:7L/0:2L/-1:0L/-2:-1L/-5:-2L/-9:-5L/-12:-9L/-15:-12L/-16:-15L/-23:-16L", 16));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-23:ROOT/19:-23R/-16:19L/17:-16R/-15:17L/16:-15R/-12:16L/14:-12R/-9:14L/8:-9R/-5:8L/7:-5R/-2:7L/2:-2R/-1:2L/0:-1R", 16));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 2, -3, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, -3, 2, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 2, -3, 4, 1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{-3, -5, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-3, 2, -5, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{}, scDefaultValue, scDefaultNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
}

void SimpleBSTTests::testRemoveNodes()
{
    bool nodeDeleted{false};

    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};
    mpAuxSearchTree = new BinarySearchTree{*mpSearchTree};

    nodeDeleted = mpSearchTree->removeNode(-16);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9L/-1:8/16:8/-12:-23R/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeDeleted = mpSearchTree->removeNode(8);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-23:-9L/-1:14/16:14/-12:-23R/-2:-1/2:-1/17:16R/0:2/7:2", 12));

    nodeDeleted = mpSearchTree->removeNode(0);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-23:-9L/-1:14/16:14/-12:-23R/-2:-1/2:-1/17:16R/7:2R", 11));

    nodeDeleted = mpSearchTree->removeNode(-23);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-12:-9L/-1:14/16:14/-2:-1/2:-1/17:16R/7:2R", 10));

    nodeDeleted = mpSearchTree->removeNode(17);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-12:-9L/-1:14/16:14/-2:-1/2:-1/7:2R", 9));

    nodeDeleted = mpSearchTree->removeNode(-5);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-9:-2/14:-2/-12:-9L/-1:14/16:14/2:-1R/7:2R", 8));

    nodeDeleted = mpSearchTree->removeNode(-9);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/14:-2/-1:14/16:14/2:-1R/7:2R", 7));

    nodeDeleted = mpSearchTree->removeNode(14);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/16:-2/-1:16L/2:-1R/7:2R", 6));

    nodeDeleted = mpSearchTree->removeNode(-1);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/16:-2/2:16L/7:2R", 5));

    nodeDeleted = mpSearchTree->removeNode(-2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-12:2/16:2/7:16L", 4));

    nodeDeleted = mpSearchTree->removeNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/16:2R/7:16L", 3));

    nodeDeleted = mpSearchTree->removeNode(16);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/7:2R", 2));

    nodeDeleted = mpSearchTree->removeNode(2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "7:ROOT", 1));

    nodeDeleted = mpSearchTree->removeNode(7);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode(7);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->removeNode(25);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // some additional (corner) cases
    nodeDeleted = mpAuxSearchTree->removeNode(-9);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT/-23:-5/8:-5/-16:-23R/-1:8/16:8/-12:-16R/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeDeleted = mpAuxSearchTree->removeNode(-1);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT/-23:-5/8:-5/-16:-23R/0:8/16:8/-12:-16R/-2:0/2:0/14:16/17:16/7:2R", 12));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12}, scDefaultValue};

    nodeDeleted = mpSearchTree->removeNode(2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT", 1));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12, 7}, scDefaultValue};
    *mpAuxSearchTree = *mpSearchTree;

    nodeDeleted = mpSearchTree->removeNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/7:2R", 2));

    nodeDeleted = mpSearchTree->removeNode(7);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT", 1));

    nodeDeleted = mpAuxSearchTree->removeNode(7);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT/-12:2L", 2));

    nodeDeleted = mpAuxSearchTree->removeNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT", 1));

    _resetTreeObjects();

    // deleting null node from custom null value tree
    mpSearchTree = new BinarySearchTree{std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue, scCustomNullValue};

    nodeDeleted = mpSearchTree->removeNode(-5);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-2:-1/3:-1/2:3/4:3", 5));

    // deleting same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue};

    nodeDeleted = mpSearchTree->removeNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeDeleted = mpAuxSearchTree->removeNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-2:-1/4:-1/2:4L", 4));
}

void SimpleBSTTests::testUpdateNodeValue()
{
    mpSearchTree = new BinarySearchTree;

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-9:h8:-5/8:b2:-5/-23:k11:-9L/-1:c3:8/16:i9:8/-16:m13:-23R/-2:e5:-1/2:d4:-1/14:j10:16/17:l12:16/-12:n14:-16R/0:g7:2/7:f6:2", 14, true));

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-9:b2:-5/8:b2:-5/-23:k11:-9L/-1:c3:8/17:L12:8/-16:m13:-23R/-2:e5:-1/2:d4_1:-1/14:j10:17/25:o15:17/-12:n14:-16R/0:g7:2L/16:i9_new:14R", 14, true));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    // test with same value for all nodes
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:DF:ROOT/-9:DF:-5/8:DF:-5/-23:DF:-9L/-1:DF:8/16:DF:8/-16:DF:-23R/-2:DF:-1/2:DF:-1/14:DF:16/17:DF:16/-12:DF:-16R/0:DF:2/7:DF:2", 14, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue(-5) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(0) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(16));

    // test updating custom null value tree node values
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{scCustomNullValue};

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-8, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(0, "g7");
    (void)mpSearchTree->addOrUpdateNode(-9, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-8::-5/8:b2:-5/-9:h8:-8L/-1::8L/-2:e5:-1/2:d4:-1/0:g7:2/7:f6:2", 9, true));
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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9:h8:-7L/0:g7:8L/-2::0/7:f6:0/-1::-2R", 8, true));

    mpAuxSearchTree = new BinarySearchTree{*mpSearchTree};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(17, "df");
    QVERIFY(newNodeAdded &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue};

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue(4));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "16:DF:ROOT/-9:DF:16/17:DF:16/-23:DF:-9/14:DF:-9/-16:DF:-23R/7::14L/-12:DF:-16R", 8, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void SimpleBSTTests::testMoveSemantics()
{
    mpSearchTree = new BinarySearchTree;

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");

    mpAuxSearchTree = new BinarySearchTree{std::move(*mpSearchTree)};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1:ROOT/8:b2:-5R/-1:c3:8L/-2:e5:-1/2:d4:-1", 5, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->addOrUpdateNode(17, "f6");
    mpSearchTree->addOrUpdateNode(0, "g7");
    mpSearchTree->addOrUpdateNode(-2, "e5_1");
    mpSearchTree->addOrUpdateNode(3, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "17:f6:ROOT/0:g7:17L/-2:e5_1:0/3:h8:0", 4, true));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "17:f6:ROOT/0:g7:17L/-2:e5_1:0/3:h8:0", 4, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "17:f6:ROOT/0:g7:17L/-2:e5_1:0/3:h8:0", 4, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->addOrUpdateNode(5, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(4, "newval");

    mpAuxSearchTree = new BinarySearchTree{std::move(*mpSearchTree)};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-2:DF:ROOT/5::-2R/4:newval:5L/0:DF:4L/-1:DF:0L", 5, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue};

    (void)mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-2:DF:ROOT/5:DF:-2R/4:NULL:5L/0:DF:4L/-1:DF:0L", 5, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void SimpleBSTTests::testMergeTrees()
{
    mpSearchTree = new BinarySearchTree;

    (void)mpSearchTree->addOrUpdateNode(-5, "a1_1");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-23, "k11");
    (void)mpSearchTree->addOrUpdateNode(17, "l12");
    (void)mpSearchTree->addOrUpdateNode(-12, "n14");
    (void)mpSearchTree->addOrUpdateNode(16, "i9_1");
    (void)mpSearchTree->addOrUpdateNode(0, "g7_1");

    mpAuxSearchTree = new BinarySearchTree;

    (void)mpAuxSearchTree->addOrUpdateNode(8, "b2");
    (void)mpAuxSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpAuxSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpAuxSearchTree->addOrUpdateNode(0, "g7_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-5, "a1_2");
    (void)mpAuxSearchTree->addOrUpdateNode(16, "i9_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-9, "h8");
    (void)mpAuxSearchTree->addOrUpdateNode(14, "j10");
    (void)mpAuxSearchTree->addOrUpdateNode(-16, "m13");

    const BinarySearchTree searchTreeCopy{*mpSearchTree};
    const BinarySearchTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    // first (main) merge
    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    const BinarySearchTree mainTreeAfterFirstMerge{*mpSearchTree};

    // merge empty tree into unempty tree
    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(*mpSearchTree == mainTreeAfterFirstMerge);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    // merge unempty tree with itself
    mpSearchTree->mergeTree(*mpSearchTree);
    QVERIFY(*mpSearchTree == mainTreeAfterFirstMerge);

    // merge unempty tree into empty tree
    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(*mpAuxSearchTree == mainTreeAfterFirstMerge);

    // merge empty tree with itself
    mpSearchTree->mergeTree(*mpSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));

    // merge two empty trees
    mpAuxSearchTree->clear();
    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    // do inverse merge operation comparing to first merge
    *mpSearchTree = searchTreeCopy;
    *mpAuxSearchTree = searchTreeAuxCopy;

    QVERIFY(*mpSearchTree == searchTreeCopy &&      // just a(n additional) check that the copy assignment operator and == operator work correctly
            *mpAuxSearchTree == searchTreeAuxCopy);

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14, true));

    QVERIFY(*mpAuxSearchTree != mainTreeAfterFirstMerge); // test the != operator too

    // actually not necesarily required to test this as the two trees SHOULD have same null value in order to be merged (assert statement enforces this)
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // merge trees with (same) custom null value
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{scCustomNullValue};

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

    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    (void)mpAuxSearchTree->addOrUpdateNode(-9, scDefaultNullValue);

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9::-7L/-1:c3:8/16:DF:8/-23:DF:-9L/-2::-1/7:DF:-1/14:DF:16/17:DF:16/-16:DF:-23R/0:g7:7L/-12:DF:-16R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void SimpleBSTTests::testPrintTree()
{
    qInfo("Creating new tree");
    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(14 == mpSearchTree->getSize());

    qInfo("The tree nodes are: ");
    mpSearchTree->printTree();

    qInfo("Clearing tree content");
    mpSearchTree->clear();

    QVERIFY(0 == mpSearchTree->getSize());

    mpSearchTree->printTree();
    qInfo("The tree has no nodes");
}

void SimpleBSTTests::_resetTreeObjects()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

QTEST_APPLESS_MAIN(SimpleBSTTests)

#include "tst_simplebsttests.moc"
