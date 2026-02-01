// clang-format off
#include <QTest>

#include <algorithm>

#include "matrixutils.h"

class SortedRangesTests : public QObject
{
    Q_OBJECT

public:
    SortedRangesTests();

private slots:
    void testBounds();      // lower_bound, upper_bound
    void testBinarySearch();
    void testEqualRange();
    void testMerge();
    void testInplaceMerge();
    void testIncludes();
    void testSetDifference();
    void testSetIntersection();
    void testSetSymmetricDifference();
    void testSetUnion();

private:
    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const IntMatrix mFourthIntMatrix;
    const IntMatrix mFifthIntMatrix;
    const IntMatrix mSixthIntMatrix;
    const IntMatrix mSeventhIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
    const StringIntPairMatrix mSecondaryStringIntPairMatrix;
    const StringIntPairMatrix mThirdStringIntPairMatrix;
    const StringIntPairMatrix mFourthStringIntPairMatrix;
};

SortedRangesTests::SortedRangesTests()
    : mPrimaryIntMatrix{4, 5, {-9, -7, -5, -5, -3,
                               -3, -1,  0,  1,  1,
                                1,  2,  3,  3,  4,
                                5,  7,  11, 11, 14
                        }}
    , mSecondaryIntMatrix{4, 5, {32,  20, 11,  11, -6,
                                 28,  20,  12,  3, -6,
                                 25,  18,  14,  1, -9,
                                 20,  16,  15, -5, -15
                          }}
    , mThirdIntMatrix{4, 5, {22,  16, 14, 11, 11,
                              9,  5,  5,   3,  3,
                              2,  1,  1,   1, -3,
                             -5, -9, -10, -10, -14
                      }}
    , mFourthIntMatrix{4, 5, {25, 14,  14,  12,  9,
                               9,  7,   5,   4,  3,
                               3,  2,   1,   1,  1,
                               0, -7, -11, -11, -14
                       }}
    , mFifthIntMatrix{4, 5, {20, 16, 12, 11, 10,
                             10,  9,  7,  7,  6,
                              4,  3,  3,  2,  1,
                              0,  0, -2, -10, -20
                      }}
    , mSixthIntMatrix{4, 5, {-1,  6,  0,  5, 3,
                             -5, -1, -3,  3, 2,
                             -2,  1,  9, -8, 4,
                              0,  7, -4,  6, 8
                      }}
    , mSeventhIntMatrix{{4, 5}, -50}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Mark", 7}, {"Alex", 8}, {"Alistair", 8}, {"Cameron", 9}, {"Andrew", 10},
                                         {"Kevin", 11}, {"John", 11}, {"Reggie", 12}, {"George", 14}, {"Patrick", 15}
                                  }} // sorted increasingly by child age
    , mSecondaryStringIntPairMatrix{2, 5, {{"Christian", 13}, {"Patrick", 15}, {"Cameron", 9}, {"Andrew", 10}, {"Reggie", 12},
                                           {"George", 14}, {"Gordon", 7}, {"John", 11}, {"Alex", 8}, {"Mark", 7}
                                  }} // sorted decreasingly by number of characters of the child name
    , mThirdStringIntPairMatrix{{4, 5}, {"RESERVED", 0}}
    , mFourthStringIntPairMatrix{2, 5, {{"Alex", 8}, {"Chris", 9}, {"Erwin", 5}, {"Edgar", 7}, {"Reggie", 12},
                                        {"Camilla", 14}, {"Cameron", 11}, {"Patrick", 15}, {"Reginald", 10}, {"Alexander", 9}
                                 }} // sorted increasingly by number of characters
{
}

