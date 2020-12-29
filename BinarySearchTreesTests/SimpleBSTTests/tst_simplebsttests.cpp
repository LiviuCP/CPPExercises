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

private:
    void reset();

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

    QVERIFY(scEmptyTreeString == mpSearchTree->getTreeAsString(false));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/8:-5" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/8:-5/-1:8" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/8:-5/-1:8/2:-1" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/8:-5/-1:8/-2:-1/2:-1" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/8:-5/-1:8/-2:-1/2:-1/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/8:-5/-1:8/-2:-1/2:-1/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/-9:-5/8:-5/-1:8/-2:-1/2:-1/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/-9:-5/8:-5/-1:8/16:8/-2:-1/2:-1/14:16/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-2:-1/2:-1/14:16/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-2:-1/2:-1/14:16/17:16/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-16:-23/-2:-1/2:-1/14:16/17:16/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded && "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-16:-23/-2:-1/2:-1/14:16/17:16/-12:-16/0:2/7:2" == mpSearchTree->getTreeAsString(false));

    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    newNodeAdded = mpSearchTree->addOrUpdateNode(25, scDefaultNullValue);

    QVERIFY(!newNodeAdded && *mpAuxSearchTree == *mpSearchTree);

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{-23, -16, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17}, scDefaultValue};

    QVERIFY("-23:ROOT/-16:-23/-12:-16/-9:-12/-5:-9/-2:-5/-1:-2/0:-1/2:0/7:2/8:7/14:8/16:14/17:16" == mpSearchTree->getTreeAsString(false));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -16, -23}, scDefaultValue};

    QVERIFY("17:ROOT/16:17/14:16/8:14/7:8/2:7/0:2/-1:0/-2:-1/-5:-2/-9:-5/-12:-9/-16:-12/-23:-16" == mpSearchTree->getTreeAsString(false));

    mpSearchTree->clear();

    QVERIFY(scEmptyTreeString == mpSearchTree->getTreeAsString(false));
}

void SimpleBSTTests::testRemoveNodes()
{
    reset();

    bool nodeDeleted{false};

    mpSearchTree = new BinarySearchTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};
    mpAuxSearchTree = new BinarySearchTree{*mpSearchTree};

    nodeDeleted = mpSearchTree->deleteNode(-16);
    QVERIFY(nodeDeleted && "-5:ROOT/-9:-5/8:-5/-23:-9/-1:8/16:8/-12:-23/-2:-1/2:-1/14:16/17:16/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(8);
    QVERIFY(nodeDeleted && "-5:ROOT/-9:-5/14:-5/-23:-9/-1:14/16:14/-12:-23/-2:-1/2:-1/17:16/0:2/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(0);
    QVERIFY(nodeDeleted && "-5:ROOT/-9:-5/14:-5/-23:-9/-1:14/16:14/-12:-23/-2:-1/2:-1/17:16/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(-23);
    QVERIFY(nodeDeleted && "-5:ROOT/-9:-5/14:-5/-12:-9/-1:14/16:14/-2:-1/2:-1/17:16/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(17);
    QVERIFY(nodeDeleted && "-5:ROOT/-9:-5/14:-5/-12:-9/-1:14/16:14/-2:-1/2:-1/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(-5);
    QVERIFY(nodeDeleted && "-2:ROOT/-9:-2/14:-2/-12:-9/-1:14/16:14/2:-1/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted && "-2:ROOT/-12:-2/14:-2/-1:14/16:14/2:-1/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(14);
    QVERIFY(nodeDeleted && "-2:ROOT/-12:-2/16:-2/-1:16/2:-1/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted && "-2:ROOT/-12:-2/16:-2/2:16/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(-2);
    QVERIFY(nodeDeleted && "2:ROOT/-12:2/16:2/7:16" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted && "2:ROOT/16:2/7:16" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(16);
    QVERIFY(nodeDeleted && "2:ROOT/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted && "7:ROOT" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted && scEmptyTreeString == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(!nodeDeleted && scEmptyTreeString == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(25);
    QVERIFY(!nodeDeleted && scEmptyTreeString == mpSearchTree->getTreeAsString(false));

    // some additional (corner) cases
    nodeDeleted = mpAuxSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted && "-5:ROOT/-23:-5/8:-5/-16:-23/-1:8/16:8/-12:-16/-2:-1/2:-1/14:16/17:16/0:2/7:2" == mpAuxSearchTree->getTreeAsString(false));
    nodeDeleted = mpAuxSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted && "-5:ROOT/-23:-5/8:-5/-16:-23/0:8/16:8/-12:-16/-2:0/2:0/14:16/17:16/7:2" == mpAuxSearchTree->getTreeAsString(false));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12}, scDefaultValue};

    nodeDeleted = mpSearchTree->deleteNode(2);
    QVERIFY(nodeDeleted && "-12:ROOT" == mpSearchTree->getTreeAsString(false));

    delete mpSearchTree;
    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -12, 7}, scDefaultValue};
    *mpAuxSearchTree = *mpSearchTree;

    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted && "2:ROOT/7:2" == mpSearchTree->getTreeAsString(false));
    nodeDeleted = mpSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted && "2:ROOT" == mpSearchTree->getTreeAsString(false));

    nodeDeleted = mpAuxSearchTree->deleteNode(7);
    QVERIFY(nodeDeleted && "2:ROOT/-12:2" == mpAuxSearchTree->getTreeAsString(false));
    nodeDeleted = mpAuxSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted && "2:ROOT" == mpAuxSearchTree->getTreeAsString(false));
}

void SimpleBSTTests::reset()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

QTEST_APPLESS_MAIN(SimpleBSTTests)

#include "tst_simplebsttests.moc"
