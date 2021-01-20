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