void SortedRangesTests::testBounds()
{
    // lower bound
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(0, 2) == std::lower_bound(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), -6));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(3, 4) == std::lower_bound(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 12));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(3, 4) == std::lower_bound(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 14));
    QVERIFY(mPrimaryIntMatrix.constZRowBegin(1) == std::lower_bound(mPrimaryIntMatrix.constZRowBegin(1), mPrimaryIntMatrix.constZRowEnd(1), -4));
    QVERIFY(mPrimaryIntMatrix.constZRowBegin(1) == std::lower_bound(mPrimaryIntMatrix.constZRowBegin(1), mPrimaryIntMatrix.constZRowEnd(1), -3));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(1, 3) == std::lower_bound(mPrimaryIntMatrix.constZRowBegin(1), mPrimaryIntMatrix.constZRowEnd(1), 1));
    QVERIFY(mPrimaryIntMatrix.getConstDIterator(2, 3) == std::lower_bound(mPrimaryIntMatrix.constDBegin(1), mPrimaryIntMatrix.constDEnd(1), 2));
    QVERIFY(mPrimaryIntMatrix.constMEnd(1) == std::lower_bound(mPrimaryIntMatrix.constMBegin(1), mPrimaryIntMatrix.constMEnd(1), 6));
    QVERIFY(mPrimaryIntMatrix.constMEnd(1) == std::lower_bound(mPrimaryIntMatrix.constMBegin(1), mPrimaryIntMatrix.constMEnd(1), 6));
    QVERIFY(mSecondaryIntMatrix.constReverseNColumnBegin(3) == std::lower_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), -6));
    QVERIFY(mSecondaryIntMatrix.constReverseNColumnBegin(3) == std::lower_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), -5));
    QVERIFY(mSecondaryIntMatrix.getConstReverseNIterator(2, 3) == std::lower_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), 0));
    QVERIFY(mSecondaryIntMatrix.constReverseNColumnEnd(3) == std::lower_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), 12));

    // upper bound
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(3, 1) == std::upper_bound(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 6));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(3, 2) == std::upper_bound(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 9));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(3, 4) == std::upper_bound(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 12));
    QVERIFY(mPrimaryIntMatrix.constZEnd() == std::upper_bound(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 14));
    QVERIFY(mPrimaryIntMatrix.constZRowBegin(1) == std::upper_bound(mPrimaryIntMatrix.constZRowBegin(1), mPrimaryIntMatrix.constZRowEnd(1), -4));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(1, 3) == std::upper_bound(mPrimaryIntMatrix.constZRowBegin(1), mPrimaryIntMatrix.constZRowEnd(1), 0));
    QVERIFY(mPrimaryIntMatrix.getConstDIterator(2, 3) == std::upper_bound(mPrimaryIntMatrix.constDBegin(1), mPrimaryIntMatrix.constDEnd(1), 2));
    QVERIFY(mSecondaryIntMatrix.constReverseNColumnBegin(3) == std::upper_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), -6));
    QVERIFY(mSecondaryIntMatrix.getConstReverseNIterator(2, 3) == std::upper_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), -5));
    QVERIFY(mSecondaryIntMatrix.getConstReverseNIterator(2, 3) == std::upper_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), 0));
    QVERIFY(mSecondaryIntMatrix.constReverseNColumnEnd(3) == std::upper_bound(mSecondaryIntMatrix.constReverseNColumnBegin(3), mSecondaryIntMatrix.constReverseNColumnEnd(3), 12));
}

