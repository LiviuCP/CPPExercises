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
    void testMergeDifferentSearchTrees();
    void testDifferentTreeTypesEquivalence();
    void testCopyAssignmentOfMixedTreeTypes();
    void testMoveAssignmentOfMixedTreeTypes();

private:
    void _reset();
    void _buildFirstMergeableTree();
    void _buildSecondMergeableTree();
    void _buildFirstSameContentTree(); // same content, different node adding order to the tree created by _buildFirstMergeableTree()
    void _buildSecondSameContentTree(); // same content, different node adding order to the tree created by _buildSecondMergeableTree()

    BinarySearchTree* mpSearchTree;
    BinarySearchTree* mpAuxSearchTree;
};

MixedTreeTypesTests::MixedTreeTypesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

MixedTreeTypesTests::~MixedTreeTypesTests()
{
    _reset();
}

void MixedTreeTypesTests::testMergeDifferentSearchTrees()
{
    /* create all trees to be used in the merge operations */
    _reset();

    mpSearchTree = new BinarySearchTree;
    mpAuxSearchTree = new BinarySearchTree;

    _buildFirstMergeableTree();
    _buildSecondMergeableTree();

    const BinarySearchTree simpleTreeCopy{*mpSearchTree};
    const BinarySearchTree simpleTreeAuxCopy{*mpAuxSearchTree};

    _reset();

    mpSearchTree = new RedBlackTree;
    mpAuxSearchTree = new RedBlackTree;

    _buildFirstMergeableTree();
    _buildSecondMergeableTree();

    const RedBlackTree redBlackTreeCopy{*dynamic_cast<RedBlackTree*>(mpSearchTree)};
    const RedBlackTree redBlackTreeAuxCopy{*dynamic_cast<RedBlackTree*>(mpAuxSearchTree)};

    _reset();

    mpSearchTree = new AVLTree;
    mpAuxSearchTree = new AVLTree;

    _buildFirstMergeableTree();
    _buildSecondMergeableTree();

    const AVLTree avlTreeCopy{*dynamic_cast<AVLTree*>(mpSearchTree)};
    const AVLTree avlTreeAuxCopy{*dynamic_cast<AVLTree*>(mpAuxSearchTree)};

    /* merge RED-BLACK tree into SIMPLE tree */
    _reset();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _reset();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14, true));

    /* merge AVL tree into SIMPLE tree */
    _reset();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _reset();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14, true));

    /* merge AVL tree into RED-BLACK tree */
    _reset();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:BK/16:i9_2:2:BK/-23:k11:-12:BK/-1:c3:-12:RD/8:b2:16:BK/17:l12:16:BK/-16:m13:-23R:RD/-5:a1_2:-1:BK/0:g7_2:-1:BK/7:f6:8:RD/14:j10:8:RD/-9:h8:-5:RD/-2:e5:-5:RD", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _reset();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-5:a1_1:-1:BK/8:b2:-1:BK/-12:n14:-5:RD/-2:e5:-5:BK/2:d4:8:BK/16:i9_1:8:BK/-16:m13:-12:BK/-9:h8:-12:BK/0:g7_1:2:RD/7:f6:2:RD/14:j10:16:RD/17:l12:16:RD/-23:k11:-16L:RD", 14, true));

    /* merge SIMPLE tree into RED-BLACK tree */
    _reset();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:BK/16:i9_2:2:BK/-23:k11:-12:BK/-1:c3:-12:RD/8:b2:16:BK/17:l12:16:BK/-16:m13:-23R:RD/-5:a1_2:-1:BK/0:g7_2:-1:BK/7:f6:8:RD/14:j10:8:RD/-9:h8:-5:RD/-2:e5:-5:RD", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _reset();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT:BK/-5:a1_1:-1:BK/8:b2:-1:BK/-16:m13:-5:RD/-2:e5:-5:BK/2:d4:8:BK/16:i9_1:8:BK/-23:k11:-16:BK/-9:h8:-16:BK/0:g7_1:2:RD/7:f6:2:RD/14:j10:16:RD/17:l12:16:RD/-12:n14:-9L:RD", 14, true));

    /* merge SIMPLE tree into AVL tree */
    _reset();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new BinarySearchTree{simpleTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-5:a1_2:2/16:i9_2:2/-12:n14:-5/-1:c3:-5/8:b2:16/17:l12:16/-23:k11:-12/-9:h8:-12/-2:e5:-1/0:g7_2:-1/7:f6:8/14:j10:8/-16:m13:-23R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _reset();

    mpSearchTree = new BinarySearchTree{simpleTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-9:h8:-1/8:b2:-1/-16:m13:-9/-5:a1_1:-9/2:d4:8/16:i9_1:8/-23:k11:-16/-12:n14:-16/-2:e5:-5R/0:g7_1:2/7:f6:2/14:j10:16/17:l12:16", 14, true));

    /* merge RED-BLACK tree into AVL tree */
    _reset();

    mpSearchTree = new AVLTree{avlTreeCopy};
    mpAuxSearchTree = new RedBlackTree{redBlackTreeAuxCopy};

    mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT/-5:a1_2:2/16:i9_2:2/-12:n14:-5/-1:c3:-5/8:b2:16/17:l12:16/-23:k11:-12/-9:h8:-12/-2:e5:-1/0:g7_2:-1/7:f6:8/14:j10:8/-16:m13:-23R", 14, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));

    _reset();

    mpSearchTree = new RedBlackTree{redBlackTreeCopy};
    mpAuxSearchTree = new AVLTree{avlTreeAuxCopy};

    mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-12:n14:-1/8:b2:-1/-16:m13:-12/-5:a1_1:-12/2:d4:8/16:i9_1:8/-23:k11:-16L/-9:h8:-5/-2:e5:-5/0:g7_1:2/7:f6:2/14:j10:16/17:l12:16", 14, true));
}

void MixedTreeTypesTests::testDifferentTreeTypesEquivalence()
{
    // Simple BST vs. Red-Black tree
    _reset();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildFirstMergeableTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildFirstSameContentTree();

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

    _reset();

    mpAuxSearchTree = new RedBlackTree{scCustomNullValue};
    _buildSecondMergeableTree();

    mpSearchTree = new BinarySearchTree{};
    _buildSecondSameContentTree();

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
    _reset();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildFirstMergeableTree();

    mpAuxSearchTree = new RedBlackTree{std::move(*dynamic_cast<RedBlackTree*>(mpSearchTree))};

    delete mpSearchTree;
    mpSearchTree = new AVLTree;
    _buildFirstMergeableTree();

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

    _reset();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondMergeableTree();

    mpSearchTree = new AVLTree{*dynamic_cast<AVLTree*>(mpAuxSearchTree)};

    delete mpAuxSearchTree;
    mpAuxSearchTree = new RedBlackTree;
    _buildSecondMergeableTree();

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
    _reset();

    mpSearchTree = new AVLTree{};
    _buildFirstMergeableTree();

    mpAuxSearchTree = new BinarySearchTree{};
    _buildFirstSameContentTree();

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

    _reset();

    mpAuxSearchTree = new AVLTree{};
    _buildSecondMergeableTree();

    mpSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondSameContentTree();

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
}

void MixedTreeTypesTests::testCopyAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    _reset();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondMergeableTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));

    _reset();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondMergeableTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    // Simple BST assigned to Red-Black tree (and vice-versa)
    _reset();

    mpSearchTree = new RedBlackTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "8:b2:ROOT/-1:c3:8/16:i9_2:8/-2:e5:-1/0:g7_2:-1/14:j10:16L/-5:a1_2:-2L/-9:h8:-5L/-16:m13:-9L", 9, true));

    _reset();

    mpSearchTree = new RedBlackTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // AVL tree assigned to Simple BST (and vice-versa)
    _reset();

    mpSearchTree = new BinarySearchTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));

    _reset();

    mpSearchTree = new BinarySearchTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/7:f6:2/17:l12:7R/16:i9_1:17L", 8, true));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/16:i9_1:2/7:f6:16/17:l12:16", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void MixedTreeTypesTests::testMoveAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    _reset();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondMergeableTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    _reset();

    mpSearchTree = new RedBlackTree{scCustomNullValue};
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree;
    _buildSecondMergeableTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    // Simple BST assigned to Red-Black tree (and vice-versa)
    _reset();

    mpSearchTree = new RedBlackTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    _reset();

    mpSearchTree = new RedBlackTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new BinarySearchTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // AVL tree assigned to Simple BST (and vice-versa)
    _reset();

    mpSearchTree = new BinarySearchTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9, true));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    _reset();

    mpSearchTree = new BinarySearchTree;
    _buildFirstMergeableTree();

    mpAuxSearchTree = new AVLTree{scCustomNullValue};
    _buildSecondMergeableTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0, true));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree, "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/16:i9_1:2/7:f6:16/17:l12:16", 8, true));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void MixedTreeTypesTests::_reset()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

void MixedTreeTypesTests::_buildFirstMergeableTree()
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

void MixedTreeTypesTests::_buildSecondMergeableTree()
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

void MixedTreeTypesTests::_buildFirstSameContentTree()
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

void MixedTreeTypesTests::_buildSecondSameContentTree()
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

QTEST_APPLESS_MAIN(MixedTreeTypesTests)

#include "tst_mixedtreetypestests.moc"
