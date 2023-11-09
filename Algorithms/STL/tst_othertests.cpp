#include <QtTest>

#include <algorithm>

#include "datautils.h"

class OtherTests : public QObject
{
    Q_OBJECT

public:
    OtherTests();

private slots:
    void testGatherAlgorithmStdVector();
    void testGatherAlgorithmMatrix();

    void testGatherAlgorithmStdVector_data();
    void testGatherAlgorithmMatrix_data();

private:
    template<typename BidirectionalIt, typename Predicate>
    std::pair<BidirectionalIt, BidirectionalIt> _gatherSequenceElements(BidirectionalIt startIt, BidirectionalIt endIt, BidirectionalIt gatheringPointIt, Predicate predicate);

    template<typename DataType>
    std::tuple<MatrixPoint<DataType>,
               MatrixPoint<DataType>,
               MatrixSizeType<DataType>> _gatherMatrixElements(Matrix<DataType>& matrix,
                                                               MatrixPoint<DataType> startingPoint,
                                                               MatrixPoint<DataType> endingPoint,
                                                               MatrixPoint<DataType> gatheringPoint,
                                                               std::function<bool(const DataType&)>& predicate);

    const IntMatrix mPrimaryIntMatrix;
    const IntVector mPrimaryIntVector;
    const IntVector mSecondaryIntVector;
    const IntVector mThirdIntVector;
    const IntVector mFourthIntVector;
    const IntVector mFifthIntVector;
};

OtherTests::OtherTests()
    : mPrimaryIntMatrix{8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                                4, 20, -7,   2,   2,  2,  10, -5,  -2,
                                5, -5,  2,  -12, -2,  6,   7,  8,  -9,
                               -6,  4,  14, -5,   0,  0,  12,  14, -7,
                               -3,  3, -3,   4,  19,  15, 11,  0,  -9,
                                8, -8, -14,  8,  17, -14, -12, 4,   2,
                                5,  5,  5,   5,   4,  3,   2, -10,  4,
                               48,  2, -3,  -4,  -5,  6,  -7,  18,  0
                       }}
    , mPrimaryIntVector{2, -1, 5, -8, -7, 0, 3, 4, -5, 10, 8, -9, 0, 1}
    , mSecondaryIntVector{5, 14}
    , mThirdIntVector{-5, 14}
    , mFourthIntVector{5, -14}
    , mFifthIntVector{-5, -14}
{
}

void OtherTests::testGatherAlgorithmStdVector()
{
    QFETCH(IntVector, inputVector);
    QFETCH(IntVector::difference_type, beginIteratorOffset);
    QFETCH(IntVector::difference_type, endIteratorOffset);
    QFETCH(IntVector::difference_type, gatheringPointIteratorOffset);
    QFETCH(std::function<bool(const int&)>, predicate);
    QFETCH(IntVector, inputVectorRef);
    QFETCH(IntVector::difference_type, gatheringStartIteratorOffset);
    QFETCH(IntVector::difference_type, gatheringEndIteratorOffset);

    const auto&[gatheringStartIt, gatheringEndIt]{_gatherSequenceElements(inputVector.begin() + beginIteratorOffset,
                                                                          inputVector.begin() + endIteratorOffset,
                                                                          inputVector.begin() + gatheringPointIteratorOffset,
                                                                          predicate)};

    QVERIFY(std::equal(inputVector.cbegin(), inputVector.cend(), inputVectorRef.cbegin()));
    QVERIFY(gatheringStartIt == inputVector.begin() + gatheringStartIteratorOffset && gatheringEndIt == inputVector.begin() + gatheringEndIteratorOffset);
}

void OtherTests::testGatherAlgorithmMatrix()
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

    const auto&[gatheringStartingPoint, gatheringEndingPoint, gatheredElementsCount]{_gatherMatrixElements(inputMatrix, startingPoint, endingPoint, gatheringPoint, predicate)};

    QVERIFY(inputMatrixRef == inputMatrix);
    QVERIFY(gatheringStartingPoint == gatheringStartingPointRef && gatheringEndingPoint == gatheringEndingPointRef && gatheredElementsCount == gatheredElementsCountRef);
}

