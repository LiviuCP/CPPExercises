#include <QtTest>

#include "testutils.h"
#include "binarysearchtree.h"
#include "redblacktree.h"
#include "avltree.h"

using namespace TestUtils;
using IntStrBinarySearchTree = BinarySearchTree<int, std::string>;
using IntStrRedBlackTree = RedBlackTree<int, std::string>;
using IntStrAVLTree = AVLTree<int, std::string>;
using upIntStrBinarySearchTree = std::unique_ptr<IntStrBinarySearchTree>;

class MixedTreeTypesTests : public QObject
{
    Q_OBJECT

public:
    MixedTreeTypesTests();

private slots:
    void init();
    void cleanup();

    void testMergeDifferentSearchTrees();
    void testFailToMergeWhenNullValuesAreDifferent();
    void testDifferentTreeTypesEquivalence();
    void testCopyAssignmentOfMixedTreeTypes();
    void testMoveAssignmentOfMixedTreeTypes();
    void testPassThroughAllLogMessages();

private:
    void _buildPrimaryTestSearchTree(); // create content for the primary test tree (referenced by mpSearchTree)
    void _buildSecondaryTestSearchTree(); // create content for the secondary test tree (referenced by mpAuxSearchTree)
    void _buildPrimaryTestSearchTreeInDifferentOrder(); // same content, different node adding order to the tree created by _buildPrimaryTestSearchTree() method (referenced by mpAuxSearchTree)
    void _buildSecondaryTestSearchTreeInDifferentOrder(); // same content, different node adding order to the tree created by _buildSecondaryTestSearchTree() method (referenced by mpSearchTree)
    
    upIntStrBinarySearchTree mpSearchTree;
    upIntStrBinarySearchTree mpAuxSearchTree;
};

MixedTreeTypesTests::MixedTreeTypesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
    IntStrBinarySearchTree::enableLogging(false);
}

void MixedTreeTypesTests::init()
{
    QVERIFY(!mpSearchTree && !mpAuxSearchTree);
}

void MixedTreeTypesTests::cleanup()
{
    mpSearchTree.reset();
    mpAuxSearchTree.reset();

    IntStrBinarySearchTree::enableLogging(false);
}

