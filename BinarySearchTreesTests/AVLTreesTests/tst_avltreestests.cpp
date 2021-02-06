#include <QtTest>

#include "testutils.h"
#include "avltree.h"

using namespace TestUtils;

class AVLTreesTests : public QObject
{
    Q_OBJECT

public:
    AVLTreesTests();
    ~AVLTreesTests();

private slots:
    void init();
    void cleanup();

    void testAddNodes();
    void testRemoveNodes();
    void testUpdateNodeValue();
    void testMoveSemantics();
    void testMergeTrees();

private:
    void _resetTreeObjects();

    AVLTree* mpSearchTree;
    AVLTree* mpAuxSearchTree;
};

AVLTreesTests::AVLTreesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
    BinarySearchTree::enableLogging(false);
}

AVLTreesTests::~AVLTreesTests()
{
    // not implemented as cleanup() does all the job
}

void AVLTreesTests::init()
{
    QVERIFY(nullptr == mpSearchTree && nullptr == mpAuxSearchTree);
}

void AVLTreesTests::cleanup()
{
    _resetTreeObjects();
    BinarySearchTree::enableLogging(false);
}

void AVLTreesTests::testAddNodes()
{
    bool newNodeAdded{false};

    mpSearchTree = new AVLTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/8:-5R", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/8:-1", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/8:-1/2:8L", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/8:-1/-2:-5R/2:8L", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5R/2:7/8:7", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5R/2:7/8:7/0:2L", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/8:7/0:2L", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/8:7/0:2L/16:8R", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/0:2L/8:14/16:14", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/-23:-9L/0:2L/8:14/16:14", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/-23:-9L/0:2L/8:14/16:14/17:16R", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-5:-1/7:-1/-16:-5/-2:-5/2:7/14:7/-23:-16/-9:-16/0:2L/8:14/16:14/17:16R", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5R/0:2L/8:14/16:14/17:16R", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode(19, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree,"-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5R/0:2L/8:14/17:14/16:17/19:17", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-15, scDefaultValue);
    QVERIFY(newNodeAdded &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5R/0:2L/8:14/17:14/-15:-12L/16:17/19:17", 16));

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

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/17:7/-23:-16/-12:-16/-2:-5R/0:2L/14:17/19:17/-15:-12L/8:14/16:14/25:19R", 17));

    // some additional (corner) cases
    delete mpSearchTree;
    mpSearchTree = new AVLTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/8:-1/-16:-12/-5:-12/2:8/16:8/-23:-16/-15:-16/-9:-5/-2:-5/0:2/7:2/14:16/17:16/19:17R", 16));

    delete mpSearchTree;
    mpSearchTree = new AVLTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-9:0/14:0/-15:-9/-2:-9/7:14/17:14/-16:-15/-12:-15/-5:-2/-1:-2/2:7/8:7/16:17/19:17/-23:-16L", 16));

    delete mpSearchTree;
    mpSearchTree = new AVLTree{std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-9:ROOT/-15:-9/2:-9/-16:-15/-12:-15/-2:2/14:2/-23:-16L/-5:-2/-1:-2/7:14/17:14/0:-1R/8:7R/16:17/19:17", 16));

    mpSearchTree->clear();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    // additional tests for constructors along with the == and != operators
    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 2, 4, 0, 1}, scDefaultValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, -3, 2, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree); // in this particular case due to AVL tree construction rules the trees become equal when third element is being added (unlike the basic BST)

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 0, 1}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, 2, -3, 4, 1, 0}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-3, -5, 2, 4, 0, 1}, scDefaultValue, scDefaultNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-3, 2, -5, 4, 0, 1}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{}, scDefaultValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    delete mpSearchTree;
    mpSearchTree = new AVLTree{scCustomNullValue};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
}

