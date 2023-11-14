#include <QtTest>

#include <algorithm>

#include "tst_combinedstltests_data.h"
#include "gather.h"

class CombinedSTLTests : public QObject
{
    Q_OBJECT

public:
    CombinedSTLTests();

private slots:
    void testGatherAlgorithmStdVector();
    void testGatherAlgorithmMatrix();

    void testGatherAlgorithmStdVector_data();
    void testGatherAlgorithmMatrix_data();

private:
    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const IntVector mPrimaryIntVector;
    const IntVector mSecondaryIntVector;
    const IntVector mThirdIntVector;
    const IntVector mFourthIntVector;
    const IntVector mFifthIntVector;
};

CombinedSTLTests::CombinedSTLTests()
    : mPrimaryIntMatrix{8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                                4, 20, -7,   2,   2,  2,  10, -5,  -2,
                                5, -5,  2,  -12, -2,  6,   7,  8,  -9,
                               -6,  4,  14, -5,   0,  0,  12,  14, -7,
                               -3,  3, -3,   4,  19,  15, 11,  0,  -9,
                                8, -8, -14,  8,  17, -14, -12, 4,   2,
                                5,  5,  5,   5,   4,  3,   2, -10,  4,
                               48,  2, -3,  -4,  -5,  6,  -7,  18,  0
                       }}
    , mSecondaryIntMatrix{1, 9, {1, -2, 0, 5, -4, -2, 3, 0, -8}}
    , mThirdIntMatrix{9, 1, {-1, 5, 3, -3, 0, 4, -7, 9, -5}}
    , mPrimaryIntVector{2, -1, 5, -8, -7, 0, 3, 4, -5, 10, 8, -9, 0, 1}
    , mSecondaryIntVector{5, 14}
    , mThirdIntVector{-5, 14}
    , mFourthIntVector{5, -14}
    , mFifthIntVector{-5, -14}
{
}

void CombinedSTLTests::testGatherAlgorithmStdVector()
{
    QFETCH(IntVector, inputVector);
    QFETCH(IntVector::difference_type, beginIteratorOffset);
    QFETCH(IntVector::difference_type, endIteratorOffset);
    QFETCH(IntVector::difference_type, gatheringPointIteratorOffset);
    QFETCH(std::function<bool(const int&)>, predicate);
    QFETCH(IntVector, inputVectorRef);
    QFETCH(IntVector::difference_type, gatheringStartIteratorOffset);
    QFETCH(IntVector::difference_type, gatheringEndIteratorOffset);

    const auto&[gatheringStartIt, gatheringEndIt]{gatherSequenceElements(inputVector.begin() + beginIteratorOffset,
                                                                          inputVector.begin() + endIteratorOffset,
                                                                          inputVector.begin() + gatheringPointIteratorOffset,
                                                                          predicate)};

    QVERIFY(std::equal(inputVector.cbegin(), inputVector.cend(), inputVectorRef.cbegin()));
    QVERIFY(gatheringStartIt == inputVector.begin() + gatheringStartIteratorOffset && gatheringEndIt == inputVector.begin() + gatheringEndIteratorOffset);
}

void CombinedSTLTests::testGatherAlgorithmMatrix()
{
    QFETCH(IntMatrix, inputMatrix);
    QFETCH(MatrixPoint<int>, startingPoint);
    QFETCH(MatrixPoint<int>, endingPoint);
    QFETCH(MatrixPoint<int>, gatheringPoint);
    QFETCH(std::function<bool(const int&)>, predicate);
    QFETCH(IntMatrix, inputMatrixRef);
    QFETCH(MatrixPoint<int>, gatheringStartingPointRef);
    QFETCH(MatrixPoint<int>, gatheringEndingPointRef);
    QFETCH(MatrixSizeType<int>, gatheredElementsCountRef);

    const auto&[gatheringStartingPoint, gatheringEndingPoint, gatheredElementsCount]{gatherMatrixElements(inputMatrix, startingPoint, endingPoint, gatheringPoint, predicate)};

    QVERIFY(inputMatrixRef == inputMatrix);
    QVERIFY(gatheringStartingPoint == gatheringStartingPointRef && gatheringEndingPoint == gatheringEndingPointRef && gatheredElementsCount == gatheredElementsCountRef);
}