void MixedTreeTypesTests::testMergeDifferentSearchTrees()
{
    /* create all trees to be used in the merge operations */
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();
    
    const IntStrBinarySearchTree simpleTreeCopy{*mpSearchTree};
    const IntStrBinarySearchTree simpleTreeAuxCopy{*mpAuxSearchTree};

    mpSearchTree = std::make_unique<IntStrRedBlackTree>();
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    const IntStrRedBlackTree redBlackTreeCopy{*dynamic_cast<IntStrRedBlackTree*>(mpSearchTree.get())};
    const IntStrRedBlackTree redBlackTreeAuxCopy{*dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get())};

    mpSearchTree = std::make_unique<IntStrAVLTree>();
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    const IntStrAVLTree avlTreeCopy{*dynamic_cast<IntStrAVLTree*>(mpSearchTree.get())};
    const IntStrAVLTree avlTreeAuxCopy{*dynamic_cast<IntStrAVLTree*>(mpAuxSearchTree.get())};

    /* merge RED-BLACK tree into SIMPLE tree */
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeAuxCopy);

    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14));

    /* merge AVL tree into SIMPLE tree */
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(avlTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1_2:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_2:2/7:f6:2/-16:m13:-12/-9:h8:-12/-1:c3:0L/17:l12:7R/-2:e5:-1L/16:i9_2:17L/8:b2:16L/14:j10:8R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "8:b2:ROOT/-1:c3:8/16:i9_1:8/-2:e5:-1/0:g7_1:-1/14:j10:16/17:l12:16/-5:a1_1:-2L/2:d4:0R/-9:h8:-5L/7:f6:2R/-16:m13:-9L/-23:k11:-16/-12:n14:-16", 14));

    /* merge AVL tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(avlTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:BK/16:i9_2:2:BK/-23:k11:-12:BK/-1:c3:-12:RD/8:b2:16:BK/17:l12:16:BK/-16:m13:-23R:RD/-5:a1_2:-1:BK/0:g7_2:-1:BK/7:f6:8:RD/14:j10:8:RD/-9:h8:-5:RD/-2:e5:-5:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_1:-1:BK/8:b2:-1:BK/-12:n14:-5:RD/-2:e5:-5:BK/2:d4:8:BK/16:i9_1:8:BK/-16:m13:-12:BK/-9:h8:-12:BK/0:g7_1:2:RD/7:f6:2:RD/14:j10:16:RD/17:l12:16:RD/-23:k11:-16L:RD", 14));

    /* merge SIMPLE tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:BK/16:i9_2:2:BK/-23:k11:-12:BK/-1:c3:-12:RD/8:b2:16:BK/17:l12:16:BK/-16:m13:-23R:RD/-5:a1_2:-1:BK/0:g7_2:-1:BK/7:f6:8:RD/14:j10:8:RD/-9:h8:-5:RD/-2:e5:-5:RD", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_1:-1:BK/8:b2:-1:BK/-16:m13:-5:RD/-2:e5:-5:BK/2:d4:8:BK/16:i9_1:8:BK/-23:k11:-16:BK/-9:h8:-16:BK/0:g7_1:2:RD/7:f6:2:RD/14:j10:16:RD/17:l12:16:RD/-12:n14:-9L:RD", 14));

    /* merge SIMPLE tree into AVL tree */

    mpSearchTree = std::make_unique<IntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-5:a1_2:2/16:i9_2:2/-12:n14:-5/-1:c3:-5/8:b2:16/17:l12:16/-23:k11:-12/-9:h8:-12/-2:e5:-1/0:g7_2:-1/7:f6:8/14:j10:8/-16:m13:-23R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(simpleTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(avlTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-9:h8:-1/8:b2:-1/-16:m13:-9/-5:a1_1:-9/2:d4:8/16:i9_1:8/-23:k11:-16/-12:n14:-16/-2:e5:-5R/0:g7_1:2/7:f6:2/14:j10:16/17:l12:16", 14));

    /* merge RED-BLACK tree into AVL tree */

    mpSearchTree = std::make_unique<IntStrAVLTree>(avlTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeAuxCopy);

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-5:a1_2:2/16:i9_2:2/-12:n14:-5/-1:c3:-5/8:b2:16/17:l12:16/-23:k11:-12/-9:h8:-12/-2:e5:-1/0:g7_2:-1/7:f6:8/14:j10:8/-16:m13:-23R", 14));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(redBlackTreeCopy);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(avlTreeAuxCopy);

    merged = mpAuxSearchTree->mergeTree(*mpSearchTree);

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-12:n14:-1/8:b2:-1/-16:m13:-12/-5:a1_1:-12/2:d4:8/16:i9_1:8/-23:k11:-16L/-9:h8:-5/-2:e5:-5/0:g7_1:2/7:f6:2/14:j10:16/17:l12:16", 14));
}

void MixedTreeTypesTests::testFailToMergeWhenNullValuesAreDifferent()
{
    /* (attempt to) merge RED-BLACK tree into SIMPLE tree */
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/7:f6:2/17:l12:7R/16:i9_1:17L", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge AVL tree into SIMPLE tree */
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/7:f6:2/17:l12:7R/16:i9_1:17L", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge AVL tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge SIMPLE tree into RED-BLACK tree */

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "8:b2:ROOT/-1:c3:8/16:i9_2:8/-2:e5:-1/0:g7_2:-1/14:j10:16L/-5:a1_2:-2L/-9:h8:-5L/-16:m13:-9L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge SIMPLE tree into AVL tree */

    mpSearchTree = std::make_unique<IntStrAVLTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "8:b2:ROOT/-1:c3:8/16:i9_2:8/-2:e5:-1/0:g7_2:-1/14:j10:16L/-5:a1_2:-2L/-9:h8:-5L/-16:m13:-9L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());

    /* (attempt to) merge RED-BLACK tree into AVL tree */

    mpSearchTree = std::make_unique<IntStrAVLTree>(scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>();

    _buildPrimaryTestSearchTree();
    _buildSecondaryTestSearchTree();

    merged = mpSearchTree->mergeTree(*mpAuxSearchTree);

    QVERIFY(!merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue() &&
            scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void MixedTreeTypesTests::testDifferentTreeTypesEquivalence()
{
    // Simple BST vs. Red-Black tree
    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    _buildPrimaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    (void)mpSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    (void)mpAuxSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*dynamic_cast<IntStrRedBlackTree*>(mpSearchTree.get()) == *mpAuxSearchTree);

    (void)mpSearchTree->addOrUpdateNode(1, "abcd");
    (void)mpAuxSearchTree->addOrUpdateNode(1, "abcd");
    QVERIFY(*dynamic_cast<IntStrRedBlackTree*>(mpSearchTree.get()) != *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14_1:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/0:g7_1:-12:BK/7:f6:16:RD/17:l12:16:RD/-5:a1_1:0:RD/1:abcd:0:RD", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R/1:abcd:0R", 9));

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();
    _buildSecondaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);

    (void)mpSearchTree->removeNode(14);
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(*mpSearchTree == *dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()));

    (void)mpSearchTree->removeNode(-2);
    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(*mpSearchTree != *dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5L/0:g7_2:8/16:i9_2:8/-16:m13:-9L", 7));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-9:h8:-1:RD/8:b2:-1:RD/-16:m13:-9:BK/-5:a1_2:-9:BK/0:g7_2:8:BK/16:i9_2:8:BK", 7));

    // Red-Black tree vs. AVL tree

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::move(*dynamic_cast<IntStrRedBlackTree*>(mpSearchTree.get())));

    mpSearchTree = std::make_unique<IntStrAVLTree>();
    _buildPrimaryTestSearchTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8));

    QVERIFY(*mpSearchTree == *mpAuxSearchTree);
    QVERIFY(*mpSearchTree == *dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()));

    (void)mpSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpSearchTree != *mpAuxSearchTree);
    QVERIFY(*mpSearchTree != *dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()));

    (void)mpAuxSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()) == *dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()));
    QVERIFY(*dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()) == *mpAuxSearchTree);

    (void)mpAuxSearchTree->addOrUpdateNode(25, "abcd");
    QVERIFY(*dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()) != *dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()));
    QVERIFY(*dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()) != *mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14_1:2:RD/16:i9_1:2:RD/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:BK/17:l12:16:BK/0:g7_1:-5R:RD/25:abcd:17R:RD", 9));

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    mpSearchTree = std::make_unique<IntStrAVLTree>(*dynamic_cast<IntStrAVLTree*>(mpAuxSearchTree.get()));

    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>();
    _buildSecondaryTestSearchTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);
    QVERIFY(*mpAuxSearchTree == *dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()));

    (void)mpSearchTree->removeNode(-2);
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);
    QVERIFY(*mpAuxSearchTree != *dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()));

    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(*dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()) == *dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()));
    QVERIFY(*dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()) == *mpSearchTree);

    (void)mpSearchTree->removeNode(14);
    QVERIFY(*dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()) != *dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()));
    QVERIFY(*dynamic_cast<IntStrRedBlackTree*>(mpAuxSearchTree.get()) != *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-9:h8:-1/8:b2:-1/-16:m13:-9/-5:a1_2:-9/0:g7_2:8/16:i9_2:8", 7));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT:BK/-9:h8:-1:RD/8:b2:-1:RD/-16:m13:-9:BK/-5:a1_2:-9:BK/0:g7_2:8:BK/16:i9_2:8:BK/14:j10:16L:RD", 8));

    // AVL tree vs. Simple BST

    mpSearchTree = std::make_unique<IntStrAVLTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>();
    _buildPrimaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    (void)mpSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);

    (void)mpAuxSearchTree->addOrUpdateNode(-12, "n14_1");
    QVERIFY(*mpAuxSearchTree == *dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()));

    (void)mpSearchTree->addOrUpdateNode(1, "abcd");
    (void)mpAuxSearchTree->addOrUpdateNode(1, "abcd");
    QVERIFY(*mpAuxSearchTree != *dynamic_cast<IntStrAVLTree*>(mpSearchTree.get()));

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/0:g7_1:-12/7:f6:16/17:l12:16/-5:a1_1:0/1:abcd:0", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14_1:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R/1:abcd:0R", 9));

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();
    _buildSecondaryTestSearchTree();
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTreeInDifferentOrder();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));

    QVERIFY(*mpAuxSearchTree == *mpSearchTree);

    (void)mpSearchTree->removeNode(14);
    QVERIFY(*mpAuxSearchTree != *mpSearchTree);

    (void)mpAuxSearchTree->removeNode(14);
    QVERIFY(*dynamic_cast<IntStrAVLTree*>(mpAuxSearchTree.get()) == *mpSearchTree);

    (void)mpSearchTree->removeNode(-2);
    (void)mpAuxSearchTree->removeNode(-2);
    QVERIFY(*dynamic_cast<IntStrAVLTree*>(mpAuxSearchTree.get()) != *mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5L/0:g7_2:8/16:i9_2:8/-16:m13:-9L", 7));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-9:h8:-1/8:b2:-1/-16:m13:-9/-5:a1_2:-9/0:g7_2:8/16:i9_2:8", 7));

    // additional tests
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{2, -1, 3}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{1, -1, 3}, scDefaultValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{2, -1, 3, 4}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(std::vector<int>{2, -1, 3, -5}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrAVLTree>(std::vector<int>{2, -1, 3, -2}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{2, -1, 3, 8}, scDefaultValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrAVLTree>(std::vector<int>{2, -1, 3, 4}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{2, -1, 3, 5}, scDefaultValue, scCustomNullValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{2, -1, 3, -4}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{2, -1, 3, -5}, scDefaultValue);

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{2, -1, 3}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(std::vector<int>{2, -1, 3}, scDefaultValue, scCustomNullValue);
    mpSearchTree->addOrUpdateNode(2, "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>(std::vector<int>{2, -1, 3, -4}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{2, -1, 3, -4}, scDefaultValue);
    mpAuxSearchTree->addOrUpdateNode(-4, "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{2, -1, 3, 5}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(std::vector<int>{2, -1, 3, 5}, scDefaultValue, scCustomNullValue);
    mpAuxSearchTree->addOrUpdateNode(5, "abcd");

    QVERIFY(*mpSearchTree != *mpAuxSearchTree);
}

void MixedTreeTypesTests::testCopyAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));

    /* simulate how the destination tree would be built by adding the keys of the source tree in (top-down/left-right) order
       (no need to take the values into consideration so default values are being used for simplicity)
    */
    QVERIFY("-1:ROOT:BK/-5:-1:RD/8:-1:RD/-9:-5:BK/-2:-5:BK/0:8:BK/16:8:BK/-16:-9L:RD/14:16L:RD" ==
            IntStrRedBlackTree(std::vector<int>{-1, -5, 8, -9, -2, 0, 16, -16, 14}, scDefaultValue).getTreeAsString(false));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("2:ROOT/-12:2/16:2/-23:-12/-5:-12/7:16/17:16/0:-5R" ==
            IntStrAVLTree(std::vector<int>{2, -12, 16, -23, -5, 7, 17, 0}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    // Simple BST assigned to Red-Black tree (and vice-versa)

    mpSearchTree = std::make_unique<IntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "8:b2:ROOT/-1:c3:8/16:i9_2:8/-2:e5:-1/0:g7_2:-1/14:j10:16L/-5:a1_2:-2L/-9:h8:-5L/-16:m13:-9L", 9));

    QVERIFY("-1:ROOT:BK/-5:-1:RD/8:-1:RD/-9:-5:BK/-2:-5:BK/0:8:BK/16:8:BK/-16:-9L:RD/14:16L:RD" ==
            IntStrRedBlackTree(std::vector<int>{8, -1, 16, -2, 0, 14, -5, -9, -16}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:d4:ROOT:BK/-12:n14:2:RD/16:i9_1:2:BK/-23:k11:-12:BK/-5:a1_1:-12:BK/7:f6:16:RD/17:l12:16:RD/0:g7_1:-5R:RD", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("2:ROOT/-12:2/16:2/-23:-12/-5:-12/7:16/17:16/0:-5R" ==
            IntStrBinarySearchTree(std::vector<int>{2, -12, 16, -23, -5, 7, 17, 0}, scDefaultValue).getTreeAsString(false));

    // AVL tree assigned to Simple BST (and vice-versa)
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = *mpAuxSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));

    QVERIFY("-1:ROOT/-5:-1/8:-1/-9:-5/-2:-5/0:8/16:8/-16:-9L/14:16L" ==
            IntStrBinarySearchTree(std::vector<int>{-1, -5, 8, -9, -2, 0, 16, -16, 14}, scDefaultValue, scCustomNullValue).getTreeAsString(false));

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = *mpSearchTree;

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/7:f6:2/17:l12:7R/16:i9_1:17L", 8));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/16:i9_1:2/7:f6:16/17:l12:16", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    QVERIFY("-5:ROOT/-23:-5/2:-5/-12:-23R/0:2/16:2/7:16/17:16" ==
            IntStrAVLTree(std::vector<int>{-5, -23, 2, -12, 0, 7, 17, 16}, scDefaultValue).getTreeAsString(false));
}