void AVLTreesTests::testRemoveNodes()
{
    bool nodeDeleted{false};

    mpSearchTree = new AVLTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue};
    mpAuxSearchTree = new AVLTree{*mpSearchTree};

    nodeDeleted = mpSearchTree->removeNode(-16);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-5:-9/2:7/14:7/-23:-15/-12:-15/-2:-5R/0:2L/8:14/17:14/16:17/19:17", 15));

    nodeDeleted = mpSearchTree->removeNode(8);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-5:-9/2:7/16:7/-23:-15/-12:-15/-2:-5R/0:2L/14:16/17:16/19:17R", 14));

    nodeDeleted = mpSearchTree->removeNode(0);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/16:-1/-15:-9/-5:-9/7:16/17:16/-23:-15/-12:-15/-2:-5R/2:7/14:7/19:17R", 13));

    nodeDeleted = mpSearchTree->removeNode(-23);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/16:-1/-15:-9/-5:-9/7:16/17:16/-12:-15R/-2:-5R/2:7/14:7/19:17R", 12));

    nodeDeleted = mpSearchTree->removeNode(19);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/16:-1/-15:-9/-5:-9/7:16/17:16/-12:-15R/-2:-5R/2:7/14:7", 11));

    nodeDeleted = mpSearchTree->removeNode(17);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-5:-9/2:7/16:7/-12:-15R/-2:-5R/14:16L", 10));

    nodeDeleted = mpSearchTree->removeNode(-5);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/7:-1/-15:-9/-2:-9/2:7/16:7/-12:-15R/14:16L", 9));

    nodeDeleted = mpSearchTree->removeNode(-9);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-15:-12/-2:-12/2:7/16:7/14:16L", 8));

    nodeDeleted = mpSearchTree->removeNode(-15);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-2:-12R/2:7/16:7/14:16L", 7));

    nodeDeleted = mpSearchTree->removeNode(14);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-2:-12R/2:7/16:7", 6));

    nodeDeleted = mpSearchTree->removeNode(-1);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-12:2/7:2/-2:-12R/16:7R", 5));

    nodeDeleted = mpSearchTree->removeNode(-2);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-12:2/7:2/16:7R", 4));

    nodeDeleted = mpSearchTree->removeNode(-12);
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "7:ROOT/2:7/16:7", 3));

    nodeDeleted = mpSearchTree->removeNode(16); // root and two children, erase right child
    QVERIFY(nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "7:ROOT/2:7L", 2));

    nodeDeleted = mpSearchTree->removeNode(2); // root and left child, erase left child
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

    // we will stop checking the nodeDeleted variable (obvious enough)
    (void)mpAuxSearchTree->removeNode(-15);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5R/0:2L/8:14/17:14/16:17/19:17", 15));

    (void)mpAuxSearchTree->removeNode(19);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16/-12:-16/-2:-5R/0:2L/8:14/17:14/16:17L", 14));

    (void)mpAuxSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-16:-9/-5:-9/2:7/14:7/-23:-16L/-2:-5R/0:2L/8:14/17:14/16:17L", 13));

    (void)mpAuxSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-23:-9/-5:-9/2:7/14:7/-2:-5R/0:2L/8:14/17:14/16:17L", 12));

    (void)mpAuxSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-9:-1/7:-1/-23:-9/-5:-9/2:7/14:7/-2:-5R/0:2L/8:14/16:14", 11));

    (void)mpAuxSearchTree->removeNode(-23);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/14:7/0:2L/8:14/16:14", 10));

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/16:7/0:2L/8:16L", 9));

    (void)mpAuxSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-9:-5/-2:-5/2:7/8:7/0:2L", 8));

    (void)mpAuxSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5R/2:7/8:7/0:2L", 7));

    (void)mpAuxSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/7:-1/-2:-5R/2:7/8:7", 6));

    (void)mpAuxSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/8:-1/-2:-5R/2:8L", 5));

    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/8:-1/2:8L", 4));

    (void)mpAuxSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT/-5:-1/8:-1", 3));

    (void)mpAuxSearchTree->removeNode(-1); // root and two children, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:ROOT/-5:8L", 2));

    (void)mpAuxSearchTree->removeNode(8); // root and left child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:ROOT", 1));

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-16:-12/-9:-12/2:7/14:7/-23:-16/-15:-16/-2:-9R/0:2L/8:14/17:14/16:17/19:17", 15));

    (void)mpSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/7:-1/-16:-12/-9:-12/2:7/16:7/-23:-16/-15:-16/-2:-9R/0:2L/14:16/17:16/19:17R", 14));

    (void)mpSearchTree->removeNode(-1);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/7:16/17:16/-23:-16/-15:-16/-2:-9R/2:7/14:7/19:17R", 13));

    (void)mpSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/7:16/17:16/-23:-16/-15:-16/-2:-9R/14:7R/19:17R", 12));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/7:16/17:16/-23:-16/-15:-16/14:7R/19:17R", 11));

    (void)mpSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:ROOT/-12:0/16:0/-16:-12/-9:-12/14:16/17:16/-23:-16/-15:-16/19:17R", 10));

    (void)mpSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "14:ROOT/-12:14/17:14/-16:-12/-9:-12/16:17/19:17/-23:-16/-15:-16", 9));

    (void)mpSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "14:ROOT/-16:14/17:14/-23:-16/-12:-16/16:17/19:17/-15:-12L", 8));

    (void)mpSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "14:ROOT/-16:14/17:14/-23:-16/-12:-16/19:17R/-15:-12L", 7));

    (void)mpSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-16:-12/17:-12/-23:-16/-15:-16/19:17R", 6));

    (void)mpSearchTree->removeNode(-23);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-16:-12/17:-12/-15:-16R/19:17R", 5));

    (void)mpSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-16:-12/19:-12/-15:-16R", 4));

    (void)mpSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT/-15:-12/19:-12", 3));

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    (void)mpSearchTree->removeNode(-23);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/8:-1/-16:-12/-5:-12/2:8/16:8/-15:-16R/-9:-5/-2:-5/0:2/7:2/14:16/17:16/19:17R", 15));

    (void)mpSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-12:-1/8:-1/-15:-12/-5:-12/2:8/16:8/-9:-5/-2:-5/0:2/7:2/14:16/17:16/19:17R", 14));

    (void)mpSearchTree->removeNode(-15);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-9:-1/8:-1/-12:-9/-5:-9/2:8/16:8/-2:-5R/0:2/7:2/14:16/17:16/19:17R", 13));

    (void)mpSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/-1:8/16:8/-5:-1/2:-1/14:16/17:16/-9:-5/-2:-5/0:2/7:2/19:17R", 12));

    (void)mpSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/-1:8/16:8/-5:-1/2:-1/14:16/17:16/-2:-5R/0:2/7:2/19:17R", 11));

    (void)mpSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/-1:8/16:8/-2:-1/2:-1/14:16/17:16/0:2/7:2/19:17R", 10));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/0:8/16:8/-1:0/2:0/14:16/17:16/7:2R/19:17R", 9));

    (void)mpSearchTree->removeNode(-1);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/2:8/16:8/0:2/7:2/14:16/17:16/19:17R", 8));

    (void)mpSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "8:ROOT/2:8/16:8/7:2R/14:16/17:16/19:17R", 7));

    (void)mpSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "16:ROOT/8:16/17:16/7:8/14:8/19:17R", 6));

    (void)mpSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "16:ROOT/8:16/17:16/14:8R/19:17R", 5));

    (void)mpSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "16:ROOT/14:16/17:16/19:17R", 4));

    (void)mpSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "17:ROOT/16:17/19:17", 3));

    (void)mpSearchTree->removeNode(16); // root and two children, erase left child
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "17:ROOT/19:17R", 2));

    (void)mpSearchTree->removeNode(17); // root and right child, erase root
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "19:ROOT", 1));

    mpAuxSearchTree = new AVLTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue, scCustomNullValue};

    (void)mpAuxSearchTree->removeNode(19);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT/-9:0/14:0/-15:-9/-2:-9/7:14/17:14/-16:-15/-12:-15/-5:-2/-1:-2/2:7/8:7/16:17L/-23:-16L", 15));

    (void)mpAuxSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT/-9:0/14:0/-15:-9/-2:-9/7:14/16:14/-16:-15/-12:-15/-5:-2/-1:-2/2:7/8:7/-23:-16L", 14));

    (void)mpAuxSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "0:ROOT/-9:0/7:0/-15:-9/-2:-9/2:7/14:7/-16:-15/-12:-15/-5:-2/-1:-2/8:14L/-23:-16L", 13));

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/0:-9/-16:-15/-12:-15/-2:0/7:0/-23:-16L/-5:-2/-1:-2/2:7/8:7", 12));

    (void)mpAuxSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/0:-9/-16:-15/-12:-15/-2:0/7:0/-23:-16L/-5:-2/-1:-2/2:7L", 11));

    (void)mpAuxSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/0:-9/-16:-15/-12:-15/-2:0/2:0/-23:-16L/-5:-2/-1:-2", 10));

    (void)mpAuxSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/-2:-9/-16:-15/-12:-15/-5:-2/0:-2/-23:-16L/-1:0L", 9));

    (void)mpAuxSearchTree->removeNode(0);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/-2:-9/-16:-15/-12:-15/-5:-2/-1:-2/-23:-16L", 8));

    (void)mpAuxSearchTree->removeNode(-1);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-9:ROOT/-15:-9/-2:-9/-16:-15/-12:-15/-5:-2L/-23:-16L", 7));

    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT/-16:-15/-9:-15/-23:-16L/-12:-9/-5:-9", 6));

    (void)mpAuxSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT/-16:-15/-9:-15/-23:-16L/-12:-9L", 5));

    (void)mpAuxSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-15:ROOT/-16:-15/-12:-15/-23:-16L", 4));

    (void)mpAuxSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-16:ROOT/-23:-16/-15:-16", 3));

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-23, 19, -16, 17, -15, 16, -12, 14, -9, 8, -5, 7, -2, 2, -1, 0}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->removeNode(14);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-9:ROOT/-15:-9/2:-9/-16:-15/-12:-15/-2:2/16:2/-23:-16L/-5:-2/-1:-2/7:16/17:16/0:-1R/8:7R/19:17R", 15));

    (void)mpSearchTree->removeNode(-9);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-15:-5/2:-5/-16:-15/-12:-15/-1:2/16:2/-23:-16L/-2:-1/0:-1/7:16/17:16/8:7R/19:17R", 14));

    (void)mpSearchTree->removeNode(8);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT/-15:-5/2:-5/-16:-15/-12:-15/-1:2/16:2/-23:-16L/-2:-1/0:-1/7:16/17:16/19:17R", 13));

    (void)mpSearchTree->removeNode(-12);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-5:2/16:2/-16:-5/-1:-5/7:16/17:16/-23:-16/-15:-16/-2:-1/0:-1/19:17R", 12));

    (void)mpSearchTree->removeNode(16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-5:2/17:2/-16:-5/-1:-5/7:17/19:17/-23:-16/-15:-16/-2:-1/0:-1", 11));

    (void)mpSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-2:2/17:2/-16:-2/-1:-2/7:17/19:17/-23:-16/-15:-16/0:-1R", 10));

    (void)mpSearchTree->removeNode(7);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-2:2/17:2/-16:-2/-1:-2/19:17R/-23:-16/-15:-16/0:-1R", 9));

    (void)mpSearchTree->removeNode(-15);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-2:2/17:2/-16:-2/-1:-2/19:17R/-23:-16L/0:-1R", 8));

    (void)mpSearchTree->removeNode(17);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-2:ROOT/-16:-2/2:-2/-23:-16L/-1:2/19:2/0:-1R", 7));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-16:-1/2:-1/-23:-16L/0:2/19:2", 6));

    (void)mpSearchTree->removeNode(2);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-16:-1/19:-1/-23:-16L/0:19L", 5));

    (void)mpSearchTree->removeNode(-16);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-23:-1/19:-1/0:19L", 4));

    (void)mpSearchTree->removeNode(19);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT/-23:-1/0:-1", 3));

    mpAuxSearchTree = new AVLTree{std::vector<int>{-3, -5, 2}, scDefaultValue};

    (void)mpAuxSearchTree->removeNode(-5);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-3:ROOT/2:-3R", 2));

    (void)mpAuxSearchTree->removeNode(2); // root and right child, erase right child
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-3:ROOT", 1));

    _resetTreeObjects();

    // deleting null node from custom null value tree
    mpSearchTree = new AVLTree{std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue, scCustomNullValue};

    nodeDeleted = mpSearchTree->removeNode(-5);
    QVERIFY(!nodeDeleted &&
            areExpectedTreeValuesMet(mpSearchTree, "2:ROOT/-1:2/3:2/-2:-1L/4:3R", 5));

    // deleting same node from custom and default null value trees of equal structure, keys and values
    mpAuxSearchTree = new AVLTree{std::vector<int>{-1, 3, 2, 4, -2}, scDefaultValue};

    nodeDeleted = mpSearchTree->removeNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree != *mpAuxSearchTree);

    nodeDeleted = mpAuxSearchTree->removeNode(3);
    QVERIFY(nodeDeleted &&
            *mpSearchTree == *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT/-1:2/4:2/-2:-1L", 4));
}

