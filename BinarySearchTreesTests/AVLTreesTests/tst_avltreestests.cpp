#include <QtTest>

#include "avltree.h"

class AVLTreesTests : public QObject
{
    Q_OBJECT

public:
    AVLTreesTests();
    ~AVLTreesTests();

private slots:
    void testAddNodes();
    void testRemoveNodes();

private:
    void _reset();
    bool _areExpectedTreeValuesMet(const AVLTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered = false) const;

    AVLTree* mpSearchTree;
    AVLTree* mpAuxSearchTree;

    static const std::string scDefaultValue;
    static const std::string scDefaultNullValue;
    static const std::string scCustomNullValue;
    static const std::string scEmptyTreeString;
};

const std::string AVLTreesTests::scDefaultValue{"DF"};
const std::string AVLTreesTests::scDefaultNullValue{""};
const std::string AVLTreesTests::scCustomNullValue{"/0"};
const std::string AVLTreesTests::scEmptyTreeString{""};

AVLTreesTests::AVLTreesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

AVLTreesTests::~AVLTreesTests()
{
    _reset();
}

void AVLTreesTests::testAddNodes()
{
    _reset();

    bool newNodeAdded{false};

    mpSearchTree = new AVLTree;

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/8:-1", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/8:-1/2:8", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/8:-1/-2:-5/2:8", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5/2:7/8:7", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5/2:7/8:7/0:2", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/8:7/0:2", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/8:7/0:2/16:8", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/0:2/8:14/16:14", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/-23:-9/0:2/8:14/16:14", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/-23:-9/0:2/8:14/16:14/17:16", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-16:-5/-2:-5/2:7/14:7/-23:-16/-9:-16/0:2/8:14/16:14/17:16", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5/0:2/8:14/16:14/17:16", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode(19, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree,"-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5/0:2/8:14/17:14/16:17/19:17", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-15, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5/0:2/8:14/17:14/-15:-12/16:17/19:17", 16));

    // adding nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue};

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

    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/17:7/-23:-16/-12:-16/-2:-5/0:2/14:17/19:17/-15:-12/8:14/16:14/25:19", 17));

    // some additional (corner) cases
    delete mpSearchTree;
    mpSearchTree = new AVLTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/8:-1/-16:-12/-5:-12/2:8/16:8/-23:-16/-15:-16/-9:-5/-2:-5/0:2/7:2/14:16/17:16/19:17", 16));

    delete mpSearchTree;
    mpSearchTree = new AVLTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-9:0/14:0/-15:-9/-2:-9/7:14/17:14/-16:-15/-12:-15/-5:-2/-1:-2/2:7/8:7/16:17/19:17/-23:-16", 16));

    delete mpSearchTree;
    mpSearchTree = new AVLTree{std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-9:ROOT/-15:-9/2:-9/-16:-15/-12:-15/-2:2/14:2/-23:-16/-5:-2/-1:-2/7:14/17:14/0:-1/8:7/16:17/19:17", 16));

    mpSearchTree->clear();

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators
    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 2, 4, 0, 1}, scDefaultValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, -3, 2, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree); // in this particular case due to AVL tree construction rules the trees become equal when third element is being added (unlike the basic BST)

    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{-3, -5, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-3, 2, -5, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{}, scDefaultValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    delete mpSearchTree;
    mpSearchTree = new AVLTree{scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
}

void AVLTreesTests::testRemoveNodes()
{
    _reset();

    bool nodeDeleted{false};

    mpSearchTree = new AVLTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue};
    mpAuxSearchTree = new AVLTree{*mpSearchTree};

    nodeDeleted = mpSearchTree->deleteNode(-16);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-5:-9/2:7/14:7/-23:-15/-12:-15/-2:-5/0:2/8:14/17:14/16:17/19:17", 15));

    nodeDeleted = mpSearchTree->deleteNode(8);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-5:-9/2:7/16:7/-23:-15/-12:-15/-2:-5/0:2/14:16/17:16/19:17", 14));

    nodeDeleted = mpSearchTree->deleteNode(0);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/16:-1/-15:-9/-5:-9/7:16/17:16/-23:-15/-12:-15/-2:-5/2:7/14:7/19:17", 13));

    nodeDeleted = mpSearchTree->deleteNode(-23);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/16:-1/-15:-9/-5:-9/7:16/17:16/-12:-15/-2:-5/2:7/14:7/19:17", 12));

    nodeDeleted = mpSearchTree->deleteNode(19);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/16:-1/-15:-9/-5:-9/7:16/17:16/-12:-15/-2:-5/2:7/14:7", 11));

    nodeDeleted = mpSearchTree->deleteNode(17);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-5:-9/2:7/16:7/-12:-15/-2:-5/14:16", 10));

    nodeDeleted = mpSearchTree->deleteNode(-5);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-2:-9/2:7/16:7/-12:-15/14:16", 9));

    nodeDeleted = mpSearchTree->deleteNode(-9);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-15:-12/-2:-12/2:7/16:7/14:16", 8));

    nodeDeleted = mpSearchTree->deleteNode(-15);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-2:-12/2:7/16:7/14:16", 7));

    nodeDeleted = mpSearchTree->deleteNode(14);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-2:-12/2:7/16:7", 6));

    nodeDeleted = mpSearchTree->deleteNode(-1);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-12:2/7:2/-2:-12/16:7", 5));

    nodeDeleted = mpSearchTree->deleteNode(-2);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-12:2/7:2/16:7", 4));

    nodeDeleted = mpSearchTree->deleteNode(-12);
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "7:ROOT/2:7/16:7", 3));

    nodeDeleted = mpSearchTree->deleteNode(16); // root and two children, erase right child
    QVERIFY(nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "7:ROOT/2:7", 2));

    nodeDeleted = mpSearchTree->deleteNode(2); // root and left child, erase left child
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

    // we will stop checking the nodeDeleted variable (obvious enough)
    (void)mpAuxSearchTree->deleteNode(-15);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5/0:2/8:14/17:14/16:17/19:17", 15));

    (void)mpAuxSearchTree->deleteNode(19);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5/0:2/8:14/17:14/16:17", 14));

    (void)mpAuxSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-2:-5/0:2/8:14/17:14/16:17", 13));

    (void)mpAuxSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-23:-9/-5:-9/2:7/14:7/-2:-5/0:2/8:14/17:14/16:17", 12));

    (void)mpAuxSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-23:-9/-5:-9/2:7/14:7/-2:-5/0:2/8:14/16:14", 11));

    (void)mpAuxSearchTree->deleteNode(-23);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/0:2/8:14/16:14", 10));

    (void)mpAuxSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/16:7/0:2/8:16", 9));

    (void)mpAuxSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/8:7/0:2", 8));

    (void)mpAuxSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5/2:7/8:7/0:2", 7));

    (void)mpAuxSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5/2:7/8:7", 6));

    (void)mpAuxSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/8:-1/-2:-5/2:8", 5));

    (void)mpAuxSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/8:-1/2:8", 4));

    (void)mpAuxSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/8:-1", 3));

    (void)mpAuxSearchTree->deleteNode(-1); // root and two children, erase root
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "8:ROOT/-5:8", 2));

    (void)mpAuxSearchTree->deleteNode(8); // root and left child, erase root
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT", 1));

    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-16:-12/-9:-12/2:7/14:7/-23:-16/-15:-16/-2:-9/0:2/8:14/17:14/16:17/19:17", 15));

    (void)mpSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-16:-12/-9:-12/2:7/16:7/-23:-16/-15:-16/-2:-9/0:2/14:16/17:16/19:17", 14));

    (void)mpSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/7:16/17:16/-23:-16/-15:-16/-2:-9/2:7/14:7/19:17", 13));

    (void)mpSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/7:16/17:16/-23:-16/-15:-16/-2:-9/14:7/19:17", 12));

    (void)mpSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/7:16/17:16/-23:-16/-15:-16/14:7/19:17", 11));

    (void)mpSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/14:16/17:16/-23:-16/-15:-16/19:17", 10));

    (void)mpSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT/-12:14/17:14/-16:-12/-9:-12/16:17/19:17/-23:-16/-15:-16", 9));

    (void)mpSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT/-16:14/17:14/-23:-16/-12:-16/16:17/19:17/-15:-12", 8));

    (void)mpSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT/-16:14/17:14/-23:-16/-12:-16/19:17/-15:-12", 7));

    (void)mpSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-16:-12/17:-12/-23:-16/-15:-16/19:17", 6));

    (void)mpSearchTree->deleteNode(-23);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-16:-12/17:-12/-15:-16/19:17", 5));

    (void)mpSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-16:-12/19:-12/-15:-16", 4));

    (void)mpSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-15:-12/19:-12", 3));

    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    (void)mpSearchTree->deleteNode(-23);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/8:-1/-16:-12/-5:-12/2:8/16:8/-15:-16/-9:-5/-2:-5/0:2/7:2/14:16/17:16/19:17", 15));

    (void)mpSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/8:-1/-15:-12/-5:-12/2:8/16:8/-9:-5/-2:-5/0:2/7:2/14:16/17:16/19:17", 14));

    (void)mpSearchTree->deleteNode(-15);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/8:-1/-12:-9/-5:-9/2:8/16:8/-2:-5/0:2/7:2/14:16/17:16/19:17", 13));

    (void)mpSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/-1:8/16:8/-5:-1/2:-1/14:16/17:16/-9:-5/-2:-5/0:2/7:2/19:17", 12));

    (void)mpSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/-1:8/16:8/-5:-1/2:-1/14:16/17:16/-2:-5/0:2/7:2/19:17", 11));

    (void)mpSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/-1:8/16:8/-2:-1/2:-1/14:16/17:16/0:2/7:2/19:17", 10));

    (void)mpSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/0:8/16:8/-1:0/2:0/14:16/17:16/7:2/19:17", 9));

    (void)mpSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/2:8/16:8/0:2/7:2/14:16/17:16/19:17", 8));

    (void)mpSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/2:8/16:8/7:2/14:16/17:16/19:17", 7));

    (void)mpSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "16:ROOT/8:16/17:16/7:8/14:8/19:17", 6));

    (void)mpSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "16:ROOT/8:16/17:16/14:8/19:17", 5));

    (void)mpSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "16:ROOT/14:16/17:16/19:17", 4));

    (void)mpSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:ROOT/16:17/19:17", 3));

    (void)mpSearchTree->deleteNode(16); // root and two children, erase left child
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "17:ROOT/19:17", 2));

    (void)mpSearchTree->deleteNode(17); // root and right child, erase root
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "19:ROOT", 1));

    mpAuxSearchTree = new AVLTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue, scCustomNullValue};

    (void)mpAuxSearchTree->deleteNode(19);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT/-9:0/14:0/-15:-9/-2:-9/7:14/17:14/-16:-15/-12:-15/-5:-2/-1:-2/2:7/8:7/16:17/-23:-16", 15));

    (void)mpAuxSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT/-9:0/14:0/-15:-9/-2:-9/7:14/16:14/-16:-15/-12:-15/-5:-2/-1:-2/2:7/8:7/-23:-16", 14));

    (void)mpAuxSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT/-9:0/7:0/-15:-9/-2:-9/2:7/14:7/-16:-15/-12:-15/-5:-2/-1:-2/8:14/-23:-16", 13));

    (void)mpAuxSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/0:-9/-16:-15/-12:-15/-2:0/7:0/-23:-16/-5:-2/-1:-2/2:7/8:7", 12));

    (void)mpAuxSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/0:-9/-16:-15/-12:-15/-2:0/7:0/-23:-16/-5:-2/-1:-2/2:7", 11));

    (void)mpAuxSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/0:-9/-16:-15/-12:-15/-2:0/2:0/-23:-16/-5:-2/-1:-2", 10));

    (void)mpAuxSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/-2:-9/-16:-15/-12:-15/-5:-2/0:-2/-23:-16/-1:0", 9));

    (void)mpAuxSearchTree->deleteNode(0);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/-2:-9/-16:-15/-12:-15/-5:-2/-1:-2/-23:-16", 8));

    (void)mpAuxSearchTree->deleteNode(-1);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/-2:-9/-16:-15/-12:-15/-5:-2/-23:-16", 7));

    (void)mpAuxSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT/-16:-15/-9:-15/-23:-16/-12:-9/-5:-9", 6));

    (void)mpAuxSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT/-16:-15/-9:-15/-23:-16/-12:-9", 5));

    (void)mpAuxSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT/-16:-15/-12:-15/-23:-16", 4));

    (void)mpAuxSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-16:ROOT/-23:-16/-15:-16", 3));

    _reset();

    mpSearchTree = new AVLTree{std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->deleteNode(14);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-9:ROOT/-15:-9/2:-9/-16:-15/-12:-15/-2:2/16:2/-23:-16/-5:-2/-1:-2/7:16/17:16/0:-1/8:7/19:17", 15));

    (void)mpSearchTree->deleteNode(-9);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-15:-5/2:-5/-16:-15/-12:-15/-1:2/16:2/-23:-16/-2:-1/0:-1/7:16/17:16/8:7/19:17", 14));

    (void)mpSearchTree->deleteNode(8);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-15:-5/2:-5/-16:-15/-12:-15/-1:2/16:2/-23:-16/-2:-1/0:-1/7:16/17:16/19:17", 13));

    (void)mpSearchTree->deleteNode(-12);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-5:2/16:2/-16:-5/-1:-5/7:16/17:16/-23:-16/-15:-16/-2:-1/0:-1/19:17", 12));

    (void)mpSearchTree->deleteNode(16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-5:2/17:2/-16:-5/-1:-5/7:17/19:17/-23:-16/-15:-16/-2:-1/0:-1", 11));

    (void)mpSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-2:2/17:2/-16:-2/-1:-2/7:17/19:17/-23:-16/-15:-16/0:-1", 10));

    (void)mpSearchTree->deleteNode(7);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-2:2/17:2/-16:-2/-1:-2/19:17/-23:-16/-15:-16/0:-1", 9));

    (void)mpSearchTree->deleteNode(-15);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-2:2/17:2/-16:-2/-1:-2/19:17/-23:-16/0:-1", 8));

    (void)mpSearchTree->deleteNode(17);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-16:-2/2:-2/-23:-16/-1:2/19:2/0:-1", 7));

    (void)mpSearchTree->deleteNode(-2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-16:-1/2:-1/-23:-16/0:2/19:2", 6));

    (void)mpSearchTree->deleteNode(2);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-16:-1/19:-1/-23:-16/0:19", 5));

    (void)mpSearchTree->deleteNode(-16);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-23:-1/19:-1/0:19", 4));

    (void)mpSearchTree->deleteNode(19);
    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-23:-1/0:-1", 3));

    mpAuxSearchTree = new AVLTree{std::vector<int>{-3, -5, 2}, scDefaultValue};

    (void)mpAuxSearchTree->deleteNode(-5);
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-3:ROOT/2:-3", 2));

    (void)mpAuxSearchTree->deleteNode(2); // root and right child, erase right child
    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-3:ROOT", 1));

    _reset();

    // deleting null node from custom null value tree
    mpSearchTree = new AVLTree{std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue, scCustomNullValue};

    nodeDeleted = mpSearchTree->deleteNode(-5);
    QVERIFY(!nodeDeleted &&
            _areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-1:2/3:2/-2:-1/4:3", 5, false));

    // deleting same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = new AVLTree{std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue};

    nodeDeleted = mpSearchTree->deleteNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeDeleted = mpAuxSearchTree->deleteNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT/-1:2/4:2/-2:-1", 4, false));
}

void AVLTreesTests::_reset()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

bool AVLTreesTests::_areExpectedTreeValuesMet(const AVLTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered) const
{
    bool areValuesMet{false};

    if (searchTree != nullptr)
    {
        areValuesMet = (treeString == searchTree->getTreeAsString(areNodeValuesConsidered) &&
                        treeSize == searchTree->getSize());
    }

    return areValuesMet;
}

QTEST_APPLESS_MAIN(AVLTreesTests)

#include "tst_avltreestests.moc"
