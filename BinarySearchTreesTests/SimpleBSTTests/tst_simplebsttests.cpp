#include <QtTest>

#include "binarysearchtree.h"

class SimpleBSTTests : public QObject
{
    Q_OBJECT

public:
    SimpleBSTTests();
    ~SimpleBSTTests();

private slots:
    void testAddNodes();
    void testRemoveNodes();
    void testUpdateNodeValue();
    void testMoveSemantics();
    void testMergeTrees();
    void testTreesWithCustomNullValue();
    void testPrintTree(); // only required for improving code coverage

private:
    void _reset();
    bool _areExpectedTreeValuesMet(const BinarySearchTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered = false) const;

    BinarySearchTree* mpSearchTree;
    BinarySearchTree* mpAuxSearchTree;

    static const std::string scDefaultValue;
    static const std::string scDefaultNullValue;
    static const std::string scCustomNullValue;
    static const std::string scEmptyTreeString;
};

const std::string SimpleBSTTests::scDefaultValue{"DF"};
const std::string SimpleBSTTests::scDefaultNullValue{""};
const std::string SimpleBSTTests::scCustomNullValue{"/0"};
const std::string SimpleBSTTests::scEmptyTreeString{""};

SimpleBSTTests::SimpleBSTTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

SimpleBSTTests::~SimpleBSTTests()
{
    _reset();
}

void SimpleBSTTests::testAddNodes()
{
    _reset();

    bool newNodeAdded{false};

    mpSearchTree = new BinarySearchTree;

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/2:-1", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/-2:-1/2:-1", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/-2:-1/2:-1/7:2", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/-2:-1/2:-1/0:2/7:2", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/-2:-1/2:-1/0:2/7:2", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/0:2/7:2", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/14:16/0:2/7:2", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-2:-1/2:-1/14:16/0:2/7:2", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-2:-1/2:-1/14:16/17:16/0:2/7:2", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-16:-23/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-16:-23/-2:-1/2:-1/14:16/17:16/-12:-16/0:2/7:2", 14));

    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            *mpAuxSearchTree == *mpSearchTree);

    // some additional (corner) cases
    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{-23, -16, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-23:ROOT/-16:-23/-12:-16/-9:-12/-5:-9/-2:-5/-1:-2/0:-1/2:0/7:2/8:7/14:8/16:14/17:16", 14));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -16, -23}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:ROOT/16:17/14:16/8:14/7:8/2:7/0:2/-1:0/-2:-1/-5:-2/-9:-5/-12:-9/-16:-12/-23:-16", 14));

    mpSearchTree->clear();

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));
}

