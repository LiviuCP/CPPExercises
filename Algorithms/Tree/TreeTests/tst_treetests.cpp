#include <QTest>

#include "tree.h"

class TreeTests : public QObject
{
    Q_OBJECT

private slots:
    void testRecursiveInversion();
    void testIterativeInversion();
    void testAddRootNode();

    void testRecursiveInversion_data();
    void testIterativeInversion_data();

private:
    void _buildInversionTestTable();

    void _buildTrees();
    void _resetTrees();

    void _buildTree1();
    void _buildTree2();
    void _buildTree3();
    void _buildTree4();
    void _buildTree5();
    void _buildEmptyTree();

    TreeSp m_Tree1;
    TreeSp m_Tree2;
    TreeSp m_Tree3;
    TreeSp m_Tree4;
    TreeSp m_Tree5;
    TreeSp m_EmptyTree;
};

void TreeTests::testRecursiveInversion()
{
    QFETCH(TreeSp, tree);
    QFETCH(NodeValues, treeValues);
    QFETCH(NodeValues, invertedTreeValues);

    QVERIFY(tree);
    QVERIFY(tree->getNodeValues() == treeValues);

    tree->invertRecursively();
    QVERIFY(tree->getNodeValues() == invertedTreeValues);

    tree->invertRecursively();
    QVERIFY(tree->getNodeValues() == treeValues);

    tree->clear();

    QVERIFY(tree->size() == 0);
    QVERIFY(tree->empty());
}

void TreeTests::testIterativeInversion()
{
    QFETCH(TreeSp, tree);
    QFETCH(NodeValues, treeValues);
    QFETCH(NodeValues, invertedTreeValues);

    QVERIFY(tree);
    QVERIFY(tree->getNodeValues() == treeValues);

    tree->invertIteratively();
    QVERIFY(tree->getNodeValues() == invertedTreeValues);

    tree->invertIteratively();
    QVERIFY(tree->getNodeValues() == treeValues);

    tree->clear();

    QVERIFY(tree->size() == 0);
    QVERIFY(tree->empty());
}

void TreeTests::testAddRootNode()
{
    Tree tree;
    QVERIFY(tree.empty());

    bool added = tree.addRootNode(5);
    QVERIFY(added);

    const NodeValues c_RequiredTreeValues{5};
    QVERIFY(tree.getNodeValues() == c_RequiredTreeValues);

    added = tree.addRootNode(-9);
    QVERIFY(!added);

    QVERIFY(tree.getNodeValues() == c_RequiredTreeValues);

    tree.clear();
    QVERIFY(tree.empty());

    added = tree.addRootNode(-9);
    QVERIFY(added);

    const NodeValues c_NewRequiredTreeValues{-9};
    QVERIFY(tree.getNodeValues() == c_NewRequiredTreeValues);

    NodeSp root = tree.getRootNode().lock();
    QVERIFY(root && root->getValue() == -9);

    root->createAndAppendChildren({-5, 3, 4});
    QVERIFY(tree.size() == 4);

    const NodeValues c_FinalRequiredTreeValues{-9, -5, 3, 4};
    QVERIFY(tree.getNodeValues() == c_FinalRequiredTreeValues);

    added = tree.addRootNode(0);
    QVERIFY(!added);

    QVERIFY(tree.getNodeValues() == c_FinalRequiredTreeValues);
}

void TreeTests::testRecursiveInversion_data()
{
    _buildInversionTestTable();
}

void TreeTests::testIterativeInversion_data()
{
    _buildInversionTestTable();
}

void TreeTests::_buildInversionTestTable()
{
    QTest::addColumn<TreeSp>("tree");
    QTest::addColumn<NodeValues>("treeValues");
    QTest::addColumn<NodeValues>("invertedTreeValues");

    _buildTrees();

    QTest::newRow("1: inversion") << m_Tree1 << NodeValues{5, -2, 8, 4, -5, 3, -9, 7} << NodeValues{5, 8, -2, 7, -9, 3, -5, 4};
    QTest::newRow("2: inversion") << m_Tree2 << NodeValues{-9, -5, 14, -2, 10, 8, 16, -17, 18, -19} << NodeValues{-9, 14, -5, 8, 10, -2, -19, 18, -17, 16};
    QTest::newRow("3: inversion") << m_Tree3 << NodeValues{9, 5, -14, -8, 20, -16, 17} << NodeValues{9, -14, 5, -8, 20, 17, -16};
    QTest::newRow("4: inversion") << m_Tree4 << NodeValues{-2, 4, 3, 0} << NodeValues{-2, 4, 3, 0};
    QTest::newRow("5: inversion") << m_Tree5 << NodeValues{9} << NodeValues{9};
    QTest::newRow("6: inversion") << m_EmptyTree << NodeValues{} << NodeValues{};

    _resetTrees();
}

void TreeTests::_buildTrees()
{
    _buildTree1();
    _buildTree2();
    _buildTree3();
    _buildTree4();
    _buildTree5();
    _buildEmptyTree();
}

void TreeTests::_resetTrees()
{
    m_Tree1.reset();
    m_Tree2.reset();
    m_Tree3.reset();
    m_Tree4.reset();
    m_Tree5.reset();
    m_EmptyTree.reset();
}

