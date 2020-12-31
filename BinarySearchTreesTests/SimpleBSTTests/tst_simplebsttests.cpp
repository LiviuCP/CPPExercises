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

private:
    void reset();
    bool areExpectedTreeValuesMet(const BinarySearchTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered = false);

    BinarySearchTree* mpSearchTree;
    BinarySearchTree* mpAuxSearchTree;

    static const std::string scDefaultValue;
    static const std::string scDefaultNullValue;
    static const std::string scEmptyTreeString;
};

const std::string SimpleBSTTests::scDefaultValue{"default"};
const std::string SimpleBSTTests::scDefaultNullValue{""};
const std::string SimpleBSTTests::scEmptyTreeString{""};

SimpleBSTTests::SimpleBSTTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

SimpleBSTTests::~SimpleBSTTests()
{
    reset();
}

void SimpleBSTTests::testAddNodes()
{
    reset();

    bool newNodeAdded{false};

    mpSearchTree = new BinarySearchTree;

    QVERIFY(0 == mpSearchTree->getSize() && scEmptyTreeString == mpSearchTree->getTreeAsString(false));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/2:-1", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/-2:-1/2:-1", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/-2:-1/2:-1/7:2", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5/-1:8/-2:-1/2:-1/0:2/7:2", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/-2:-1/2:-1/0:2/7:2", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/0:2/7:2", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/14:16/0:2/7:2", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-2:-1/2:-1/14:16/0:2/7:2", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-2:-1/2:-1/14:16/17:16/0:2/7:2", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-16:-23/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-16:-23/-2:-1/2:-1/14:16/17:16/-12:-16/0:2/7:2", 14));

    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            *mpAuxSearchTree == *mpSearchTree);

    // some additional (corner) cases
    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{-23, -16, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-23:ROOT/-16:-23/-12:-16/-9:-12/-5:-9/-2:-5/-1:-2/0:-1/2:0/7:2/8:7/14:8/16:14/17:16", 14));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -16, -23}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "17:ROOT/16:17/14:16/8:14/7:8/2:7/0:2/-1:0/-2:-1/-5:-2/-9:-5/-12:-9/-16:-12/-23:-16", 14));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));
}

void SimpleBSTTests::testRemoveNodes()
{
    reset();

    bool nodeDeleted{false};

    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};
    mpAuxSearchTree = new BinarySearchTree{*mpSearchTree};

    nodeDeleted = mpSearchTree->deleteNode(-16);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-12:-23/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeDeleted = mpSearchTree->deleteNode(8);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-23:-9/-1:14/16:14/-12:-23/-2:-1/2:-1/17:16/0:2/7:2", 12));

    nodeDeleted = mpSearchTree->deleteNode(0);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-23:-9/-1:14/16:14/-12:-23/-2:-1/2:-1/17:16/7:2", 11));

    nodeDeleted = mpSearchTree->deleteNode(-23);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-12:-9/-1:14/16:14/-2:-1/2:-1/17:16/7:2", 10));

    nodeDeleted = mpSearchTree->deleteNode(17);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-9:-5/14:-5/-12:-9/-1:14/16:14/-2:-1/2:-1/7:2", 9));

    nodeDeleted = mpSearchTree->deleteNode(-5);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-9:-2/14:-2/-12:-9/-1:14/16:14/2:-1/7:2", 8));

    nodeDeleted = mpSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/14:-2/-1:14/16:14/2:-1/7:2", 7));

    nodeDeleted = mpSearchTree->deleteNode(14);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/16:-2/-1:16/2:-1/7:2", 6));

    nodeDeleted = mpSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-12:-2/16:-2/2:16/7:2", 5));

    nodeDeleted = mpSearchTree->deleteNode(-2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-12:2/16:2/7:16", 4));

    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/16:2/7:16", 3));

    nodeDeleted = mpSearchTree->deleteNode(16);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/7:2", 2));

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "7:ROOT", 1));

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->deleteNode(7);

    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    nodeDeleted = mpSearchTree->deleteNode(25);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // some additional (corner) cases
    nodeDeleted = mpAuxSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT/-23:-5/8:-5/-16:-23/-1:8/16:8/-12:-16/-2:-1/2:-1/14:16/17:16/0:2/7:2", 13));

    nodeDeleted = mpAuxSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT/-23:-5/8:-5/-16:-23/0:8/16:8/-12:-16/-2:0/2:0/14:16/17:16/7:2", 12));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12}, scDefaultValue};

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT", 1));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12, 7}, scDefaultValue};
    *mpAuxSearchTree = *mpSearchTree;

    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/7:2", 2));

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT", 1));

    nodeDeleted = mpAuxSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT/-12:2", 2));

    nodeDeleted = mpAuxSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT", 1));
}

void SimpleBSTTests::testUpdateNodeValue()
{
    reset();

    mpSearchTree = new BinarySearchTree;

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1:ROOT/-9:h8:-5/8:b2:-5/-23:k11:-9/-1:c3:8/16:i9:8/-16:m13:-23/-2:e5:-1/2:d4:-1/14:j10:16/17:l12:16/-12:n14:-16/0:g7:2/7:f6:2", 14, true));

    QVERIFY("a1" == mpSearchTree->getNodeValue(-5));
    QVERIFY("i9" == mpSearchTree->getNodeValue(16));
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

    QVERIFY(15 == mpSearchTree->getSize());

    // update by deleting node
    bool nodeDeleted{false};

    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    nodeDeleted = mpSearchTree->deleteNode(-28);
    QVERIFY(!nodeDeleted &&
            scDefaultNullValue == mpSearchTree->getNodeValue(-28));

    QVERIFY(14 == mpSearchTree->getSize());
}

void SimpleBSTTests::reset()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

bool SimpleBSTTests::areExpectedTreeValuesMet(const BinarySearchTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered)
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