void SimpleBSTTests::testRemoveNodes()
{
    _reset();

    bool nodeDeleted{false};

    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};
    mpAuxSearchTree = new BinarySearchTree{*mpSearchTree};

    nodeDeleted = mpSearchTree->deleteNode(-16);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-12:-23/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeDeleted = mpSearchTree->deleteNode(8);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-23:-9/-1:14/16:14/-12:-23/-2:-1/2:-1/17:16/0:2/7:2", 12));

    nodeDeleted = mpSearchTree->deleteNode(0);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-23:-9/-1:14/16:14/-12:-23/-2:-1/2:-1/17:16/7:2", 11));

    nodeDeleted = mpSearchTree->deleteNode(-23);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-12:-9/-1:14/16:14/-2:-1/2:-1/17:16/7:2", 10));

    nodeDeleted = mpSearchTree->deleteNode(17);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-12:-9/-1:14/16:14/-2:-1/2:-1/7:2", 9));

    nodeDeleted = mpSearchTree->deleteNode(-5);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-9:-2/14:-2/-12:-9/-1:14/16:14/2:-1/7:2", 8));

    nodeDeleted = mpSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/14:-2/-1:14/16:14/2:-1/7:2", 7));

    nodeDeleted = mpSearchTree->deleteNode(14);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/16:-2/-1:16/2:-1/7:2", 6));

    nodeDeleted = mpSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/16:-2/2:16/7:2", 5));

    nodeDeleted = mpSearchTree->deleteNode(-2);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-12:2/16:2/7:16", 4));

    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/16:2/7:16", 3));

    nodeDeleted = mpSearchTree->deleteNode(16);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/7:2", 2));

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "7:ROOT", 1));

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->deleteNode(7);

    QVERIFY(!nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->deleteNode(25);
    QVERIFY(!nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // some additional (corner) cases
    nodeDeleted = mpAuxSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT/-23:-5/8:-5/-16:-23/-1:8/16:8/-12:-16/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeDeleted = mpAuxSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT/-23:-5/8:-5/-16:-23/0:8/16:8/-12:-16/-2:0/2:0/14:16/17:16/7:2", 12));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12}, scDefaultValue};

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT", 1));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12, 7}, scDefaultValue};
    *mpAuxSearchTree = *mpSearchTree;

    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/7:2", 2));

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT", 1));

    nodeDeleted = mpAuxSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT/-12:2", 2));

    nodeDeleted = mpAuxSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT", 1));
}

void SimpleBSTTests::testUpdateNodeValue()
{
    _reset();

    mpSearchTree = new BinarySearchTree;

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

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

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-9:h8:-5/8:b2:-5/-23:k11:-9/-1:c3:8/16:i9:8/-16:m13:-23/-2:e5:-1/2:d4:-1/14:j10:16/17:l12:16/-12:n14:-16/0:g7:2/7:f6:2", 14, true));

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

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-9:b2:-5/8:b2:-5/-23:k11:-9/-1:c3:8/16:i9:8/-16:m13:-23/-2:e5:-1/2:d4_1:-1/14:j10:16/17:L12:16/-12:n14:-16/0:g7:2/7:f6:2", 14, true));

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, "o15");
    QVERIFY(newNodeAdded &&
            "o15" == mpSearchTree->getNodeValue(25));

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-9:b2:-5/8:b2:-5/-23:k11:-9/-1:c3:8/16:i9:8/-16:m13:-23/-2:e5:-1/2:d4_1:-1/14:j10:16/17:L12:16/-12:n14:-16/0:g7:2/7:f6:2/25:o15:17", 15, true));

    // update by deleting node
    bool nodeDeleted{false};

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    nodeDeleted = mpSearchTree->deleteNode(-28);
    QVERIFY(!nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-28));

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-9:b2:-5/8:b2:-5/-23:k11:-9/-1:c3:8/16:i9:8/-16:m13:-23/-2:e5:-1/2:d4_1:-1/14:j10:16/17:L12:16/-12:n14:-16/0:g7:2/25:o15:17", 14, true));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    // test with same value for all nodes
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-5:DF:ROOT/-9:DF:-5/8:DF:-5/-23:DF:-9/-1:DF:8/16:DF:8/-16:DF:-23/-2:DF:-1/2:DF:-1/14:DF:16/17:DF:16/-12:DF:-16/0:DF:2/7:DF:2", 14, true));

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue(-5) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(0) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(16));
}

void SimpleBSTTests::testMoveSemantics()
{
    _reset();

    mpSearchTree = new BinarySearchTree;

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");

    mpAuxSearchTree = new BinarySearchTree{std::move(*mpSearchTree)};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1:ROOT/8:b2:-5/-1:c3:8/-2:e5:-1/2:d4:-1", 5, true));

    mpSearchTree->addOrUpdateNode(17, "f6");
    mpSearchTree->addOrUpdateNode(0, "g7");
    mpSearchTree->addOrUpdateNode(-2, "e5_1");
    mpSearchTree->addOrUpdateNode(3, "h8");

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:f6:ROOT/0:g7:17/-2:e5_1:0/3:h8:0", 4, true));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "17:f6:ROOT/0:g7:17/-2:e5_1:0/3:h8:0", 4, true));

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:f6:ROOT/0:g7:17/-2:e5_1:0/3:h8:0", 4, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
}