void CombinedSTLTests::testGatherAlgorithmStdVector_data()
{
    QTest::addColumn<IntVector>("inputVector");
    QTest::addColumn<IntVector::difference_type>("beginIteratorOffset");
    QTest::addColumn<IntVector::difference_type>("endIteratorOffset");
    QTest::addColumn<IntVector::difference_type>("gatheringPointIteratorOffset");
    QTest::addColumn<std::function<bool(const int&)>>("predicate");
    QTest::addColumn<IntVector>("inputVectorRef");
    QTest::addColumn<IntVector::difference_type>("gatheringStartIteratorOffset");
    QTest::addColumn<IntVector::difference_type>("gatheringEndIteratorOffset");

    std::function<bool(const int&)> isNegativeInt{[](const int& element) {return element < 0;}};

    QTest::newRow("1") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{14} << IntVectorDiff{4} << isNegativeInt << IntVector{2, 5, -1, -8, -7, -5, -9, 0, 3, 4, 10, 8, 0, 1} << IntVectorDiff{2} << IntVectorDiff{7};
    QTest::newRow("2") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{14} << IntVectorDiff{0} << isNegativeInt << IntVector{-1, -8, -7, -5, -9, 2, 5, 0, 3, 4, 10, 8, 0, 1} << IntVectorDiff{0} << IntVectorDiff{5};
    QTest::newRow("3") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{14} << IntVectorDiff{14} << isNegativeInt << IntVector{2, 5, 0, 3, 4, 10, 8, 0, 1, -1, -8, -7, -5, -9} << IntVectorDiff{9} << IntVectorDiff{14};
    QTest::newRow("4") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{5} << isNegativeInt << IntVector{2, -1, 5, -8, -7, -5, 0, 3, 4, 10, 8, -9, 0, 1} << IntVectorDiff{3} << IntVectorDiff{6};
    QTest::newRow("5") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{2} << isNegativeInt << IntVector{2, -1, -8, -7, -5, 5, 0, 3, 4, 10, 8, -9, 0, 1} << IntVectorDiff{2} << IntVectorDiff{5};
    QTest::newRow("6") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{1} << isNegativeInt << IntVector{2, -1, -8, -7, -5, 5, 0, 3, 4, 10, 8, -9, 0, 1} << IntVectorDiff{2} << IntVectorDiff{5};
    QTest::newRow("7") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{0} << isNegativeInt << IntVector{2, -1, -8, -7, -5, 5, 0, 3, 4, 10, 8, -9, 0, 1} << IntVectorDiff{2} << IntVectorDiff{5};
    QTest::newRow("8") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{10} << isNegativeInt << IntVector{2, -1, 5, 0, 3, 4, 10, -8, -7, -5, 8, -9, 0, 1} << IntVectorDiff{7} << IntVectorDiff{10};
    QTest::newRow("9") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{11} << isNegativeInt << IntVector{2, -1, 5, 0, 3, 4, 10, -8, -7, -5, 8, -9, 0, 1} << IntVectorDiff{7} << IntVectorDiff{10};
    QTest::newRow("10") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{13} << isNegativeInt << IntVector{2, -1, 5, 0, 3, 4, 10, -8, -7, -5, 8, -9, 0, 1} << IntVectorDiff{7} << IntVectorDiff{10};
    QTest::newRow("11") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{0} << IntVectorDiff{0} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{0};
    QTest::newRow("12") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{0} << IntVectorDiff{14} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("13") << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4} << IntVectorDiff{4} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4};
    QTest::newRow("14") << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4} << IntVectorDiff{4} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4};
    QTest::newRow("15") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{5} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("16") << mPrimaryIntVector << IntVectorDiff{5} << IntVectorDiff{8} << IntVectorDiff{6} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{6} << IntVectorDiff{6};
    QTest::newRow("17") << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{0};
    QTest::newRow("18") << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mSecondaryIntVector << IntVectorDiff{1} << IntVectorDiff{1};
    QTest::newRow("19") << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << mSecondaryIntVector << IntVectorDiff{2} << IntVectorDiff{2};
    QTest::newRow("20") << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("21") << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("22") << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << IntVector{14, -5} << IntVectorDiff{1} << IntVectorDiff{2};
    QTest::newRow("23") << mFourthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << IntVector{-14, 5} << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("24") << mFourthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mFourthIntVector << IntVectorDiff{1} << IntVectorDiff{2};
    QTest::newRow("25") << mFourthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << mFourthIntVector << IntVectorDiff{1} << IntVectorDiff{2};
    QTest::newRow("26") << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2};
    QTest::newRow("27") << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2};
    QTest::newRow("28") << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2};
    QTest::newRow("29") << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{0} << isNegativeInt << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("30") << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{1} << isNegativeInt << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("31") << IntVector{5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{0} << isNegativeInt << IntVector{5} << IntVectorDiff{0} << IntVectorDiff{0};
    QTest::newRow("32") << IntVector{5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{1} << isNegativeInt << IntVector{5} << IntVectorDiff{1} << IntVectorDiff{1};
    QTest::newRow("33") << IntVector{} << IntVectorDiff{0} << IntVectorDiff{0} << IntVectorDiff{0} << isNegativeInt << IntVector{} << IntVectorDiff{0} << IntVectorDiff{0};
}

void CombinedSTLTests::testGatherAlgorithmMatrix_data()
{
    QTest::addColumn<IntMatrix>("inputMatrix");
    QTest::addColumn<MatrixPoint<int>>("startingPoint");
    QTest::addColumn<MatrixPoint<int>>("endingPoint");
    QTest::addColumn<MatrixPoint<int>>("gatheringPoint");
    QTest::addColumn<std::function<bool(const int&)>>("predicate");
    QTest::addColumn<IntMatrix>("inputMatrixRef");
    QTest::addColumn<MatrixPoint<int>>("gatheringStartingPointRef");
    QTest::addColumn<MatrixPoint<int>>("gatheringEndingPointRef");
    QTest::addColumn<MatrixSizeType<int>>("gatheredElementsCountRef");

    std::function<bool(const int&)> isNegativeInt{[](const int& element) {return element < 0;}};

    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_InputMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};

    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};

    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};

    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, 0} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, -1} << isNegativeInt << c_InputMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};

    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, 9} << isNegativeInt << c_InputMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 8} << isNegativeInt << c_InputMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    // input bounding rectangle: (2, 3) -> (8, 9): all elements excluding the left/bottom edges
    QTest::newRow("2a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 5} << isNegativeInt << c_InputMatrixRef2a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 7} << MatrixSizeType<int>{12};
    QTest::newRow("2a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{3, 5} << isNegativeInt << c_InputMatrixRef2a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 7} << MatrixSizeType<int>{12};

    // input bounding rectangle: (2, 0) -> (8, 3): all elements excluding the left/bottom edges
    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{5, 1} << isNegativeInt << c_InputMatrixRef2b << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{5, 1} << isNegativeInt << c_InputMatrixRef2b << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};

    // input bounding rectangle: (0, 3) -> (2, 9): all elements excluding the left/bottom edges
    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{1, 6} << isNegativeInt << c_InputMatrixRef2c << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 6} << isNegativeInt << c_InputMatrixRef2c << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};

    // input bounding rectangle: (0, 0) -> (2, 3): all elements excluding the left/bottom edges
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 2} << isNegativeInt << c_InputMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 2} << isNegativeInt << c_InputMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};

    QTest::newRow("3a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 4} << isNegativeInt << c_InputMatrixRef3a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 6} << MatrixSizeType<int>{7};
    QTest::newRow("3a") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 4} << isNegativeInt << c_InputMatrixRef3a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 6} << MatrixSizeType<int>{7};

    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 3} << isNegativeInt << c_InputMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 3} << isNegativeInt << c_InputMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 5} << isNegativeInt << c_InputMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 5} << isNegativeInt << c_InputMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 5} << isNegativeInt << c_InputMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << isNegativeInt << c_InputMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{4, 0} << isNegativeInt << c_InputMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{4, 0} << isNegativeInt << c_InputMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{4, 2} << isNegativeInt << c_InputMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{4, 2} << isNegativeInt << c_InputMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 8} << isNegativeInt << c_InputMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 8} << isNegativeInt << c_InputMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 7} << isNegativeInt << c_InputMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 7} << isNegativeInt << c_InputMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 0} << isNegativeInt << c_InputMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{1, 0} << isNegativeInt << c_InputMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << isNegativeInt << c_InputMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 2} << isNegativeInt << c_InputMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 7} << isNegativeInt << c_InputMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 7} << isNegativeInt << c_InputMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 1} << isNegativeInt << c_InputMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 1} << isNegativeInt << c_InputMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 2} << isNegativeInt << c_InputMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 2} << isNegativeInt << c_InputMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 8} << isNegativeInt << c_InputMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 7} << isNegativeInt << c_InputMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << isNegativeInt << c_InputMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    QTest::newRow("4a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 5} << MatrixPoint<int>{4, 8} << MatrixPoint<int>{3, 6} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{3, 6} << MatrixPoint<int>{3, 6} << MatrixSizeType<int>{0};
    QTest::newRow("4a") << mPrimaryIntMatrix << MatrixPoint<int>{4, 8} << MatrixPoint<int>{2, 5} << MatrixPoint<int>{3, 6} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{3, 6} << MatrixPoint<int>{3, 6} << MatrixSizeType<int>{0};

    QTest::newRow("5a") << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{3, 7} << MatrixPoint<int>{3, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5a") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 7} << MatrixPoint<int>{3, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{3, 7} << MatrixPoint<int>{3, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5b") << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{1, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5b") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 7} << MatrixPoint<int>{0, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{1, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5c") << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5c") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 7} << MatrixPoint<int>{7, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5d") << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{4, 6} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{4, 7} << MatrixPoint<int>{4, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5d") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 7} << MatrixPoint<int>{4, 6} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{4, 7} << MatrixPoint<int>{4, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5e") << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 8} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{1, 7} << MatrixSizeType<int>{0};
    QTest::newRow("5e") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 7} << MatrixPoint<int>{0, 8} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{1, 7} << MatrixPoint<int>{1, 7} << MatrixSizeType<int>{0};

    QTest::newRow("6a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 3} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{0};
    QTest::newRow("6a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 3} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{0};
    QTest::newRow("6b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 0} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{0};
    QTest::newRow("6b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 0} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{0};
    QTest::newRow("6c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 6} << MatrixSizeType<int>{0};
    QTest::newRow("6c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 6} << MatrixSizeType<int>{0};
    QTest::newRow("6d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 6} << MatrixPoint<int>{3, 4} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 4} << MatrixPoint<int>{2, 4} << MatrixSizeType<int>{0};
    QTest::newRow("6d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 1} << MatrixPoint<int>{3, 4} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 4} << MatrixPoint<int>{2, 4} << MatrixSizeType<int>{0};
    QTest::newRow("6e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 1} << MatrixPoint<int>{2, 6} << MatrixPoint<int>{1, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 6} << MatrixSizeType<int>{0};
    QTest::newRow("6e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 1} << MatrixPoint<int>{1, 7} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 6} << MatrixPoint<int>{2, 6} << MatrixSizeType<int>{0};

    QTest::newRow("7a") << mPrimaryIntMatrix << MatrixPoint<int>{4, 4} << MatrixPoint<int>{4, 4} << MatrixPoint<int>{4, 4} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{4, 4} << MatrixPoint<int>{4, 4} << MatrixSizeType<int>{0};
    QTest::newRow("7b") << mPrimaryIntMatrix << MatrixPoint<int>{4, 4} << MatrixPoint<int>{4, 4} << MatrixPoint<int>{5, 5} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{4, 4} << MatrixPoint<int>{4, 4} << MatrixSizeType<int>{0};
    QTest::newRow("7c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{0};
    QTest::newRow("7d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 2} << isNegativeInt << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{0};

    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};
    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};
    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};
    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};

    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 0} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 0} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};

    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 8} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 8} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 8} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_InputMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};

    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{0, 1} << MatrixPoint<int>{1, 5} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};
    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{1, 5} << MatrixPoint<int>{0, 1} << MatrixPoint<int>{0, 3} << isNegativeInt << c_InputMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};
    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{-1, 1} << MatrixPoint<int>{1, 5} << MatrixPoint<int>{1, 3} << isNegativeInt << c_InputMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};
    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{1, 5} << MatrixPoint<int>{-1, 1} << MatrixPoint<int>{1, 3} << isNegativeInt << c_InputMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};

    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{-1, 2} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 2} << MatrixPoint<int>{0, 2} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 2} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 2} << MatrixPoint<int>{0, 2} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{0, 3} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 3} << MatrixPoint<int>{0, 3} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 3} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 3} << MatrixPoint<int>{0, 3} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{1, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 4} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{1, 4} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 0} << MatrixPoint<int>{1, 4} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{1, 4} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{0, 1} << MatrixPoint<int>{0, 5} << MatrixPoint<int>{0, 3} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 3} << MatrixPoint<int>{0, 3} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{0, 5} << MatrixPoint<int>{0, 1} << MatrixPoint<int>{0, 3} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 3} << MatrixPoint<int>{0, 3} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixPoint<int>{0, 3} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 3} << MatrixPoint<int>{0, 3} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << mSecondaryIntMatrix << MatrixPoint<int>{1, 4} << MatrixPoint<int>{0, 2} << MatrixPoint<int>{0, 3} << isNegativeInt << mSecondaryIntMatrix << MatrixPoint<int>{0, 3} << MatrixPoint<int>{0, 3} << MatrixSizeType<int>{0};

    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{4, 0} << isNegativeInt << c_InputMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 0} << isNegativeInt << c_InputMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{4, 0} << isNegativeInt << c_InputMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{4, 0} << isNegativeInt << c_InputMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};

    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, 1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_InputMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};

    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 0} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, -1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, 0} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 0} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, -1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, -1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{8, 1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, 1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, -1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, -1} << isNegativeInt << c_InputMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};

    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{2, 0} << MatrixPoint<int>{7, 1} << MatrixPoint<int>{5, 0} << isNegativeInt << c_InputMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};
    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{7, 1} << MatrixPoint<int>{2, 0} << MatrixPoint<int>{5, 0} << isNegativeInt << c_InputMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};
    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{2, -1} << MatrixPoint<int>{7, 1} << MatrixPoint<int>{5, 1} << isNegativeInt << c_InputMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};
    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{7, 1} << MatrixPoint<int>{2, -1} << MatrixPoint<int>{5, 1} << isNegativeInt << c_InputMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};

    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{9, -1} << MatrixPoint<int>{2, -1} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{2, 0} << MatrixPoint<int>{2, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{9, -1} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{2, -1} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{2, 0} << MatrixPoint<int>{2, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 0} << MatrixPoint<int>{3, 0} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{3, 0} << MatrixPoint<int>{3, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{9, 0} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{3, 0} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{3, 0} << MatrixPoint<int>{3, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{0, 1} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{4, 1} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{4, 1} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 1} << MatrixPoint<int>{4, 1} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{4, 1} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{1, 0} << MatrixPoint<int>{5, 0} << MatrixPoint<int>{3, 0} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{3, 0} << MatrixPoint<int>{3, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{5, 0} << MatrixPoint<int>{1, 0} << MatrixPoint<int>{3, 0} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{3, 0} << MatrixPoint<int>{3, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixPoint<int>{5, 0} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{5, 0} << MatrixPoint<int>{5, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9e") << mThirdIntMatrix << MatrixPoint<int>{6, 1} << MatrixPoint<int>{4, 0} << MatrixPoint<int>{5, 0} << isNegativeInt << mThirdIntMatrix << MatrixPoint<int>{5, 0} << MatrixPoint<int>{5, 0} << MatrixSizeType<int>{0};

    QTest::newRow("10a") << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("10a") << IntMatrix{1, 1, -2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("10b") << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("10b") << IntMatrix{1, 1, -2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("10c") << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("10c") << IntMatrix{1, 1, -2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("10d") << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("10d") << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("10e") << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{0};
    QTest::newRow("10e") << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{0};
    QTest::newRow("10f") << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("10f") << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};

    QTest::newRow("11a") << IntMatrix{} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("11b") << IntMatrix{} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << IntMatrix{} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
}

QTEST_APPLESS_MAIN(CombinedSTLTests)

#include "tst_combinedstltests.moc"