void SortedRangesTests::testBinarySearch()
{
    QVERIFY(std::binary_search(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 11));
    QVERIFY(!std::binary_search(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), -2));
    QVERIFY(std::binary_search(mPrimaryIntMatrix.constMBegin(1), mPrimaryIntMatrix.constMEnd(1), 2));
    QVERIFY(!std::binary_search(mPrimaryIntMatrix.constMBegin(1), mPrimaryIntMatrix.constMEnd(1), 3));
    QVERIFY(!std::binary_search(mPrimaryIntMatrix.getConstNIterator(2, 1), mPrimaryIntMatrix.getConstNIterator(3, 2), 2)); // unsorted sequence
    QVERIFY(!std::binary_search(mPrimaryIntMatrix.getConstReverseZIterator(2, 3), mPrimaryIntMatrix.getConstReverseZIterator(1, 2), 1)); // the sequence should be reverse sorted (it's not!)
    QVERIFY(std::binary_search(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(2, 3), 0));
    QVERIFY(std::binary_search(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(2, 3), 1));
    QVERIFY(!std::binary_search(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(2, 1), 2));
    QVERIFY(std::binary_search(mThirdIntMatrix.getConstReverseZIterator(2, 3), mThirdIntMatrix.getConstReverseZIterator(1, 2), 1)); // now the sequence is reverse sorted
    QVERIFY(std::binary_search(mThirdIntMatrix.getConstReverseZIterator(2, 3), mThirdIntMatrix.getConstReverseZIterator(1, 2), 2));
    QVERIFY(std::binary_search(mFourthIntMatrix.getConstReverseZIterator(3, 0), mFourthIntMatrix.getConstReverseZIterator(1, 1), 0));
    QVERIFY(std::binary_search(mFourthIntMatrix.getConstReverseZIterator(3, 0), mFourthIntMatrix.getConstReverseZIterator(1, 1), 5));
    QVERIFY(!std::binary_search(mFourthIntMatrix.getConstReverseZIterator(3, 0), mFourthIntMatrix.getConstReverseZIterator(1, 1), -1));
    QVERIFY(!std::binary_search(mFourthIntMatrix.getConstReverseZIterator(3, 0), mFourthIntMatrix.getConstReverseZIterator(1, 1), 7));

    /* Case 1: sorting by age (increasing) */

    // age 12 is found for at least one of the elements, doesn't matter if the name is not the right one (sorting and comparing is done by age so from this standpoint at least one element is equivalent)
    QVERIFY(std::binary_search(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), StringIntPair{"Abigail", 12},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    // this time not, no child with age 13 exists in the table
    QVERIFY(!std::binary_search(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), StringIntPair{"Alistair", 13},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    // the element "fully" exists
    QVERIFY(std::binary_search(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), StringIntPair{"Alistair", 8},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    // wrong sorting order assumed, should be increasing ("direct" iterator)
    QVERIFY(!std::binary_search(mPrimaryStringIntPairMatrix.constReverseZBegin(), mPrimaryStringIntPairMatrix.constReverseZEnd(), StringIntPair{"Alistair", 12},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    /* Case 2: sorting by name size (decreasing) */

    // there is at least a child with name size 4 within table so equivalency is ensured
    QVERIFY(std::binary_search(mSecondaryStringIntPairMatrix.constReverseZBegin(), mSecondaryStringIntPairMatrix.constReverseZEnd(), StringIntPair{"Jack", 10},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));

    // this time not, there is no child within table with name size 8 so the passed value is not equivalent to any value within matrix (age 10 exists nonetheless)
    QVERIFY(!std::binary_search(mSecondaryStringIntPairMatrix.constReverseZBegin(), mSecondaryStringIntPairMatrix.constReverseZEnd(), StringIntPair{"Reginald", 10},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));

    // the element "fully" exists
    QVERIFY(std::binary_search(mSecondaryStringIntPairMatrix.constReverseZBegin(), mSecondaryStringIntPairMatrix.constReverseZEnd(), StringIntPair{"Cameron", 9},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));

    // wrong sorting order assumed, should be decreasing (reverse iterator)
    QVERIFY(!std::binary_search(mSecondaryStringIntPairMatrix.constZBegin(), mSecondaryStringIntPairMatrix.constZEnd(), StringIntPair{"Jack", 10},
                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));
}

void SortedRangesTests::testEqualRange()
{
    std::pair<IntMatrix::ConstZIterator, IntMatrix::ConstZIterator> itPair1{std::equal_range(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), 1)};
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(1, 3) == itPair1.first && mPrimaryIntMatrix.getConstZIterator(2, 1) == itPair1.second);

    std::pair<IntMatrix::ConstNIterator, IntMatrix::ConstNIterator> itPair2{std::equal_range(mPrimaryIntMatrix.constNColumnBegin(2), mPrimaryIntMatrix.constNColumnEnd(2), 1)};
    QVERIFY(mPrimaryIntMatrix.getConstNIterator(2, 2) == itPair2.first && mPrimaryIntMatrix.getConstNIterator(2, 2) == itPair2.second);

    std::pair<IntMatrix::ConstMIterator, IntMatrix::ConstMIterator> itPair3{std::equal_range(mPrimaryIntMatrix.constMBegin(1), mPrimaryIntMatrix.constMEnd(1), -4)};
    QVERIFY(mPrimaryIntMatrix.getConstMIterator(1, 2) == itPair3.first && mPrimaryIntMatrix.getConstMIterator(1, 2) == itPair3.second);

    std::pair<IntMatrix::ConstDIterator, IntMatrix::ConstDIterator> itPair4{std::equal_range(mPrimaryIntMatrix.constDBegin(1), mPrimaryIntMatrix.constDEnd(1), 14)};
    QVERIFY(mPrimaryIntMatrix.getConstDIterator(3, 4) == itPair4.first && mPrimaryIntMatrix.constDEnd(1) == itPair4.second);

    itPair4 = std::equal_range(mPrimaryIntMatrix.constDBegin(1), mPrimaryIntMatrix.constDEnd(1), 15);
    QVERIFY(mPrimaryIntMatrix.constDEnd(1) == itPair4.first && mPrimaryIntMatrix.constDEnd(1) == itPair4.second);

    std::pair<IntMatrix::ConstReverseZIterator, IntMatrix::ConstReverseZIterator> itPair5{std::equal_range(mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.constReverseZRowEnd(2), 3)};
    QVERIFY(mFifthIntMatrix.getConstReverseZIterator(2,2) == itPair5.first && mFifthIntMatrix.getConstReverseZIterator(2, 0) == itPair5.second);

    itPair5 = std::equal_range(mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.constReverseZRowEnd(2), 4);
    QVERIFY(mFifthIntMatrix.getConstReverseZIterator(2, 0) == itPair5.first && mFifthIntMatrix.constReverseZRowEnd(2) == itPair5.second);

    itPair5 = std::equal_range(mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.constReverseZRowEnd(2), 5);
    QVERIFY(mFifthIntMatrix.constReverseZRowEnd(2) == itPair5.first && mFifthIntMatrix.constReverseZRowEnd(2) == itPair5.second);

    itPair5 = std::equal_range(mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.constReverseZRowEnd(2), 0);
    QVERIFY(mFifthIntMatrix.constReverseZRowBegin(2) == itPair5.first && mFifthIntMatrix.constReverseZRowBegin(2) == itPair5.second);

    itPair5 = std::equal_range(mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.constReverseZRowEnd(2), 1);
    QVERIFY(mFifthIntMatrix.constReverseZRowBegin(2) == itPair5.first && mFifthIntMatrix.getConstReverseZIterator(2, 3) == itPair5.second);
}

void SortedRangesTests::testMerge()
{
    IntMatrix srcMatrix{mSixthIntMatrix};
    std::sort(srcMatrix.zRowBegin(1), srcMatrix.zRowEnd(1));
    std::sort(srcMatrix.zRowBegin(3), srcMatrix.zEnd());

    IntMatrix destMatrix{mFifthIntMatrix};

    const IntMatrix c_DestMatrixRef{4, 5, {20, 16, 12, 11, 10,
                                            8,  7,  6,  3,  2,
                                            0,  -1,  -3,  -4, -5,
                                            0,  0, -2, -10, -20
                             }};

    IntMatrix::ConstReverseZIterator it{std::merge(srcMatrix.zRowBegin(1), srcMatrix.zRowEnd(1), srcMatrix.zRowBegin(3), srcMatrix.zRowEnd(3), destMatrix.reverseZRowBegin(2))};

    QVERIFY(c_DestMatrixRef == destMatrix && destMatrix.constReverseZRowBegin(0) == it);
}

void SortedRangesTests::testInplaceMerge()
{
    IntMatrix matrix{mSixthIntMatrix};

    const IntMatrix c_MatrixRef{4, 5, {-1, -8, 0, 6, 3,
                                       -5, -4, 1, 6, 2,
                                       -2, -3, 3, 7, 4,
                                        0, -1, 5, 9, 8
                                }};

    std::sort(matrix.nColumnBegin(1), matrix.getNIterator(2, 2));
    std::sort(matrix.getNIterator(2, 2), matrix.nColumnEnd(3));

    std::inplace_merge(matrix.nColumnBegin(1), matrix.getNIterator(2, 2), matrix.nColumnEnd(3));

    QVERIFY(c_MatrixRef == matrix);
}

void SortedRangesTests::testIncludes()
{
    QVERIFY(std::includes(mFourthIntMatrix.constReverseZRowBegin(2), mFourthIntMatrix.getConstReverseZIterator(1, 1), mThirdIntMatrix.getConstReverseZIterator(2, 3), mThirdIntMatrix.getConstReverseZIterator(1, 1)));
    QVERIFY(!std::includes(mThirdIntMatrix.getConstReverseZIterator(2, 3), mThirdIntMatrix.getConstReverseZIterator(1, 1), mFourthIntMatrix.constReverseZRowBegin(2), mFourthIntMatrix.getConstReverseZIterator(1, 1)));
    QVERIFY(std::includes(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(3, 3), mFourthIntMatrix.getConstReverseZIterator(2, 0), mFourthIntMatrix.getConstReverseZIterator(1, 0)));
    QVERIFY(!std::includes(mFourthIntMatrix.getConstReverseZIterator(2, 0), mFourthIntMatrix.getConstReverseZIterator(1, 0), mFifthIntMatrix.getConstReverseZIterator(2, 2), mFifthIntMatrix.getConstReverseZIterator(1, 2)));
    QVERIFY(!std::includes(mFifthIntMatrix.getConstReverseZIterator(2, 2), mFifthIntMatrix.getConstReverseZIterator(1, 2), mFourthIntMatrix.getConstReverseZIterator(2, 0), mFourthIntMatrix.getConstReverseZIterator(1, 0)));
    QVERIFY(std::includes(mFourthIntMatrix.getConstReverseZIterator(2, 0), mFourthIntMatrix.getConstReverseZIterator(1, 0), mPrimaryIntMatrix.getConstZIterator(2, 2), mPrimaryIntMatrix.getConstZIterator(3, 2)));
    QVERIFY(std::includes(mPrimaryIntMatrix.getConstZIterator(2, 2), mPrimaryIntMatrix.getConstZIterator(3, 2), mFourthIntMatrix.getConstReverseZIterator(2, 0), mFourthIntMatrix.getConstReverseZIterator(1, 0)));
}

void SortedRangesTests::testSetDifference()
{
    IntMatrix matrix{mSeventhIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {0, -50, -50, -50, -50,
                                       1, -50, -50, -50, -50,
                                       1, -50, -50, -50, -50,
                                       5, -50, -50, -50,  6
                                }};

    IntMatrix::ConstNIterator it1{std::set_difference(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(3, 2), mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.getConstReverseZIterator(1, 2), matrix.nBegin())};

    QVERIFY(matrix.constNColumnBegin(1) == it1);

    IntMatrix::ConstReverseNIterator it2{std::set_difference(mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.getConstReverseZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(3, 2), matrix.reverseNBegin())};

    QVERIFY(c_MatrixRef == matrix && matrix.getConstReverseNIterator(2, 4) == it2);

    // same considerations as for the union scenario
    StringIntPairMatrix secondMatrix{mThirdStringIntPairMatrix};
    StringIntPairMatrix secondMatrixRef{4, 5, {{"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"Reggie", 12},
                                               {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"George", 14},
                                               {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"John", 11},
                                               {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"Andrew", 10},  {"Alex", 8}
                                        }};

    StringIntPairMatrix::ConstReverseNIterator it3{std::set_difference(mSecondaryStringIntPairMatrix.constReverseZBegin(),
                                                                mSecondaryStringIntPairMatrix.constReverseZEnd(),
                                                                mFourthStringIntPairMatrix.constZBegin(),
                                                                mFourthStringIntPairMatrix.constZEnd(),
                                                                secondMatrix.reverseNBegin(),
                                                                [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();})};

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(2, 3) == it3);

    secondMatrix = mThirdStringIntPairMatrix;
    secondMatrixRef = {4, 5, {{"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Patrick", 15},
                              {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Edgar", 7},
                              {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Erwin", 5},
                              {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"Reginald", 10}, {"Chris", 9}
                       }};

    it3 = std::set_difference(mFourthStringIntPairMatrix.constZBegin(),
                              mFourthStringIntPairMatrix.constZEnd(),
                              mSecondaryStringIntPairMatrix.constReverseZBegin(),
                              mSecondaryStringIntPairMatrix.constReverseZEnd(),
                              secondMatrix.reverseNBegin(),
                              [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(2, 3) == it3);
}

void SortedRangesTests::testSetIntersection()
{
    IntMatrix firstMatrix{mSeventhIntMatrix};
    const IntMatrix c_FirstMatrixRef{4, 5, {1,  3,  -50, -50, 7,
                                            1,  4,  -50, -50, 4,
                                            2,  5,  -50, -50, 3,
                                            3, -50, -50, -50, 3
                                     }};

    IntMatrix::ConstNIterator it1{std::set_intersection(mFourthIntMatrix.getConstReverseZIterator(2, 3), mFourthIntMatrix.getConstReverseZIterator(1, 1), mPrimaryIntMatrix.getConstZIterator(1, 3), mPrimaryIntMatrix.getConstZIterator(3, 2), firstMatrix.nBegin())};

    QVERIFY(firstMatrix.getConstNIterator(3, 1) == it1);

    IntMatrix::ConstReverseNIterator it2{std::set_intersection(mFifthIntMatrix.getConstReverseZIterator(2, 2), mFifthIntMatrix.getConstReverseZIterator(1, 2), mPrimaryIntMatrix.constZRowBegin(2), mPrimaryIntMatrix.constZEnd(), firstMatrix.reverseNBegin())};

    QVERIFY(c_FirstMatrixRef == firstMatrix && firstMatrix.constReverseNColumnBegin(3) == it2);

    // same considerations as for the union scenario
    StringIntPairMatrix secondMatrix{mThirdStringIntPairMatrix};
    StringIntPairMatrix secondMatrixRef{4, 5, {{"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},   {"Patrick", 15},
                                               {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},   {"Cameron", 9},
                                               {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},   {"Gordon", 7},
                                               {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"Christian", 13}, {"Mark", 7}
                                        }};

    StringIntPairMatrix::ConstReverseNIterator it3{std::set_intersection(mSecondaryStringIntPairMatrix.constReverseZBegin(),
                                                                mSecondaryStringIntPairMatrix.constReverseZEnd(),
                                                                mFourthStringIntPairMatrix.constZBegin(),
                                                                mFourthStringIntPairMatrix.constZEnd(),
                                                                secondMatrix.reverseNBegin(),
                                                                [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();})};

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(2, 3) == it3);

    secondMatrix = mThirdStringIntPairMatrix;
    secondMatrixRef = {4, 5, {{"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Cameron", 11},
                              {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Camilla", 14},
                              {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Reggie", 12},
                              {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0}, {"Alexander", 9}, {"Alex", 8}
                       }};

    it3 = std::set_intersection(mFourthStringIntPairMatrix.constZBegin(),
                                mFourthStringIntPairMatrix.constZEnd(),
                                mSecondaryStringIntPairMatrix.constReverseZBegin(),
                                mSecondaryStringIntPairMatrix.constReverseZEnd(),
                                secondMatrix.reverseNBegin(),
                                [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(2, 3) == it3);
}

void SortedRangesTests::testSetSymmetricDifference()
{
    IntMatrix firstMatrix{mSeventhIntMatrix};
    const IntMatrix c_FirstMatrixRef{4, 5, {0,   6, -50, -50,  5,
                                            1, -50, -50, -50,  1,
                                            1, -50, -50, -50,  1,
                                            5, -50, -50,   6,  0
                                     }};

    IntMatrix::ConstNIterator it1{std::set_symmetric_difference(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(3, 2), mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.getConstReverseZIterator(1, 2), firstMatrix.nBegin())};

    QVERIFY(firstMatrix.getConstNIterator(1, 1) == it1);

    IntMatrix::ConstReverseNIterator it2{std::set_symmetric_difference(mFifthIntMatrix.constReverseZRowBegin(2), mFifthIntMatrix.getConstReverseZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(3, 2), firstMatrix.reverseNBegin())};

    QVERIFY(c_FirstMatrixRef == firstMatrix && firstMatrix.getConstReverseNIterator(2, 3) == it2);

    // same considerations as for the union scenario
    // +: the symmetric difference is always the same regardless of order of operands (it mixes the two differences and sorts the elements by preserving their relative order when "equal")
    StringIntPairMatrix secondMatrix{mThirdStringIntPairMatrix};
    const StringIntPairMatrix c_SecondMatrixRef{4, 5, {{"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Andrew", 10}, {"Erwin", 5},
                                                       {"RESERVED", 0}, {"RESERVED", 0}, {"RESERVED", 0},  {"Reggie", 12}, {"Chris", 9},
                                                       {"RESERVED", 0}, {"RESERVED", 0}, {"Reginald", 10}, {"George", 14}, {"John", 11},
                                                       {"RESERVED", 0}, {"RESERVED", 0}, {"Patrick", 15},  {"Edgar", 7},   {"Alex", 8}
                                                }};

    StringIntPairMatrix::ConstReverseNIterator it3{std::set_symmetric_difference(mSecondaryStringIntPairMatrix.constReverseZBegin(),
                                                                                 mSecondaryStringIntPairMatrix.constReverseZEnd(),
                                                                                 mFourthStringIntPairMatrix.constZBegin(),
                                                                                 mFourthStringIntPairMatrix.constZEnd(),
                                                                                 secondMatrix.reverseNBegin(),
                                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();})};

    QVERIFY(c_SecondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(1, 2) == it3);

    secondMatrix = mThirdStringIntPairMatrix;

    it3 = std::set_symmetric_difference(mFourthStringIntPairMatrix.constZBegin(),
                                        mFourthStringIntPairMatrix.constZEnd(),
                                        mSecondaryStringIntPairMatrix.constReverseZBegin(),
                                        mSecondaryStringIntPairMatrix.constReverseZEnd(),
                                        secondMatrix.reverseNBegin(),
                                        [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(c_SecondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(1, 2) == it3);
}

void SortedRangesTests::testSetUnion()
{
    IntMatrix matrix{mSeventhIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {-3, 1, 3,  9,  -50,
                                       -1, 1, 4,  11, -50,
                                        0, 2, 5, -50, -50,
                                        1, 3, 7, -50, -50
                                }};

    IntMatrix::ConstNIterator it1{std::set_union(mFourthIntMatrix.getConstReverseZIterator(2, 3), mFourthIntMatrix.getConstReverseZIterator(0, 4), mPrimaryIntMatrix.constZRowBegin(1), mPrimaryIntMatrix.getConstZIterator(3, 3), matrix.nBegin())};

    QVERIFY(c_MatrixRef == matrix && matrix.getConstNIterator(2, 3) == it1);

    // in this scenario two elements are considered "equal" if they have the same number of characters (no matter the value of the integer pair element)
    StringIntPairMatrix secondMatrix{mThirdStringIntPairMatrix};
    StringIntPairMatrix secondMatrixRef{4, 5, {{"RESERVED", 0}, {"RESERVED", 0        }, {"Patrick", 15 /*1*/}, {"George", 14 /*1*/}, {"Chris", 9 /*2*/}, // 1: from mSecondaryStringIntPairMatrix
                                               {"RESERVED", 0}, {"Christian", 13 /*1*/}, {"Cameron", 9  /*1*/}, {"Gordon", 7  /*1*/}, {"John", 11 /*1*/}, // 2: from mThirdStringIntPairMatrix
                                               {"RESERVED", 0}, {"Reginald", 10  /*2*/}, {"Andrew", 10  /*1*/}, {"Edgar", 7   /*2*/}, {"Alex", 8  /*1*/},
                                               {"RESERVED", 0}, {"Patrick", 15   /*2*/}, {"Reggie", 12  /*1*/}, {"Erwin", 5   /*2*/}, {"Mark", 7  /*1*/}
                                        }};

    StringIntPairMatrix::ConstReverseNIterator it2{std::set_union(mSecondaryStringIntPairMatrix.constReverseZBegin(),
                                                                  mSecondaryStringIntPairMatrix.constReverseZEnd(),
                                                                  mFourthStringIntPairMatrix.constZBegin(),
                                                                  mFourthStringIntPairMatrix.constZEnd(),
                                                                  secondMatrix.reverseNBegin(),
                                                                  [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();})};

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(0, 1) == it2);

    secondMatrix = mThirdStringIntPairMatrix;
    secondMatrixRef = {4, 5, {{"RESERVED", 0}, {"RESERVED", 0       }, {"Cameron", 11 /*2*/}, {"George", 14 /*1*/}, {"Chris", 9 /*2*/}, // 1: from mSecondaryStringIntPairMatrix
                              {"RESERVED", 0}, {"Alexander", 9 /*2*/}, {"Camilla", 14 /*2*/}, {"Reggie", 12 /*2*/}, {"John", 11 /*1*/}, // 2: from mThirdStringIntPairMatrix
                              {"RESERVED", 0}, {"Reginald", 10 /*2*/}, {"Andrew", 10  /*1*/}, {"Edgar", 7   /*2*/}, {"Alex", 8  /*1*/},
                              {"RESERVED", 0}, {"Patrick", 15  /*2*/}, {"Reggie", 12  /*1*/}, {"Erwin", 5   /*2*/}, {"Alex", 8  /*2*/}
                       }};

    it2 = std::set_union(mFourthStringIntPairMatrix.constZBegin(),
                         mFourthStringIntPairMatrix.constZEnd(),
                         mSecondaryStringIntPairMatrix.constReverseZBegin(),
                         mSecondaryStringIntPairMatrix.constReverseZEnd(),
                         secondMatrix.reverseNBegin(),
                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstReverseNIterator(0, 1) == it2);
}

QTEST_APPLESS_MAIN(SortedRangesTests)

#include "tst_sortedrangestests.moc"
// clang-format on
