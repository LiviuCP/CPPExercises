#include <QtTest>

#include "redblacktree.h"

class RedBlackTreesTests : public QObject
{
    Q_OBJECT

public:
    RedBlackTreesTests();
    ~RedBlackTreesTests();

private slots:
    void testAddNodes();

private:
    void _reset();
    bool _areExpectedTreeValuesMet(const RedBlackTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered = false) const;

    RedBlackTree* mpSearchTree;
    RedBlackTree* mpAuxSearchTree;

    static const std::string scDefaultValue;
    static const std::string scDefaultNullValue;
    static const std::string scCustomNullValue;
    static const std::string scEmptyTreeString;
};

const std::string RedBlackTreesTests::scDefaultValue{"DF"};
const std::string RedBlackTreesTests::scDefaultNullValue{""};
const std::string RedBlackTreesTests::scCustomNullValue{"/0"};
const std::string RedBlackTreesTests::scEmptyTreeString{""};

RedBlackTreesTests::RedBlackTreesTests()
    : mpSearchTree{nullptr}
    , mpAuxSearchTree{nullptr}
{
}

RedBlackTreesTests::~RedBlackTreesTests()
{

}

void RedBlackTreesTests::testAddNodes()
{
    _reset();

    bool newNodeAdded{false};

    mpSearchTree = new RedBlackTree;

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-5, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT:BK", 1));

    newNodeAdded = mpSearchTree->addOrUpdateNode(8, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-5:ROOT:BK/8:-5:RD", 2));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-1, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:RD/8:-1:RD", 3));

    newNodeAdded = mpSearchTree->addOrUpdateNode(2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/2:8:RD", 4));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-2, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/8:-1:BK/-2:-5:RD/2:8:RD", 5));

    newNodeAdded = mpSearchTree->addOrUpdateNode(7, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-2:-5:RD/2:7:RD/8:7:RD", 6));

    newNodeAdded = mpSearchTree->addOrUpdateNode(0, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2:RD", 7));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-9, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2:RD", 8));

    newNodeAdded = mpSearchTree->addOrUpdateNode(16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/8:7:BK/0:2:RD/16:8:RD", 9));

    newNodeAdded = mpSearchTree->addOrUpdateNode(14, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:RD/-9:-5:RD/-2:-5:RD/2:7:BK/14:7:BK/0:2:RD/8:14:RD/16:14:RD", 10));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-23, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:RD/7:-1:RD/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:BK/-23:-9:RD/0:2:RD/8:14:RD/16:14:RD", 11));

    newNodeAdded = mpSearchTree->addOrUpdateNode(17, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-9:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-9:RD/0:2:RD/8:14:BK/16:14:BK/17:16:RD", 12));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-16, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:BK/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:RD/-9:-16:RD/0:2:RD/8:14:BK/16:14:BK/17:16:RD", 13));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-12, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2:RD/8:14:BK/16:14:BK/-12:-9:RD/17:16:RD", 14));

    newNodeAdded = mpSearchTree->addOrUpdateNode(19, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree,"-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-9:-16:BK/0:2:RD/8:14:BK/17:14:BK/-12:-9:RD/16:17:RD/19:17:RD", 15));

    newNodeAdded = mpSearchTree->addOrUpdateNode(-15, scDefaultValue);
    QVERIFY(newNodeAdded &&
            _areExpectedTreeValuesMet(mpSearchTree, "-1:ROOT:BK/-5:-1:BK/7:-1:BK/-16:-5:RD/-2:-5:BK/2:7:BK/14:7:RD/-23:-16:BK/-12:-16:BK/0:2:RD/8:14:BK/17:14:BK/-15:-12:RD/-9:-12:RD/16:17:RD/19:17:RD", 16));

    // adding nodes to custom null value tree (compare with default null value tree)
    mpAuxSearchTree = new RedBlackTree{std::vector<int>{-5, 8, -1, 2, -2, 7, 0, -9, 16, 14, -23, 17, -16, -12, 19, -15}, scDefaultValue, scCustomNullValue};

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

    QVERIFY(_areExpectedTreeValuesMet(mpAuxSearchTree, "-1:ROOT:BK/-5:-1:BK/14:-1:BK/-16:-5:RD/-2:-5:BK/7:14:RD/17:14:RD/-23:-16:BK/-12:-16:BK/2:7:BK/8:7:BK/16:17:BK/19:17:BK/-15:-12:RD/-9:-12:RD/0:2:RD/25:19:RD", 17));

    // some additional (corner) cases
    delete mpSearchTree;
    mpSearchTree = new RedBlackTree{std::vector<int>{-23, -16, -15, -12, -9, -5, -2, -1, 0, 2, 7, 8, 14, 16, 17, 19}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "-12:ROOT:BK/-16:-12:BK/-1:-12:RD/-23:-16:BK/-15:-16:BK/-5:-1:BK/8:-1:BK/-9:-5:BK/-2:-5:BK/2:8:RD/16:8:RD/0:2:BK/7:2:BK/14:16:BK/17:16:BK/19:17:RD", 16));

    delete mpSearchTree;
    mpSearchTree = new RedBlackTree{std::vector<int>{19, 17, 16, 14, 8, 7, 2, 0, -1, -2, -5, -9, -12, -15, -16, -23}, scDefaultValue};

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, "14:ROOT:BK/0:14:RD/17:14:BK/-9:0:BK/7:0:BK/16:17:BK/19:17:BK/-15:-9:RD/-2:-9:RD/2:7:BK/8:7:BK/-16:-15:BK/-12:-15:BK/-5:-2:BK/-1:-2:BK/-23:-16:RD", 16));

    mpSearchTree->clear();

    QVERIFY(_areExpectedTreeValuesMet(mpSearchTree, scEmptyTreeString, 0));
}

void RedBlackTreesTests::_reset()
{
    delete mpSearchTree;
    mpSearchTree = nullptr;
    delete mpAuxSearchTree;
    mpAuxSearchTree = nullptr;
}

bool RedBlackTreesTests::_areExpectedTreeValuesMet(const RedBlackTree* searchTree, const std::string& treeString, int treeSize, bool areNodeValuesConsidered) const
{
    bool areValuesMet{false};

    if (searchTree != nullptr)
    {
        areValuesMet = (treeString == searchTree->getTreeAsString(areNodeValuesConsidered) &&
                        treeSize == searchTree->getSize());
    }

    return areValuesMet;
}

QTEST_APPLESS_MAIN(RedBlackTreesTests)

#include "tst_redblacktreestests.moc"
