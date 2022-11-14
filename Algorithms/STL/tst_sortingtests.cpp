#include <QtTest>

#include <algorithm>

#include "datautils.h"

class SortingTests : public QObject
{
    Q_OBJECT

public:
    SortingTests();

private slots:
    void testIsSorted(); // is_sorted, is_sorted_until
    void testSort();
    void testPartialSort();
    void testPartialSortCopy();
    void testStableSort();
    void testNthElement();

private:
    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntPairMatrix mPrimaryIntPairMatrix;
    const IntPairMatrix mSecondaryIntPairMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
};

SortingTests::SortingTests()
    : mPrimaryIntMatrix{3, 4, {-1,  1, 1,  0,
                                2,  4, 5,  1,
                                3, -3, -3, 2
                        }}
    , mSecondaryIntMatrix{2, 2, {-2, 8,
                                 -5, 9
                          }}
    , mPrimaryIntPairMatrix{4, 3, {{1, 4}, {-1, 0}, {-1, 2},
                                   {4, -5}, {1, 2}, {1, 3},
                                   {-2, 4}, {2, 0}, {-3, -3},
                                   {3, 2}, {-1, 0}, {3, -1}
                            }}
    , mSecondaryIntPairMatrix{4, 3, {{4, 1}, {0, -1}, {2, -1},
                                     {-5, 4}, {2, 1}, {3,  1},
                                     {4, -2}, {0, 2}, {-3, -3},
                                     {2, 3}, {0, -1},  {-1, 3}
                              }}
    , mPrimaryStringIntPairMatrix{2, 4, {{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14},
                                         {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}
                                  }}
{
}

void SortingTests::testIsSorted()
{
    QVERIFY(!std::is_sorted(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd()));
    QVERIFY(std::is_sorted(mPrimaryIntMatrix.getConstZIterator(0, 3), mPrimaryIntMatrix.getConstZIterator(1, 2)));
    QVERIFY(std::is_sorted_until(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd()) == mPrimaryIntMatrix.getConstZIterator(0, 3));
}

void SortingTests::testSort()
{
    StringIntPairMatrix matrix{mPrimaryStringIntPairMatrix};
    const StringIntPairMatrix c_MatrixRef{2, 4, {{"Alex", 10}, {"Alistair", 10}, {"George", 14}, {"Kevin", 11},
                                                 {"Cameron", 10}, {"Andrew", 11}, {"Reggie", 12}, {"Patrick", 14}
                                          }};

    std::sort(matrix.zBegin(), matrix.zRowEnd(0));
    std::sort(matrix.zRowBegin(1), matrix.zEnd(), [](StringIntPair firstPair, StringIntPair secondPair){return firstPair.second < secondPair.second;});

    QVERIFY(c_MatrixRef == matrix);
}

void SortingTests::testPartialSort()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    const IntMatrix c_MatrixRef{3, 4, {-3, 0, 1, -9,
                                       -3, 1, -9, -9,
                                       -1, 1, -9, -9
                                }};

    std::partial_sort(matrix.nBegin(), matrix.getNIterator(1, 2), matrix.nEnd());

    IntMatrix::ConstNIterator firstIt{matrix.constNBegin()};
    IntMatrix::ConstNIterator secondIt{c_MatrixRef.constNBegin()};

    bool areElementsEqual{true};

    while(firstIt != matrix.getConstNIterator(1, 2) && secondIt != c_MatrixRef.getConstNIterator(1, 2))
    {
        areElementsEqual = areElementsEqual && (*firstIt == *secondIt);
        ++firstIt;
        ++secondIt;
    }

    QVERIFY(areElementsEqual);
}

void SortingTests::testPartialSortCopy()
{
    IntMatrix srcMatrix{mPrimaryIntMatrix};
    IntMatrix destMatrix{mSecondaryIntMatrix};
    IntMatrix destMatrixRef{2, 2, {1, 5,
                                   2, 9
                            }};

    std::partial_sort_copy(srcMatrix.constDBegin(1), srcMatrix.constDEnd(1), destMatrix.nBegin(), destMatrix.nEnd());

    QVERIFY(destMatrix == destMatrixRef);

    destMatrix = mSecondaryIntMatrix;
    destMatrixRef = {2, 2, {2, 8,
                            -5, 1
                     }};

    std::partial_sort_copy(srcMatrix.constDBegin(1), srcMatrix.constDEnd(1), destMatrix.reverseDBegin(0), destMatrix.reverseDEnd(0));

    QVERIFY(destMatrix == destMatrixRef);
}

void SortingTests::testStableSort()
{
    IntPairMatrix matrix{mPrimaryIntPairMatrix};
    IntPairMatrix matrixRef{4, 3, {{-3, -3}, {-2, 4}, {-1, 0},
                                   {-1, 2},  {-1, 0}, {1, 4},
                                    {1, 2},  {1, 3},  {2, 0},
                                    {3, 2},  {3, -1}, {4, -5}
                            }};

    std::stable_sort(matrix.zBegin(), matrix.zEnd(), [](IntPair firstPair, IntPair secondPair){return firstPair.first < secondPair.first;});

    QVERIFY(matrix == matrixRef);

    matrix = mSecondaryIntPairMatrix;
    matrixRef = {4, 3, {{-3, -3}, {4, -2}, {0, -1},
                         {2, -1}, {0, -1}, {4, 1},
                         {2, 1},  {3, 1},  {0, 2},
                         {2, 3},  {-1, 3}, {-5, 4}
                 }};

    std::stable_sort(matrix.zBegin(), matrix.zEnd(), [](IntPair firstPair, IntPair secondPair){return firstPair.second < secondPair.second;});

    QVERIFY(matrix == matrixRef);
}

void SortingTests::testNthElement()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    std::nth_element(matrix.nBegin(), matrix.getNIterator(1, 2), matrix.nEnd());

    const IntMatrix::size_type nthElementRowNr{1};
    const IntMatrix::size_type nthElementColumnNr{2};
    IntMatrix::ConstNIterator nthElementIt{matrix.getConstNIterator(nthElementRowNr, nthElementColumnNr)};
    const int expectedNthElementValue{2};

    QVERIFY(expectedNthElementValue == *nthElementIt);

    bool areLeftElementsLowerEqual{true};

    for (IntMatrix::ConstNIterator it{matrix.constNBegin()}; it != matrix.getConstNIterator(nthElementRowNr, nthElementColumnNr); ++it)
    {
        areLeftElementsLowerEqual = areLeftElementsLowerEqual && (*it <= *nthElementIt);
    }

    bool areRightElementsHigherEqual{true};

    for (IntMatrix::ConstNIterator it{nthElementIt + 1}; it != matrix.constNEnd(); ++it)
    {
        areRightElementsHigherEqual = areRightElementsHigherEqual && (*it >= *nthElementIt);
    }

    QVERIFY(areLeftElementsLowerEqual && areRightElementsHigherEqual);
}

QTEST_APPLESS_MAIN(SortingTests)

#include "tst_sortingtests.moc"