void AVLTreesTests::testUpdateNodeValue()
{
    mpSearchTree = new AVLTree;

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-9:h8:-1/7:f6:-1/-16:m13:-9/-5:a1:-9/2:d4:7/14:j10:7/-23:k11:-16/-12:n14:-16/-2:e5:-5R/0:g7:2L/8:b2:14/16:i9:14/17:l12:16R", 14, true));

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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-9:b2:-1/8:b2:-1/-16:m13:-9/-5:a1:-9/2:d4_1:8/17:L12:8/-23:k11:-16/-12:n14:-16/-2:e5:-5R/0:g7:2L/14:j10:17/25:o15:17/16:i9_new:14R", 14, true));

    mpSearchTree->clear();

    QVERIFY(scDefaultNullValue == mpSearchTree->getNodeValue(-5) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(0) &&
            scDefaultNullValue == mpSearchTree->getNodeValue(16));

    // test with same value for all nodes
    mpAuxSearchTree = new AVLTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:DF:ROOT/-9:DF:-1/7:DF:-1/-16:DF:-9/-5:DF:-9/2:DF:7/14:DF:7/-23:DF:-16/-12:DF:-16/-2:DF:-5R/0:DF:2L/8:DF:14/16:DF:14/17:DF:16R", 14, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY(scDefaultValue == mpAuxSearchTree->getNodeValue(-5) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(0) &&
            scDefaultValue == mpAuxSearchTree->getNodeValue(16));

    // test updating custom null value tree node values
    _resetTreeObjects();

    mpSearchTree = new AVLTree{scCustomNullValue};

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-8, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(0, "g7");
    (void)mpSearchTree->addOrUpdateNode(-9, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1::ROOT/-5:a1:-1/7:f6:-1/-8::-5/-2:e5:-5/2:d4:7/8:b2:7/-9:h8:-8L/0:g7:2L", 9, true));
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

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:g7:ROOT/-5:a1:0/7:f6:0/-7:i9:-5/-2::-5/8:b2:7R/-9:h8:-7L/-1::-2R", 8, true));

    mpAuxSearchTree = new AVLTree{*mpSearchTree};

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(17, "df");
    QVERIFY(newNodeAdded &&
            *mpSearchTree != *mpAuxSearchTree);

    // test value update and copy assignment between default and custom null value trees
    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue};

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    QVERIFY(!newNodeAdded &&
            scDefaultNullValue == mpSearchTree->getNodeValue(7));

    newNodeAdded = mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);
    QVERIFY(!newNodeAdded &&
            scCustomNullValue == mpAuxSearchTree->getNodeValue(4));

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "14:DF:ROOT/-9:DF:14/16:DF:14/-16:DF:-9/7::-9/17:DF:16R/-23:DF:-16/-12:DF:-16", 8, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void AVLTreesTests::testMoveSemantics()
{
    mpSearchTree = new AVLTree;

    (void)mpSearchTree->addOrUpdateNode(-5, "a1");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");

    mpAuxSearchTree = new AVLTree{std::move(*mpSearchTree)};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-5:a1:-1/8:b2:-1/-2:e5:-5R/2:d4:8L", 5, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree->addOrUpdateNode(17, "f6");
    mpSearchTree->addOrUpdateNode(0, "g7");
    mpSearchTree->addOrUpdateNode(-2, "e5_1");
    mpSearchTree->addOrUpdateNode(3, "h8");

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:g7:ROOT/-2:e5_1:0/17:f6:0/3:h8:17L", 4, true));

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "0:g7:ROOT/-2:e5_1:0/17:f6:0/3:h8:17L", 4, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "0:g7:ROOT/-2:e5_1:0/17:f6:0/3:h8:17L", 4, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // test move constructor for trees with custom null value
    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue, scCustomNullValue};

    (void)mpSearchTree->addOrUpdateNode(5, scDefaultNullValue);
    (void)mpSearchTree->addOrUpdateNode(4, "newval");

    mpAuxSearchTree = new AVLTree{std::move(*mpSearchTree)};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "4:newval:ROOT/-1:DF:4/5::4/-2:DF:-1/0:DF:-1", 5, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());

    // test move and copy for trees with different null values
    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{-2, 5, 4, 0, -1}, scDefaultValue};

    (void)mpSearchTree->addOrUpdateNode(7, scDefaultNullValue);
    (void)mpAuxSearchTree->addOrUpdateNode(4, scCustomNullValue);

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "4:NULL:ROOT/-1:DF:4/5:DF:4/-2:DF:-1/0:DF:-1", 5, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void AVLTreesTests::testMergeTrees()
{
    mpSearchTree = new AVLTree;

    (void)mpSearchTree->addOrUpdateNode(-5, "a1_1");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-23, "k11");
    (void)mpSearchTree->addOrUpdateNode(17, "l12");
    (void)mpSearchTree->addOrUpdateNode(-12, "n14");
    (void)mpSearchTree->addOrUpdateNode(16, "i9_1");
    (void)mpSearchTree->addOrUpdateNode(0, "g7_1");

    mpAuxSearchTree = new AVLTree;

    (void)mpAuxSearchTree->addOrUpdateNode(8, "b2");
    (void)mpAuxSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpAuxSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpAuxSearchTree->addOrUpdateNode(0, "g7_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-5, "a1_2");
    (void)mpAuxSearchTree->addOrUpdateNode(16, "i9_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-9, "h8");
    (void)mpAuxSearchTree->addOrUpdateNode(14, "j10");
    (void)mpAuxSearchTree->addOrUpdateNode(-16, "m13");

    const AVLTree searchTreeCopy{*mpSearchTree};
    const AVLTree searchTreeAuxCopy{*mpAuxSearchTree};

    QVERIFY(searchTreeCopy == *mpSearchTree &&      // just a(n additional) check that the copy constructor and == operator work correctly
            searchTreeAuxCopy == *mpAuxSearchTree);

    // first (main) merge
    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-5:a1_2:2/16:i9_2:2/-12:n14:-5/-1:c3:-5/8:b2:16/17:l12:16/-23:k11:-12/-9:h8:-12/-2:e5:-1/0:g7_2:-1/7:f6:8/14:j10:8/-16:m13:-23R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    const AVLTree mainTreeAfterFirstMerge{*mpSearchTree};

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
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-12:n14:-1/8:b2:-1/-16:m13:-12/-5:a1_1:-12/2:d4:8/16:i9_1:8/-23:k11:-16L/-9:h8:-5/-2:e5:-5/0:g7_1:2/7:f6:2/14:j10:16/17:l12:16", 14, true));

    QVERIFY(*mpAuxSearchTree != mainTreeAfterFirstMerge); // test the != operator too

    // actually not necesarily required to test this as the two trees SHOULD have same null value in order to be merged (assert statement enforces this)
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // merge trees with (same) custom null value
    _resetTreeObjects();

    mpSearchTree = new AVLTree{scCustomNullValue};

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

    mpAuxSearchTree = new AVLTree{std::vector<int>{16, -9, 14, 7, -23, 17, -16, -12}, scDefaultValue, scCustomNullValue};
    (void)mpAuxSearchTree->addOrUpdateNode(-9, scDefaultNullValue);

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-9::-1/14:DF:-1/-16:DF:-9/-5:a1:-9/7:DF:14/16:DF:14/-23:DF:-16/-12:DF:-16/-7:i9:-5/-2::-5/0:g7:7/8:b2:7/17:DF:16R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scCustomNullValue == mpAuxSearchTree->getNullValue());
}

void AVLTreesTests::_resetTreeObjects()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

QTEST_APPLESS_MAIN(AVLTreesTests)

#include "tst_avltreestests.moc"