/* Same test base used as for the copy assignment test
   The destination tree should be the same as when copy assigning while the source tree should become empty
   No in-order simulation by using default values (see copy assignment test) is required any longer
*/
void MixedTreeTypesTests::testMoveAssignmentOfMixedTreeTypes()
{
    // AVL tree assigned to Red-Black tree (and vice-versa)
    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(scCustomNullValue);
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>();
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    // Simple BST assigned to Red-Black tree (and vice-versa)

    mpSearchTree = std::make_unique<IntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT:BK/-5:a1_2:-1:RD/8:b2:-1:RD/-9:h8:-5:BK/-2:e5:-5:BK/0:g7_2:8:BK/16:i9_2:8:BK/-16:m13:-9L:RD/14:j10:16L:RD", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree = std::make_unique<IntStrRedBlackTree>();
    _buildPrimaryTestSearchTree();
    
    mpAuxSearchTree = std::make_unique<IntStrBinarySearchTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "2:d4:ROOT/-12:n14:2/16:i9_1:2/-23:k11:-12/-5:a1_1:-12/7:f6:16/17:l12:16/0:g7_1:-5R", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());

    // AVL tree assigned to Simple BST (and vice-versa)
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpSearchTree = std::move(*mpAuxSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "-1:c3:ROOT/-5:a1_2:-1/8:b2:-1/-9:h8:-5/-2:e5:-5/0:g7_2:8/16:i9_2:8/-16:m13:-9L/14:j10:16L", 9));
    QVERIFY(scCustomNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scCustomNullValue == mpAuxSearchTree->getNullValue());

    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();
    _buildPrimaryTestSearchTree();

    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(scCustomNullValue);
    _buildSecondaryTestSearchTree();

    *mpAuxSearchTree = std::move(*mpSearchTree);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(scDefaultNullValue == mpSearchTree->getNullValue());
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), "-5:a1_1:ROOT/-23:k11:-5/2:d4:-5/-12:n14:-23R/0:g7_1:2/16:i9_1:2/7:f6:16/17:l12:16", 8));
    QVERIFY(scDefaultNullValue == mpAuxSearchTree->getNullValue());
}

