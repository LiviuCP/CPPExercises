#include <QtTest>

#include "testutils.h"
#include "binarysearchtree.h"
#include "redblacktree.h"
#include "avltree.h"

using namespace TestUtils;

class MixedTreeTypesTests : public QObject
{
    Q_OBJECT

public:
    MixedTreeTypesTests();
    ~MixedTreeTypesTests();

private slots:
    void init();
    void cleanup();

    void testMergeDifferentSearchTrees();
    void testDifferentTreeTypesEquivalence();
    void testCopyAssignmentOfMixedTreeTypes();
    void testMoveAssignmentOfMixedTreeTypes();
    void testPassThroughAllLogMessages();

private:
    void _buildPrimaryTestSearchTree(); // create content for the primary test tree (referenced by mpSearchTree)
    void _buildSecondaryTestSearchTree(); // create content for the secondary test tree (referenced by mpAuxSearchTree)
    void _buildPrimaryTestSearchTreeInDifferentOrder(); // same content, different node adding order to the tree created by _buildPrimaryTestSearchTree() method (referenced by mpAuxSearchTree)
    void _buildSecondaryTestSearchTreeInDifferentOrder(); // same content, different node adding order to the tree created by _buildSecondaryTestSearchTree() method (referenced by mpSearchTree)

    void _resetTreeObjects();

    BinarySearchTree* mpSearchTree;
    BinarySearchTree* mpAuxSearchTree;
};

MixedTreeTypesTests::MixedTreeTypesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
    BinarySearchTree::enableLogging(false);
}

MixedTreeTypesTests::~MixedTreeTypesTests()
{
    // not implemented as cleanup() does all the job
}

void MixedTreeTypesTests::init()
{
    QVERIFY(nullptr == mpSearchTree && nullptr == mpAuxSearchTree);
}

void MixedTreeTypesTests::cleanup()
{
    _resetTreeObjects();
    BinarySearchTree::enableLogging(false);
}

void MixedTreeTypesTests::testMergeDifferentSearchTrees()
{
    /* create all trees to be used in the merge operations */
    mpSearchTree = new BinarySearchTree;
    mpAuxSearchTree = new BinarySearchTree;

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    const BinarySearchTree simpleTreeCopy{*mpSearchTree};
    const BinarySearchTree simpleTreeAuxCopy{*mpAuxSearchTree};

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree;
    mpAuxSearchTree = new RedBlackTree;

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    const RedBlackTree redBlackTreeCopy{*dynamic_cast<RedBlackTree*>(mpSearchTree)};
    const RedBlackTree redBlackTreeAuxCopy{*dynamic_cast<RedBlackTree*>(mpAuxSearchTree)};

    _resetTreeObjects();

    mpSearchTree = new AVLTree;
    mpAuxSearchTree = new AVLTree;

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    const AVLTree avlTreeCopy{*dynamic_cast<AVLTree*>(mpSearchTree)};
    const AVLTree avlTreeAuxCopy{*dynamic_cast<AVLTree*>(mpAuxSearchTree)};

    /* merge RED-BLACK tree into SIMPLE tree */
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14, true));

    /* merge AVL tree into SIMPLE tree */
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _resetTreeObjects();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14, true));

    /* merge AVL tree into RED-BLACK tree */
    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:BK/16:i9_2:2:BK/-23:k11:-12:BK/-1:c3:-12:RD/8:b2:16:BK/17:l12:16:BK/-16:m13:-23R:RD/-5:a1_2:-1:BK/0:g7_2:-1:BK/7:f6:8:RD/14:j10:8:RD/-9:h8:-5:RD/-2:e5:-5:RD", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _resetTreeObjects();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-5:a1_1:-1:BK/8:b2:-1:BK/-12:n14:-5:RD/-2:e5:-5:BK/2:d4:8:BK/16:i9_1:8:BK/-16:m13:-12:BK/-9:h8:-12:BK/0:g7_1:2:RD/7:f6:2:RD/14:j10:16:RD/17:l12:16:RD/-23:k11:-16L:RD", 14, true));

    /* merge SIMPLE tree into RED-BLACK tree */
    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:BK/16:i9_2:2:BK/-23:k11:-12:BK/-1:c3:-12:RD/8:b2:16:BK/17:l12:16:BK/-16:m13:-23R:RD/-5:a1_2:-1:BK/0:g7_2:-1:BK/7:f6:8:RD/14:j10:8:RD/-9:h8:-5:RD/-2:e5:-5:RD", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-5:a1_1:-1:BK/8:b2:-1:BK/-16:m13:-5:RD/-2:e5:-5:BK/2:d4:8:BK/16:i9_1:8:BK/-23:k11:-16:BK/-9:h8:-16:BK/0:g7_1:2:RD/7:f6:2:RD/14:j10:16:RD/17:l12:16:RD/-12:n14:-9L:RD", 14, true));

    /* merge SIMPLE tree into AVL tree */
    _resetTreeObjects();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-5:a1_2:2/16:i9_2:2/-12:n14:-5/-1:c3:-5/8:b2:16/17:l12:16/-23:k11:-12/-9:h8:-12/-2:e5:-1/0:g7_2:-1/7:f6:8/14:j10:8/-16:m13:-23R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-9:h8:-1/8:b2:-1/-16:m13:-9/-5:a1_1:-9/2:d4:8/16:i9_1:8/-23:k11:-16/-12:n14:-16/-2:e5:-5R/0:g7_1:2/7:f6:2/14:j10:16/17:l12:16", 14, true));

    /* merge RED-BLACK tree into AVL tree */
    _resetTreeObjects();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-5:a1_2:2/16:i9_2:2/-12:n14:-5/-1:c3:-5/8:b2:16/17:l12:16/-23:k11:-12/-9:h8:-12/-2:e5:-1/0:g7_2:-1/7:f6:8/14:j10:8/-16:m13:-23R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-12:n14:-1/8:b2:-1/-16:m13:-12/-5:a1_1:-12/2:d4:8/16:i9_1:8/-23:k11:-16L/-9:h8:-5/-2:e5:-5/0:g7_1:2/7:f6:2/14:j10:16/17:l12:16", 14, true));
}