void SimpleBSTTests::testMergeTrees()
{
    _reset();

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

    BinarySearchTree searchTreeCopy{*mpSearchTree};
    BinarySearchTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0/17:l12:7/-2:e5:-1/16:i9_2:17/8:b2:16/14:j10:8", 14, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    BinarySearchTree firstMergeResult{*mpSearchTree};

    *mpSearchTree = searchTreeCopy;
    *mpAuxSearchTree = searchTreeAuxCopy;

    QVERIFY(*mpSearchTree == searchTreeCopy &&      // just a(n additional) check that the copy assignment operator and == operator work correctly
            *mpAuxSearchTree == searchTreeAuxCopy);

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2/2:d4:0/-9:h8:-5/7:f6:2/-16:m13:-9/-23:k11:-16/-12:n14:-16", 14, true));

    QVERIFY(*mpAuxSearchTree != firstMergeResult); // test the != operator too
}

void SimpleBSTTests::testTreesWithCustomNullValue()
{
    _reset();

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

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-8::-5/8:b2:-5/-9:h8:-8/-1::8/-2:e5:-1/2:d4:-1/0:g7:2/7:f6:2", 9, true));

    bool newNodeAdded{false};

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

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-8::-5/8:b2:-5/-9:h8:-8/-1:c3:8/-2::-1/2:d4_1:-1/0:g7:2/7:f6:2", 9, true));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-7, "i9");
    QVERIFY(newNodeAdded &&
            "i9" == mpSearchTree->getNodeValue(-7));

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-8::-5/8:b2:-5/-9:h8:-8/-7:i9:-8/-1:c3:8/-2::-1/2:d4_1:-1/0:g7:2/7:f6:2", 10, true));

    bool nodeDeleted{false};

    nodeDeleted = mpSearchTree->deleteNode(-25);
    QVERIFY(!nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(-25));

    nodeDeleted = mpSearchTree->deleteNode(-8);
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(-8));

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted &&
            scCustomNullValue == mpSearchTree->getNodeValue(2));

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9:h8:-7/-1:c3:8/-2::-1/7:f6:-1/0:g7:7", 8, true));

    mpAuxSearchTree = new BinarySearchTree{std::move(*mpSearchTree)};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9:h8:-7/-1:c3:8/-2::-1/7:f6:-1/0:g7:7", 8, true));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "16:ROOT/-9:16/17:16/-23:-9/14:-9/-16:-23/7:14/-12:-16", 8, false));
    QVERIFY("DF" == mpSearchTree->getNodeValue(14));

    (void)mpSearchTree->addOrUpdateNode(-9, scDefaultNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9::-7/-1:c3:8/16:DF:8/-23:DF:-9/-2::-1/7:DF:-1/14:DF:16/17:DF:16/-16:DF:-23/0:g7:7/-12:DF:-16", 14, true));

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-7:i9:-5/8:b2:-5/-9::-7/-1:c3:8/16:DF:8/-23:DF:-9/-2::-1/7:DF:-1/14:DF:16/17:DF:16/-16:DF:-23/0:g7:7/-12:DF:-16", 14, true));
    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    (void)mpAuxSearchTree->addOrUpdateNode(17, "df");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);
}

void SimpleBSTTests::testPrintTree()
{
    _reset();

    qInfo("Creating new tree");
    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(14 == mpSearchTree->getSize());

    qInfo("The tree nodes are: ");
    mpSearchTree->printTree();

    qInfo("Clearing tree content");
    mpSearchTree->clear();

    QVERIFY(0 == mpSearchTree->getSize());

    mpSearchTree->printTree();
}

void SimpleBSTTests::_reset()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

bool SimpleBSTTests::_areExpectedTreeValuesMet(const BinarySearchTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered) const
{
    bool areValuesMet{false};

    if (searchTree != nullptr)
    {
        areValuesMet = (treeString == searchTree->getTreeAsString(areNodeValuesConsidered) &&
                        treeSize == searchTree->getSize());
    }

    return areValuesMet;
}

QTEST_APPLESS_MAIN(SimpleBSTTests)

#include "tst_simplebsttests.moc"