void MixedTreeTypesTests::testPassThroughAllLogMessages()
{
    IntStrBinarySearchTree::enableLogging(true);
    
    mpSearchTree = std::make_unique<IntStrBinarySearchTree>();
    mpAuxSearchTree = std::make_unique<IntStrRedBlackTree>();

    mpSearchTree->printTree();
    mpAuxSearchTree->printTree();

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), scEmptyTreeString, 0));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(true), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

    mpSearchTree = std::make_unique<IntStrRedBlackTree>(std::vector<int>{2, 3, 2, -4}, scDefaultValue);
    mpAuxSearchTree = std::make_unique<IntStrAVLTree>(std::vector<int>{2, 5, 2, 2}, scDefaultValue);

    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(), mpSearchTree->getSize(), "2:ROOT:BK/-4:2:RD/3:2:RD", 3));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), "2:ROOT/5:2R", 2));

    mpAuxSearchTree->addOrUpdateNode(2, "abcd");

    qInfo("Printing red-black tree before merge");
    mpSearchTree->printTree();
    qInfo("Printing AVL tree before merge");
    mpAuxSearchTree->printTree();

    const bool merged{mpSearchTree->mergeTree(*mpAuxSearchTree)};

    QVERIFY(merged);
    QVERIFY(areExpectedTreeValuesMet(mpSearchTree->getTreeAsString(true), mpSearchTree->getSize(), "2:abcd:ROOT:BK/-4:DF:2:BK/3:DF:2:BK/5:DF:3R:RD", 4));
    QVERIFY(areExpectedTreeValuesMet(mpAuxSearchTree->getTreeAsString(), mpAuxSearchTree->getSize(), scEmptyTreeString, 0));

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

QTEST_APPLESS_MAIN(MixedTreeTypesTests)

#include "tst_mixedtreetypestests.moc"