void MixedTreeTypesTests::testDifferentTreeTypesEquivalence()
{
    // Simple BST vs. Red-Black tree
    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildPrimaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    (void)mpSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    (void)mpAuxSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*dynamic_cast<RedBlackTree*>(mpSearchTree) == *mpAuxSearchTree);

    (void)mpSearchTree->addOrUpdateNode(1, "abcd");
    (void)mpAuxSearchTree->addOrUpdateNode(1, "abcd");
    QVERIFY(*dynamic_cast<RedBlackTree*>(mpSearchTree) != *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14_1:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/0:g7_1:-12:BK/7:f6:16:RD/17:l12:16:RD/-5:a1_1:0:RD/1:abcd:0:RD", 9, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R/1:abcd:0R", 9, true));

    _resetTreeObjects();

    mpAuxSearchTree = new RedBlackTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    mpSearchTree = new BinarySearchTree{};
    _buildSecondaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    (void)mpSearchTree->removeNode(14);
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(*mpSearchTree == *dynamic_cast<RedBlackTree*>(mpAuxSearchTree));

    (void)mpSearchTree->removeNode(-2);
    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(*mpSearchTree != *dynamic_cast<RedBlackTree*>(mpAuxSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5L/0:g7_2:8/16:i9_2:8/-16:m13:-9L", 7, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-9:h8:-1:RD/8:b2:-1:RD/-16:m13:-9:BK/-5:a1_2:-9:BK/0:g7_2:8:BK/16:i9_2:8:BK", 7, true));

    // Red-Black tree vs. AVL tree
    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new RedBlackTree{std::move(*dynamic_cast<RedBlackTree*>(mpSearchTree))};

    delete mpSearchTree;
    mpSearchTree = new AVLTree;
    _buildPrimaryTestSearchTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8, true));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(*mpSearchTree == *dynamic_cast<RedBlackTree*>(mpAuxSearchTree));

    (void)mpSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);
    QVERIFY(*mpSearchTree != *dynamic_cast<RedBlackTree*>(mpAuxSearchTree));

    (void)mpAuxSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*dynamic_cast<AVLTree*>(mpSearchTree) == *dynamic_cast<RedBlackTree*>(mpAuxSearchTree));
    QVERIFY(*dynamic_cast<AVLTree*>(mpSearchTree) == *mpAuxSearchTree);

    (void)mpAuxSearchTree->addOrUpdateNode(25, "abcd");
    QVERIFY(*dynamic_cast<AVLTree*>(mpSearchTree) != *dynamic_cast<RedBlackTree*>(mpAuxSearchTree));
    QVERIFY(*dynamic_cast<AVLTree*>(mpSearchTree) != *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT:BK/-12:n14_1:2:RD/16:i9_1:2:RD/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:BK/17:l12:16:BK/0:g7_1:-5R:RD/25:abcd:17R:RD", 9, true));

    _resetTreeObjects();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    mpSearchTree = new AVLTree{*dynamic_cast<AVLTree*>(mpAuxSearchTree)};

    delete mpAuxSearchTree;
    mpAuxSearchTree = new RedBlackTree;
    _buildSecondaryTestSearchTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);
    QVERIFY(*mpAuxSearchTree == *dynamic_cast<AVLTree*>(mpSearchTree));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);
    QVERIFY(*mpAuxSearchTree != *dynamic_cast<AVLTree*>(mpSearchTree));

    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(*dynamic_cast<RedBlackTree*>(mpAuxSearchTree) == *dynamic_cast<AVLTree*>(mpSearchTree));
    QVERIFY(*dynamic_cast<RedBlackTree*>(mpAuxSearchTree) == *mpSearchTree);

    (void)mpSearchTree->removeNode(14);
    QVERIFY(*dynamic_cast<RedBlackTree*>(mpAuxSearchTree) != *dynamic_cast<AVLTree*>(mpSearchTree));
    QVERIFY(*dynamic_cast<RedBlackTree*>(mpAuxSearchTree) != *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-9:h8:-1/8:b2:-1/-16:m13:-9/-5:a1_2:-9/0:g7_2:8/16:i9_2:8", 7, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-9:h8:-1:RD/8:b2:-1:RD/-16:m13:-9:BK/-5:a1_2:-9:BK/0:g7_2:8:BK/16:i9_2:8:BK/14:j10:16L:RD", 8, true));

    // AVL tree vs. Simple BST
    _resetTreeObjects();

    mpSearchTree = new AVLTree{};
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new BinarySearchTree{};
    _buildPrimaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    (void)mpSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);

    (void)mpAuxSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpAuxSearchTree == *dynamic_cast<AVLTree*>(mpSearchTree));

    (void)mpSearchTree->addOrUpdateNode(1, "abcd");
    (void)mpAuxSearchTree->addOrUpdateNode(1, "abcd");
    QVERIFY(*mpAuxSearchTree != *dynamic_cast<AVLTree*>(mpSearchTree));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/0:g7_1:-12/7:f6:16/17:l12:16/-5:a1_1:0/1:abcd:0", 9, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R/1:abcd:0R", 9, true));

    _resetTreeObjects();

    mpAuxSearchTree = new AVLTree{};
    _buildSecondaryTestSearchTree();

    mpSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    (void)mpSearchTree->removeNode(14);
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(*dynamic_cast<AVLTree*>(mpAuxSearchTree) == *mpSearchTree);

    (void)mpSearchTree->removeNode(-2);
    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(*dynamic_cast<AVLTree*>(mpAuxSearchTree) != *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5L/0:g7_2:8/16:i9_2:8/-16:m13:-9L", 7, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-9:h8:-1/8:b2:-1/-16:m13:-9/-5:a1_2:-9/0:g7_2:8/16:i9_2:8", 7, true));

    // additional tests
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -1, 3}, scDefaultValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{1, -1, 3}, scDefaultValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{std::vector<int>{2, -1, 3, 4}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{2, -1, 3, -5}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{2, -1, 3, -2}, scDefaultValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{2, -1, 3, 8}, scDefaultValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new AVLTree{std::vector<int>{2, -1, 3, 4}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{2, -1, 3, 5}, scDefaultValue, scCustomNullValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{std::vector<int>{2, -1, 3, -4}, scDefaultValue};
    mpAuxSearchTree = new BinarySearchTree{std::vector<int>{2, -1, 3, -5}, scDefaultValue};

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -1, 3}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{2, -1, 3}, scDefaultValue, scCustomNullValue};
    mpSearchTree->addOrUpdateNode(2, "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree{std::vector<int>{2, -1, 3, -4}, scDefaultValue};
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{2, -1, 3, -4}, scDefaultValue};
    mpAuxSearchTree->addOrUpdateNode(-4, "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{std::vector<int>{2, -1, 3, 5}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{2, -1, 3, 5}, scDefaultValue, scCustomNullValue};
    mpAuxSearchTree->addOrUpdateNode(5, "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);
}

void MixedTreeTypesTests::testCopyAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));

    /* simulate how the destination tree would be built by adding the keys of the source tree in (top-down/left-right) order
       (no need to take the values into consideration so default values are being used for simplicity)
    */
    QVERIFY("-1:ROOT:BK/-5:-1:RD/8:-1:RD/-9:-5:BK/-2:-5:BK/0:8:BK/16:8:BK/-16:-9L:RD/14:16L:RD" ==
            RedBlackTree(std::vector<int>{-1, -5, 8, -9, -2, 0, 16, -16, 14}, scDefaultValue).getTreeAsString(false));

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("2:ROOT/-12:2/16:2/-23:-12/-5:-12/7:16/17:16/0:-5R" ==
            AVLTree(std::vector<int>{2, -12, 16, -23, -5, 7, 17, 0}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    // Simple BST assigned to Red-Black tree (and vice-versa)
    _resetTreeObjects();

    mpSearchTree = new RedBlackTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_2:8/-2:e5:-1/0:g7_2:-1/14:j10:16L/-5:a1_2:-2L/-9:h8:-5L/-16:m13:-9L", 9, true));

    QVERIFY("-1:ROOT:BK/-5:-1:RD/8:-1:RD/-9:-5:BK/-2:-5:BK/0:8:BK/16:8:BK/-16:-9L:RD/14:16L:RD" ==
            RedBlackTree(std::vector<int>{8, -1, 16, -2, 0, 14, -5, -9, -16}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("2:ROOT/-12:2/16:2/-23:-12/-5:-12/7:16/17:16/0:-5R" ==
            BinarySearchTree(std::vector<int>{2, -12, 16, -23, -5, 7, 17, 0}, scDefaultValue).getTreeAsString(false));

    // AVL tree assigned to Simple BST (and vice-versa)
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));

    QVERIFY("-1:ROOT/-5:-1/8:-1/-9:-5/-2:-5/0:8/16:8/-16:-9L/14:16L" ==
            BinarySearchTree(std::vector<int>{-1, -5, 8, -9, -2, 0, 16, -16, 14}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/7:f6:2/17:l12:7R/16:i9_1:17L", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/16:i9_1:2/7:f6:16/17:l12:16", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("-5:ROOT/-23:-5/2:-5/-12:-23R/0:2/16:2/7:16/17:16" ==
            AVLTree(std::vector<int>{-5, -23, 2, -12, 0, 7, 17, 16}, scDefaultValue).getTreeAsString(false));
}

/* Same test base used as for the copy assignment test
   The destination tree should be the same as when copy assigning while the source tree should become empty
   No in-order simulation by using default values (see copy assignment test) is required any longer
*/
void MixedTreeTypesTests::testMoveAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    // Simple BST assigned to Red-Black tree (and vice-versa)
    _resetTreeObjects();

    mpSearchTree = new RedBlackTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // AVL tree assigned to Simple BST (and vice-versa)
    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    _resetTreeObjects();

    mpSearchTree = new BinarySearchTree;
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/16:i9_1:2/7:f6:16/17:l12:16", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void MixedTreeTypesTests::testPassThroughAllLogMessages()
{
    BinarySearchTree::enableLogging(true);

    mpSearchTree = new BinarySearchTree;
    mpAuxSearchTree = new RedBlackTree;

    mpSearchTree->printTree();
    mpAuxSearchTree->printTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _resetTreeObjects();

    mpSearchTree = new RedBlackTree{std::vector<int>{2, 3, 2, -4}, scDefaultValue};
    mpAuxSearchTree = new AVLTree{std::vector<int>{2, 5, 2, 2}, scDefaultValue};

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:ROOT:BK/-4:2:RD/3:2:RD", 3));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:ROOT/5:2R", 2));

    mpAuxSearchTree->addOrUpdateNode(2, "abcd");

    qInfo("Printing red-black tree before merge");
    mpSearchTree->printTree();
    qInfo("Printing AVL tree before merge");
    mpAuxSearchTree->printTree();

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:abcd:ROOT:BK/-4:DF:2:BK/3:DF:2:BK/5:DF:3R:RD", 4, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, false));

    qInfo("Printing red-black tree after merging AVL tree into it");
    mpSearchTree->printTree();
}

void MixedTreeTypesTests::_buildPrimaryTestSearchTree()
{
    (void)mpSearchTree->addOrUpdateNode(-5, "a1_1");
    (void)mpSearchTree->addOrUpdateNode(2, "d4");
    (void)mpSearchTree->addOrUpdateNode(7, "f6");
    (void)mpSearchTree->addOrUpdateNode(-23, "k11");
    (void)mpSearchTree->addOrUpdateNode(17, "l12");
    (void)mpSearchTree->addOrUpdateNode(-12, "n14");
    (void)mpSearchTree->addOrUpdateNode(16, "i9_1");
    (void)mpSearchTree->addOrUpdateNode(0, "g7_1");
}

void MixedTreeTypesTests::_buildSecondaryTestSearchTree()
{
    (void)mpAuxSearchTree->addOrUpdateNode(8, "b2");
    (void)mpAuxSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpAuxSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpAuxSearchTree->addOrUpdateNode(0, "g7_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-5, "a1_2");
    (void)mpAuxSearchTree->addOrUpdateNode(16, "i9_2");
    (void)mpAuxSearchTree->addOrUpdateNode(-9, "h8");
    (void)mpAuxSearchTree->addOrUpdateNode(14, "j10");
    (void)mpAuxSearchTree->addOrUpdateNode(-16, "m13");
}

void MixedTreeTypesTests::_buildPrimaryTestSearchTreeInDifferentOrder()
{
    (void)mpAuxSearchTree->addOrUpdateNode(2, "d4");
    (void)mpAuxSearchTree->addOrUpdateNode(-12, "n14");
    (void)mpAuxSearchTree->addOrUpdateNode(16, "i9_1");
    (void)mpAuxSearchTree->addOrUpdateNode(-23, "k11");
    (void)mpAuxSearchTree->addOrUpdateNode(-5, "a1_1");
    (void)mpAuxSearchTree->addOrUpdateNode(7, "f6");
    (void)mpAuxSearchTree->addOrUpdateNode(17, "l12");
    (void)mpAuxSearchTree->addOrUpdateNode(0, "g7_1");
}

void MixedTreeTypesTests::_buildSecondaryTestSearchTreeInDifferentOrder()
{
    (void)mpSearchTree->addOrUpdateNode(-1, "c3");
    (void)mpSearchTree->addOrUpdateNode(-5, "a1_2");
    (void)mpSearchTree->addOrUpdateNode(8, "b2");
    (void)mpSearchTree->addOrUpdateNode(-9, "h8");
    (void)mpSearchTree->addOrUpdateNode(-2, "e5");
    (void)mpSearchTree->addOrUpdateNode(0, "g7_2");
    (void)mpSearchTree->addOrUpdateNode(16, "i9_2");
    (void)mpSearchTree->addOrUpdateNode(-16, "m13");
    (void)mpSearchTree->addOrUpdateNode(14, "j10");
}

void MixedTreeTypesTests::_resetTreeObjects()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

QTEST_APPLESS_MAIN(MixedTreeTypesTests)

#include "tst_mixedtreetypestests.moc"