void OtherTests::testGatherAlgorithmStdVector_data()
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

void OtherTests::testGatherAlgorithmMatrix_data()
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


    IntMatrix inputMatrixRef{8, 9, { 4,  20, -5, -1,  0,   8,  12,  9,   9,
                                     4, -1,  -7, -5, -2,   2,  2,   2,  10,
                                     5, -5, -12, -2, -9,   2,  6,   7,   8,
                                     4, -6, -5,  -7, -12, -7,  0,  12,  14,
                                     3, -3, -3,  -9,  -5,  0, 15,  11,   0,
                                     8, -8, -14, -14, 14,  19, 17,  4,   2,
                                     5,  5, -10,  -4,  4,  8,  3,   2,   4,
                                     48, 2, -3,   5,  5,   4,  6,  18,   0
                             }};

    std::function<bool(const int&)> isNegativeInt{[](const int& element) {return element < 0;}};

    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                              4, 20, -7,   2,   2,  2,  10, -5,  -2,
                              5, -5,  2,  -12, -2,  -9,  6,  7,   8,
                             -6,  4,  14,  -4,  -5, -7, -12, 12,  14,
                             -3,  3, -3,   0,  -5,  -9,  0,  11,  0,
                              8, -8, -14,  4,  19, -14,  15,  4,  2,
                              5,  5,  5,   8,  17, -10,  3,   2,  4,
                              48,  2, -3,  5,  4,  -7,   6,  18,  0
                      }};

    // input bounding rectangle: (2, 3) -> (8, 9): all elements excluding the left/bottom edges
    QTest::newRow("2a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 5} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 7} << MatrixSizeType<int>{12};
    QTest::newRow("2a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{3, 5} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 7} << MatrixSizeType<int>{12};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                              4, 20, -7,   2,   2,  2,  10, -5,  -2,
                              5,  4,  2,  -12, -2,  6,   7,  8,  -9,
                             -6, -5,  14, -5,   0,  0,  12,  14, -7,
                             -3, -3,  3,   4,  19,  15, 11,  0,  -9,
                              8, -8, -14,  8,  17, -14, -12, 4,   2,
                              5, -3,  5,   5,   4,  3,   2, -10,  4,
                             48,  5,  2,  -4,  -5,  6,  -7,  18,  0
                      }};

    // input bounding rectangle: (2, 0) -> (8, 3): all elements excluding the left/bottom edges
    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{5, 1} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{5, 1} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,   8,  -5, -1,   12,  9,   9,
                              4, 20, -7,   2,   2,  2,   -5, -2,  10,
                              5, -5,  2,  -12, -2,  6,    7,  8,  -9,
                             -6,  4,  14, -5,   0,  0,   12,  14, -7,
                             -3,  3, -3,   4,  19,  15,  11,  0,  -9,
                              8, -8, -14,  8,  17, -14, -12,  4,   2,
                              5,  5,  5,   5,   4,  3,   2, -10,   4,
                             48,  2, -3,  -4,  -5,  6,  -7,  18,   0
                      }};

    // input bounding rectangle: (0, 3) -> (2, 9): all elements excluding the left/bottom edges
    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{1, 6} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 6} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};

    inputMatrixRef = {8, 9, { 4, -1,  0,  -5,  -1,  8,   12,  9,   9,
                              4, 20, -7,   2,   2,  2,   10, -5,  -2,
                              5, -5,  2,  -12, -2,  6,    7,  8,  -9,
                             -6,  4,  14, -5,   0,  0,   12,  14, -7,
                             -3,  3, -3,   4,  19,  15,  11,  0,  -9,
                              8, -8, -14,  8,  17, -14, -12, 4,    2,
                              5,  5,  5,   5,   4,  3,   2, -10,   4,
                             48,  2, -3,  -4,  -5,  6,  -7,  18,   0
                     }};

    // input bounding rectangle: (0, 0) -> (2, 3): all elements excluding the left/bottom edges
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,   8,  12,   9,   9,
                              4, 20, -7,   2,   2,   2,  10,  -5,  -2,
                              5, -5,  2,  -12, -2,   6,   7,   8,  -9,
                             -6,  4,  14, -5,  -14, -12, 12,  14,  -7,
                             -3,  3,  4,  -3,   0,   0,   11,  0,  -9,
                              8, -8,  8, -14,  19,   15,  17,  4,   2,
                              5,  5,  5,   5,   4,   3,   2, -10,   4,
                             48,  2, -3,  -4,  -5,   6,  -7,  18,   0
                     }};

    QTest::newRow("3a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 4} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 6} << MatrixSizeType<int>{7};
    QTest::newRow("3a") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 4} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 6} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,  9,
                              4, 20, -7,   2,   2,  2,  10, -5, -2,
                              5, -5, -3,  -12, -2,  6,  7,   8, -9,
                             -6,  4, -14, -5, -12,  0,  12, 14, -7,
                             -3,  3,  2,  -14,  0,  15, 11,  0, -9,
                              8, -8,  14,  4,  19,  8,  17,  4,  2,
                              5,  5,  5,   5,   4,  3,   2, -10, 4,
                             48,  2, -3,  -4,  -5,  6,  -7,  18, 0
                     }};

    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 3} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{0, 3} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 3} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 3} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,   8,   12,  9,   9,
                              4, 20, -7,   2,   2,   2,   10, -5,  -2,
                              5, -5,  2,   0,  -2,   6,    7,  8,  -9,
                             -6,  4,  14,  19, -5,   0,   12,  14, -7,
                             -3,  3,  4,   17, -3,   15,  11,  0,  -9,
                              8, -8,  8,  -12, -14, -14, -12,  4,   2,
                              5,  5,  5,   5,   4,   3,    2, -10,  4,
                             48,  2, -3,  -4,  -5,   6,   -7,  18,  0
                     }};

    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 5} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 5} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 5} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                              4, 20, -7,   2,   2,  2,  10, -5,  -2,
                              5, -5, -12, 14,   2,  6,   7,  8,  -9,
                             -6,  4, -5,  -2,   0,  0,  12,  14, -7,
                             -3,  3, -3,  -14, -12, 15, 11,  0,  -9,
                              8, -8, -14,  4,  19,  8,  17,  4,   2,
                              5,  5,  5,   5,   4,  3,   2, -10,  4,
                             48,  2, -3,  -4,  -5,  6,  -7,  18,  0
                     }};

    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{4, 0} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{4, 0} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{4, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{4, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                              4, 20, -7,   2,   2,  2,  10, -5,  -2,
                              5, -5,  2,   6,   7, -12, -2,  8,  -9,
                             -6,  4,  14,  0, -14, -14, -5,  14, -7,
                             -3,  3,  4,  19,   0,  12, -3,  0,  -9,
                              8, -8,  8,  17,  15,  11, -12, 4,   2,
                              5,  5,  5,   5,   4,  3,   2, -10,  4,
                             48,  2, -3,  -4,  -5,  6,  -7,  18,  0
                     }};

    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 8} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 8} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 7} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 7} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                              4, 20, -7,   2,   2,  2,  10, -5,  -2,
                              5, -5, -12, -2, -12,  6,   7,  8,  -9,
                             -6,  4, -5, -14,   2,  0,  12,  14, -7,
                             -3,  3, -3,  14,   0,  15, 11,  0,  -9,
                              8, -8, -14,  4,  19,  8,  17,  4,   2,
                              5,  5,  5,   5,   4,  3,   2, -10,  4,
                             48,  2, -3,  -4,  -5,  6,  -7,  18,  0
                     }};

    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 0} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{1, 0} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,   8,   12,  9,   9,
                              4, 20, -7,   2,   2,   2,   10, -5,  -2,
                              5, -5,  2,   6,  -14, -12,  -2,  8,  -9,
                             -6,  4,  14,  0,   7,  -14,  -5,  14, -7,
                             -3,  3,   4,  19,  0,   12,  -3,  0,  -9,
                              8, -8,   8,  17, 15,   11, -12,  4,   2,
                              5,  5,  5,   5,   4,   3,    2, -10,  4,
                             48,  2, -3,  -4,  -5,   6,   -7,  18,  0
                     }};

    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 8} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{0, 8} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 7} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 7} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,   -1,  8,  12,  9,   9,
                              4, 20, -7,   2,    2,  2,  10, -5,  -2,
                              5, -5, -12, 14,    2,  6,   7,  8,  -9,
                             -6,  4, -5,   4,    0,  0,  12,  14, -7,
                             -3,  3, -3,  -2,   19,  15, 11,  0,  -9,
                              8, -8, -14, -14, -12,  8,  17,  4,   2,
                              5,  5,  5,   5,    4,  3,   2, -10,  4,
                             48,  2, -3,  -4,   -5,  6,  -7,  18,  0
                     }};

    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 1} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 1} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 2} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    inputMatrixRef = {8, 9, {-1,  4,  0,  -5,  -1,  8,  12,  9,   9,
                              4, 20, -7,   2,   2,  2,  10, -5,  -2,
                              5, -5,  2,   6,   7,  12, -2,  8,  -9,
                             -6,  4,  14,  0,   0,  11, -5,  14, -7,
                             -3,  3,  4,  19,  15, -12, -3,  0,  -9,
                              8, -8,  8,  17, -14, -14, -12, 4,   2,
                              5,  5,  5,   5,   4,  3,   2, -10,  4,
                             48,  2, -3,  -4,  -5,  6,  -7,  18,  0
                     }};

    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 8} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 8} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 7} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << isNegativeInt << inputMatrixRef << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

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

    QTest::newRow("8a") << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("8a") << IntMatrix{1, 1, -2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("8b") << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("8b") << IntMatrix{1, 1, -2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("8c") << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("8c") << IntMatrix{1, 1, -2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, -2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{1};
    QTest::newRow("8d") << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("8d") << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{0};
    QTest::newRow("8e") << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{1, 1} << MatrixSizeType<int>{0};
    QTest::newRow("8f") << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("8f") << IntMatrix{1, 1, 2} << MatrixPoint<int>{1, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << IntMatrix{1, 1, 2} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};

    QTest::newRow("9a") << IntMatrix{} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << IntMatrix{} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
    QTest::newRow("9b") << IntMatrix{} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << IntMatrix{} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << MatrixSizeType<int>{0};
}

template<typename BidirectionalIt, typename Predicate>
std::pair<BidirectionalIt, BidirectionalIt> OtherTests::_gatherSequenceElements(BidirectionalIt startIt, BidirectionalIt endIt, BidirectionalIt gatheringPointIt, Predicate predicate)
{
    BidirectionalIt gatheringStartIt{startIt};
    BidirectionalIt gatheringEndIt{startIt};

    if (std::distance(startIt, endIt) > 0)
    {
        gatheringPointIt = std::clamp<BidirectionalIt>(gatheringPointIt, startIt, endIt,
                                                          [](const BidirectionalIt& first, const BidirectionalIt& second) {return std::distance(first, second) > 0;});
        gatheringStartIt = std::stable_partition(startIt, gatheringPointIt, [predicate](const auto& element) {return !predicate(element);});
        gatheringEndIt = std::stable_partition(gatheringPointIt, endIt, predicate);
    }

    return {gatheringStartIt, gatheringEndIt};
}

template<typename DataType>
std::tuple<MatrixPoint<DataType>,
           MatrixPoint<DataType>,
           MatrixSizeType<DataType>> OtherTests::_gatherMatrixElements(Matrix<DataType>& matrix,
                                                                       MatrixPoint<DataType> startingPoint,
                                                                       MatrixPoint<DataType> endingPoint,
                                                                       MatrixPoint<DataType> gatheringPoint,
                                                                       std::function<bool (const DataType &)>& predicate)
{
    MatrixPoint<DataType> gatheringStartingPoint{0, 0};
    MatrixPoint<DataType> gatheringEndingPoint{0, 0};
    MatrixSizeType<DataType> gatheredElementsCount{0};

    if (!matrix.isEmpty())
    {
        startingPoint.first = std::clamp(startingPoint.first, 0, matrix.getNrOfRows());
        startingPoint.second = std::clamp(startingPoint.second, 0, matrix.getNrOfColumns());
        endingPoint.first = std::clamp(endingPoint.first, 0, matrix.getNrOfRows());
        endingPoint.second = std::clamp(endingPoint.second, 0, matrix.getNrOfColumns());

        // get the input bounding rectangle
        MatrixSizeType<DataType> const c_StartingRowNr{std::min(startingPoint.first, endingPoint.first)};
        MatrixSizeType<DataType> const c_EndingRowNr{std::max(startingPoint.first, endingPoint.first)};
        MatrixSizeType<DataType> const c_StartingColumnNr{std::min(startingPoint.second, endingPoint.second)};
        MatrixSizeType<DataType> const c_EndingColumnNr{std::max(startingPoint.second, endingPoint.second)};

        gatheringPoint.first = std::clamp(gatheringPoint.first, c_StartingRowNr, c_EndingRowNr);
        gatheringPoint.second = std::clamp(gatheringPoint.second, c_StartingColumnNr, c_EndingColumnNr);

        // start from the opposite end of each interval in order to determine the min/max row/column numbers that limit the gathered elements
        MatrixSizeType<DataType> minGatheringRowNr{c_EndingRowNr};
        MatrixSizeType<DataType> maxGatheringRowNr{c_StartingRowNr};
        MatrixSizeType<DataType> minGatheringColumnNr{c_EndingColumnNr};
        MatrixSizeType<DataType> maxGatheringColumnNr{c_StartingColumnNr};

        for(auto rowNr{c_StartingRowNr}; rowNr < c_EndingRowNr; ++rowNr)
        {
            auto rowRangeStartIt{c_StartingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_StartingColumnNr)};
            auto rowRangeEndIt{c_EndingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_EndingColumnNr)};
            auto rowGatheringPointIt{gatheringPoint.second == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, gatheringPoint.second)};

            const auto&[gatheringRowStartIt, gatheringRowEndIt]{_gatherSequenceElements(rowRangeStartIt, rowRangeEndIt, rowGatheringPointIt, predicate)};
            minGatheringColumnNr = std::min(minGatheringColumnNr, gatheringRowStartIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : gatheringRowStartIt.getColumnNr());
            maxGatheringColumnNr = std::max(maxGatheringColumnNr, gatheringRowEndIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : gatheringRowEndIt.getColumnNr());
            gatheredElementsCount += std::distance(gatheringRowStartIt, gatheringRowEndIt);
        }

        for(auto columnNr{c_StartingColumnNr}; columnNr < c_EndingColumnNr; ++columnNr)
        {
            auto columnRangeStartIt{c_StartingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_StartingRowNr, columnNr)};
            auto columnRangeEndIt{c_EndingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_EndingRowNr, columnNr)};
            auto columnGatheringPointIt{gatheringPoint.first == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(gatheringPoint.first, columnNr)};

            const auto&[gatheringColumnStartIt, gatheringColumnEndIt]{_gatherSequenceElements(columnRangeStartIt, columnRangeEndIt, columnGatheringPointIt, predicate)};
            minGatheringRowNr = std::min(minGatheringRowNr, gatheringColumnStartIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : gatheringColumnStartIt.getRowNr());
            maxGatheringRowNr = std::max(maxGatheringRowNr, gatheringColumnEndIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : gatheringColumnEndIt.getRowNr());
        }

        // determine the output bounding rectangle (should be the gathering point if the input bounding rectangle is a row, column or single element)
        if (minGatheringRowNr != maxGatheringRowNr && minGatheringColumnNr != maxGatheringColumnNr)
        {
            gatheringStartingPoint = {minGatheringRowNr, minGatheringColumnNr};
            gatheringEndingPoint = {maxGatheringRowNr, maxGatheringColumnNr};
        }
        else
        {
            gatheringStartingPoint = gatheringPoint;
            gatheringEndingPoint = gatheringPoint;
        }
    }

    return {gatheringStartingPoint, gatheringEndingPoint, gatheredElementsCount};
}

QTEST_APPLESS_MAIN(OtherTests)

#include "tst_othertests.moc"