void TreeTests::_buildTree1()
{
    m_Tree1 = std::make_shared<Tree>(5);
    QVERIFY(m_Tree1);

    NodeSp rootNode = m_Tree1->getRootNode().lock();
    QVERIFY(rootNode && rootNode->getValue() == 5);

    rootNode->createAndAppendChildren({-2, 8});

    const NodeSp c_Left = rootNode->getChildAtIndex(0).lock();
    QVERIFY(c_Left && c_Left->getValue() == -2);

    c_Left->createAndAppendChildren({4, -5, 3});

    const NodeSp c_Right = rootNode->getChildAtIndex(1).lock();
    QVERIFY(c_Right && c_Right->getValue() == 8);

    c_Right->createAndAppendChildren({-9, 7});

    const NodeValues c_RequiredTreeValues{5, -2, 8, 4, -5, 3, -9, 7};

    QVERIFY(m_Tree1->getNodeValues() == c_RequiredTreeValues);
    QVERIFY(m_Tree1->size() == 8);
}

void TreeTests::_buildTree2()
{
    m_Tree2 = std::make_shared<Tree>(-9);
    QVERIFY(m_Tree2);

    NodeSp rootNode = m_Tree2->getRootNode().lock();
    QVERIFY(rootNode && rootNode->getValue() == -9);

    rootNode->createAndAppendChildren({-5, 14});

    const NodeSp c_Left = rootNode->getChildAtIndex(0).lock();
    QVERIFY(c_Left && c_Left->getValue() == -5);

    c_Left->createAndAppendChildren({-2, 10});

    const NodeSp c_Right = rootNode->getChildAtIndex(1).lock();
    QVERIFY(c_Right && c_Right->getValue() == 14);

    c_Right->createAndAppendChildren({8});

    const NodeSp c_Additional = c_Left->getChildAtIndex(1).lock();
    QVERIFY(c_Additional && c_Additional->getValue() == 10);

    c_Additional->createAndAppendChildren({16, -17, 18, -19});

    const NodeValues c_RequiredTreeValues{-9, -5, 14, -2, 10, 8, 16, -17, 18, -19};

    QVERIFY(m_Tree2->getNodeValues() == c_RequiredTreeValues);
    QVERIFY(m_Tree2->size() == 10);
}

void TreeTests::_buildTree3()
{
    m_Tree3 = std::make_shared<Tree>(9);
    QVERIFY(m_Tree3);

    NodeSp rootNode = m_Tree3->getRootNode().lock();
    QVERIFY(rootNode && rootNode->getValue() == 9);

    rootNode->createAndAppendChildren({5, -14});

    const NodeSp c_Left = rootNode->getChildAtIndex(0).lock();
    QVERIFY(c_Left && c_Left->getValue() == 5);

    const NodeSp c_Right = rootNode->getChildAtIndex(1).lock();
    QVERIFY(c_Right && c_Right->getValue() == -14);

    c_Right->createAndAppendChildren({-8});

    const NodeSp c_Additional1 = c_Right->getChildAtIndex(0).lock();
    QVERIFY(c_Additional1 && c_Additional1->getValue() == -8);

    c_Additional1->createAndAppendChildren({20});

    const NodeSp c_Additional2 = c_Additional1->getChildAtIndex(0).lock();
    QVERIFY(c_Additional2 && c_Additional2->getValue() == 20);

    c_Additional2->createAndAppendChildren({-16, 17});

    const NodeValues c_RequiredTreeValues{9, 5, -14, -8, 20, -16, 17};

    QVERIFY(m_Tree3->getNodeValues() == c_RequiredTreeValues);
    QVERIFY(m_Tree3->size() == 7);
}

void TreeTests::_buildTree4()
{
    m_Tree4 = std::make_shared<Tree>(-2);
    QVERIFY(m_Tree4);

    NodeSp rootNode = m_Tree4->getRootNode().lock();
    QVERIFY(rootNode && rootNode->getValue() == -2);

    rootNode->createAndAppendChildren({4});

    NodeSp child = rootNode->getChildAtIndex(0).lock();
    QVERIFY(child && child->getValue() == 4);

    child->createAndAppendChildren({3});

    child = child->getChildAtIndex(0).lock();
    QVERIFY(child && child->getValue() == 3);

    child->createAndAppendChildren({0});

    child = child->getChildAtIndex(0).lock();
    QVERIFY(child && child->getValue() == 0);

    const NodeValues c_RequiredTreeValues{-2, 4, 3, 0};

    QVERIFY(m_Tree4->getNodeValues() == c_RequiredTreeValues);
    QVERIFY(m_Tree4->size() == 4);
}

void TreeTests::_buildTree5()
{
    m_Tree5 = std::make_shared<Tree>(9);
    QVERIFY(m_Tree5);

    NodeSp rootNode = m_Tree5->getRootNode().lock();
    QVERIFY(rootNode && rootNode->getValue() == 9);

    const NodeValues c_RequiredTreeValues{9};

    QVERIFY(m_Tree5->getNodeValues() == c_RequiredTreeValues);
    QVERIFY(m_Tree5->size() == 1);
}

void TreeTests::_buildEmptyTree()
{
    m_EmptyTree = std::make_shared<Tree>();

    QVERIFY(m_EmptyTree);
    QVERIFY(m_EmptyTree->empty());
}

QTEST_APPLESS_MAIN(TreeTests)

#include "tst_treetests.moc"
