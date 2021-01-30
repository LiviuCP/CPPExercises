TEMPLATE = subdirs

SUBDIRS += \
    SimpleBSTTests \
    RedBlackTreesTests \
    AVLTreesTests \
    MixedTreeTypesTests \
    TestUtils

SimpleBSTTests.depends = TestUtils
RedBlackTreesTests.depends = TestUtils
AVLTreesTests.depends = TestUtils
MixedTreeTypesTests.depends = TestUtils

