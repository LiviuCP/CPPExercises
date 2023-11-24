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
    void testGatherAlgorithmStdList();
    void testGatherAlgorithmMatrix();

    void testGatherAlgorithmStdVector_data();
    void testGatherAlgorithmStdList_data();
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
    const IntList mPrimaryIntList;
    const IntList mSecondaryIntList;
    const IntList mThirdIntList;
    const IntList mFourthIntList;
    const IntList mFifthIntList;
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
    , mPrimaryIntList{2, -1, 5, -8, -7, 0, 3, 4, -5, 10, 8, -9, 0, 1}
    , mSecondaryIntList{5, 14}
    , mThirdIntList{-5, 14}
    , mFourthIntList{5, -14}
    , mFifthIntList{-5, -14}
{
}

void CombinedSTLTests::testGatherAlgorithmStdVector()
{
    QFETCH(IntVector, intVector);
    QFETCH(IntVector::difference_type, beginIteratorOffset);
    QFETCH(IntVector::difference_type, endIteratorOffset);
    QFETCH(IntVector::difference_type, gatheringPointIteratorOffset);
    QFETCH(std::function<bool(const int&)>, predicate);
    QFETCH(IntVector, intVectorRef);
    QFETCH(IntVector::difference_type, gatheringStartIteratorOffset);
    QFETCH(IntVector::difference_type, gatheringEndIteratorOffset);

    const auto&[gatheringStartIt, gatheringEndIt]{gatherSequenceElements(intVector.begin() + beginIteratorOffset,
                                                                         intVector.begin() + endIteratorOffset,
                                                                         intVector.begin() + gatheringPointIteratorOffset,
                                                                         predicate)};

    QVERIFY(std::equal(intVector.cbegin(), intVector.cend(), intVectorRef.cbegin()));
    QVERIFY(gatheringStartIt == intVector.begin() + gatheringStartIteratorOffset && gatheringEndIt == intVector.begin() + gatheringEndIteratorOffset);
}

void CombinedSTLTests::testGatherAlgorithmStdList()
{
    QFETCH(IntList, intList);
    QFETCH(size_t, startIteratorIndex);
    QFETCH(size_t, endIteratorIndex);
    QFETCH(size_t, gatheringPointIteratorIndex);
    QFETCH(std::function<bool(const int&)>, predicate);
    QFETCH(IntList, intListRef);
    QFETCH(size_t, gatheringStartIteratorIndex);
    QFETCH(size_t, gatheringEndIteratorIndex);

    const auto&[gatheringStartIt, gatheringEndIt]{gatherSequenceElements(Utilities::getListIteratorAtIndex(intList, startIteratorIndex),
                                                                         Utilities::getListIteratorAtIndex(intList, endIteratorIndex),
                                                                         Utilities::getListIteratorAtIndex(intList, gatheringPointIteratorIndex),
                                                                         predicate)};

    QVERIFY(std::equal(intList.cbegin(), intList.cend(), intListRef.cbegin()));
    QVERIFY(gatheringStartIt == Utilities::getListIteratorAtIndex(intList, gatheringStartIteratorIndex) &&
            gatheringEndIt == Utilities::getListIteratorAtIndex(intList, gatheringEndIteratorIndex));
}

void CombinedSTLTests::testGatherAlgorithmMatrix()
{
    QFETCH(IntMatrix, intMatrix);
    QFETCH(MatrixPoint<int>, startingPoint);
    QFETCH(MatrixPoint<int>, endingPoint);
    QFETCH(MatrixPoint<int>, gatheringPoint);
    QFETCH(std::function<bool(const int&)>, predicate);
    QFETCH(IntMatrix, intMatrixRef);
    QFETCH(MatrixPoint<int>, gatheringStartingPointRef);
    QFETCH(MatrixPoint<int>, gatheringEndingPointRef);
    QFETCH(MatrixSizeType<int>, gatheredElementsCountRef);

    const auto&[gatheringStartingPoint, gatheringEndingPoint, gatheredElementsCount]{gatherMatrixElements(intMatrix, startingPoint, endingPoint, gatheringPoint, predicate)};

    QVERIFY(intMatrixRef == intMatrix);
    QVERIFY(gatheringStartingPoint == gatheringStartingPointRef && gatheringEndingPoint == gatheringEndingPointRef && gatheredElementsCount == gatheredElementsCountRef);
}

void CombinedSTLTests::testGatherAlgorithmStdVector_data()
{
    QTest::addColumn<IntVector>("intVector");
    QTest::addColumn<IntVector::difference_type>("beginIteratorOffset");
    QTest::addColumn<IntVector::difference_type>("endIteratorOffset");
    QTest::addColumn<IntVector::difference_type>("gatheringPointIteratorOffset");
    QTest::addColumn<std::function<bool(const int&)>>("predicate");
    QTest::addColumn<IntVector>("intVectorRef");
    QTest::addColumn<IntVector::difference_type>("gatheringStartIteratorOffset");
    QTest::addColumn<IntVector::difference_type>("gatheringEndIteratorOffset");

    std::function<bool(const int&)> isNegativeInt{[](const int& element) {return element < 0;}};

    QTest::newRow("1") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{14} << IntVectorDiff{0} << isNegativeInt << c_IntVectorRef2 << IntVectorDiff{0} << IntVectorDiff{5};
    QTest::newRow("2") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{14} << IntVectorDiff{4} << isNegativeInt << c_IntVectorRef1 << IntVectorDiff{2} << IntVectorDiff{7};
    QTest::newRow("3") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{14} << IntVectorDiff{14} << isNegativeInt << c_IntVectorRef3 << IntVectorDiff{9} << IntVectorDiff{14};
    QTest::newRow("4") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{0} << isNegativeInt << c_IntVectorRef5_6_7 << IntVectorDiff{2} << IntVectorDiff{5};
    QTest::newRow("5") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{1} << isNegativeInt << c_IntVectorRef5_6_7 << IntVectorDiff{2} << IntVectorDiff{5};
    QTest::newRow("6") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{2} << isNegativeInt << c_IntVectorRef5_6_7 << IntVectorDiff{2} << IntVectorDiff{5};
    QTest::newRow("7") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{5} << isNegativeInt << c_IntVectorRef4 << IntVectorDiff{3} << IntVectorDiff{6};
    QTest::newRow("8") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{10} << isNegativeInt << c_IntVectorRef8_9_10 << IntVectorDiff{7} << IntVectorDiff{10};
    QTest::newRow("9") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{11} << isNegativeInt << c_IntVectorRef8_9_10 << IntVectorDiff{7} << IntVectorDiff{10};
    QTest::newRow("10") << mPrimaryIntVector << IntVectorDiff{2} << IntVectorDiff{10} << IntVectorDiff{13} << isNegativeInt << c_IntVectorRef8_9_10 << IntVectorDiff{7} << IntVectorDiff{10};
    QTest::newRow("11") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{0} << IntVectorDiff{0} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{0};
    QTest::newRow("12") << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{0} << IntVectorDiff{1} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{0} << IntVectorDiff{0};
    QTest::newRow("13") << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4} << IntVectorDiff{3} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4};
    QTest::newRow("14") << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4} << IntVectorDiff{4} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4};
    QTest::newRow("15") << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4} << IntVectorDiff{5} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{4} << IntVectorDiff{4};
    QTest::newRow("16") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14} << IntVectorDiff{0} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("17") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14} << IntVectorDiff{7} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("18") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14} << IntVectorDiff{14} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("19") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{0} << IntVectorDiff{0} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("20") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{0} << IntVectorDiff{5} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("21") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{0} << IntVectorDiff{14} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("22") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{0} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("23") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{1} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("24") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{3} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("25") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{5} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("26") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{7} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("27") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{9} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("28") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{3} << IntVectorDiff{14} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("29") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{3} << IntVectorDiff{0} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("30") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{3} << IntVectorDiff{1} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("31") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{3} << IntVectorDiff{3} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("32") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{3} << IntVectorDiff{5} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("33") << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{3} << IntVectorDiff{14} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{14} << IntVectorDiff{14};
    QTest::newRow("34") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{0} << IntVectorDiff{0} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("35") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{0} << IntVectorDiff{1} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("36") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{0} << IntVectorDiff{3} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("37") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{0} << IntVectorDiff{7} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("38") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{0} << IntVectorDiff{9} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("39") << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{0} << IntVectorDiff{14} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{7} << IntVectorDiff{7};
    QTest::newRow("40") << mPrimaryIntVector << IntVectorDiff{5} << IntVectorDiff{8} << IntVectorDiff{6} << isNegativeInt << mPrimaryIntVector << IntVectorDiff{6} << IntVectorDiff{6};
    QTest::newRow("41") << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{0};
    QTest::newRow("42") << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mSecondaryIntVector << IntVectorDiff{1} << IntVectorDiff{1};
    QTest::newRow("43") << mSecondaryIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << mSecondaryIntVector << IntVectorDiff{2} << IntVectorDiff{2};
    QTest::newRow("44") << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("45") << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("46") << mThirdIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << IntVector{14, -5} << IntVectorDiff{1} << IntVectorDiff{2};
    QTest::newRow("47") << mFourthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << IntVector{-14, 5} << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("48") << mFourthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mFourthIntVector << IntVectorDiff{1} << IntVectorDiff{2};
    QTest::newRow("49") << mFourthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << mFourthIntVector << IntVectorDiff{1} << IntVectorDiff{2};
    QTest::newRow("50") << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{0} << isNegativeInt << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2};
    QTest::newRow("51") << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{1} << isNegativeInt << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2};
    QTest::newRow("52") << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2} << IntVectorDiff{2} << isNegativeInt << mFifthIntVector << IntVectorDiff{0} << IntVectorDiff{2};
    QTest::newRow("53") << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{0} << isNegativeInt << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("54") << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{1} << isNegativeInt << IntVector{-5} << IntVectorDiff{0} << IntVectorDiff{1};
    QTest::newRow("55") << IntVector{5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{0} << isNegativeInt << IntVector{5} << IntVectorDiff{0} << IntVectorDiff{0};
    QTest::newRow("56") << IntVector{5} << IntVectorDiff{0} << IntVectorDiff{1} << IntVectorDiff{1} << isNegativeInt << IntVector{5} << IntVectorDiff{1} << IntVectorDiff{1};
    QTest::newRow("57") << IntVector{} << IntVectorDiff{0} << IntVectorDiff{0} << IntVectorDiff{0} << isNegativeInt << IntVector{} << IntVectorDiff{0} << IntVectorDiff{0};
}

void CombinedSTLTests::testGatherAlgorithmStdList_data()
{
    QTest::addColumn<IntList>("intList");
    QTest::addColumn<size_t>("startIteratorIndex");
    QTest::addColumn<size_t>("endIteratorIndex");
    QTest::addColumn<size_t>("gatheringPointIteratorIndex");
    QTest::addColumn<std::function<bool(const int&)>>("predicate");
    QTest::addColumn<IntList>("intListRef");
    QTest::addColumn<size_t>("gatheringStartIteratorIndex");
    QTest::addColumn<size_t>("gatheringEndIteratorIndex");

    std::function<bool(const int&)> isNegativeInt{[](const int& element) {return element < 0;}};

    // same test data used as for std::vector (hence same row names), data entries that were not applicable to std::list have been skipped
    QTest::newRow("1") << mPrimaryIntList << size_t{0} << size_t{14} << size_t{0} << isNegativeInt << c_IntListRef2 << size_t{0} << size_t{5};
    QTest::newRow("2") << mPrimaryIntList << size_t{0} << size_t{14} << size_t{4} << isNegativeInt << c_IntListRef1 << size_t{2} << size_t{7};
    QTest::newRow("3") << mPrimaryIntList << size_t{0} << size_t{14} << size_t{14} << isNegativeInt << c_IntListRef3 << size_t{9} << size_t{14};
    QTest::newRow("6") << mPrimaryIntList << size_t{2} << size_t{10} << size_t{2} << isNegativeInt << c_IntListRef5 << size_t{2} << size_t{5};
    QTest::newRow("7") << mPrimaryIntList << size_t{2} << size_t{10} << size_t{5} << isNegativeInt << c_IntListRef4 << size_t{3} << size_t{6};
    QTest::newRow("8") << mPrimaryIntList << size_t{2} << size_t{10} << size_t{10} << isNegativeInt << c_IntListRef8 << size_t{7} << size_t{10};
    QTest::newRow("11") << mPrimaryIntList << size_t{0} << size_t{0} << size_t{0} << isNegativeInt << mPrimaryIntList << size_t{0} << size_t{0};
    QTest::newRow("14") << mPrimaryIntList << size_t{4} << size_t{4} << size_t{4} << isNegativeInt << mPrimaryIntList << size_t{4} << size_t{4};
    QTest::newRow("18") << mPrimaryIntList << size_t{14} << size_t{14} << size_t{14} << isNegativeInt << mPrimaryIntList << size_t{14} << size_t{14};
    QTest::newRow("40") << mPrimaryIntList << size_t{5} << size_t{8} << size_t{6} << isNegativeInt << mPrimaryIntList << size_t{6} << size_t{6};
    QTest::newRow("41") << mSecondaryIntList << size_t{0} << size_t{2} << size_t{0} << isNegativeInt << mSecondaryIntList << size_t{0} << size_t{0};
    QTest::newRow("42") << mSecondaryIntList << size_t{0} << size_t{2} << size_t{1} << isNegativeInt << mSecondaryIntList << size_t{1} << size_t{1};
    QTest::newRow("43") << mSecondaryIntList << size_t{0} << size_t{2} << size_t{2} << isNegativeInt << mSecondaryIntList << size_t{2} << size_t{2};
    QTest::newRow("44") << mThirdIntList << size_t{0} << size_t{2} << size_t{0} << isNegativeInt << mThirdIntList << size_t{0} << size_t{1};
    QTest::newRow("45") << mThirdIntList << size_t{0} << size_t{2} << size_t{1} << isNegativeInt << mThirdIntList << size_t{0} << size_t{1};
    QTest::newRow("46") << mThirdIntList << size_t{0} << size_t{2} << size_t{2} << isNegativeInt << IntList{14, -5} << size_t{1} << size_t{2};
    QTest::newRow("47") << mFourthIntList << size_t{0} << size_t{2} << size_t{0} << isNegativeInt << IntList{-14, 5} << size_t{0} << size_t{1};
    QTest::newRow("48") << mFourthIntList << size_t{0} << size_t{2} << size_t{1} << isNegativeInt << mFourthIntList << size_t{1} << size_t{2};
    QTest::newRow("49") << mFourthIntList << size_t{0} << size_t{2} << size_t{2} << isNegativeInt << mFourthIntList << size_t{1} << size_t{2};
    QTest::newRow("50") << mFifthIntList << size_t{0} << size_t{2} << size_t{0} << isNegativeInt << mFifthIntList << size_t{0} << size_t{2};
    QTest::newRow("51") << mFifthIntList << size_t{0} << size_t{2} << size_t{1} << isNegativeInt << mFifthIntList << size_t{0} << size_t{2};
    QTest::newRow("52") << mFifthIntList << size_t{0} << size_t{2} << size_t{2} << isNegativeInt << mFifthIntList << size_t{0} << size_t{2};
    QTest::newRow("53") << IntList{-5} << size_t{0} << size_t{1} << size_t{0} << isNegativeInt << IntList{-5} << size_t{0} << size_t{1};
    QTest::newRow("54") << IntList{-5} << size_t{0} << size_t{1} << size_t{1} << isNegativeInt << IntList{-5} << size_t{0} << size_t{1};
    QTest::newRow("55") << IntList{5} << size_t{0} << size_t{1} << size_t{0} << isNegativeInt << IntList{5} << size_t{0} << size_t{0};
    QTest::newRow("56") << IntList{5} << size_t{0} << size_t{1} << size_t{1} << isNegativeInt << IntList{5} << size_t{1} << size_t{1};
    QTest::newRow("57") << IntList{} << size_t{0} << size_t{0} << size_t{0} << isNegativeInt << IntList{} << size_t{0} << size_t{0};
}

void CombinedSTLTests::testGatherAlgorithmMatrix_data()
{
    QTest::addColumn<IntMatrix>("intMatrix");
    QTest::addColumn<MatrixPoint<int>>("startingPoint");
    QTest::addColumn<MatrixPoint<int>>("endingPoint");
    QTest::addColumn<MatrixPoint<int>>("gatheringPoint");
    QTest::addColumn<std::function<bool(const int&)>>("predicate");
    QTest::addColumn<IntMatrix>("intMatrixRef");
    QTest::addColumn<MatrixPoint<int>>("gatheringStartingPointRef");
    QTest::addColumn<MatrixPoint<int>>("gatheringEndingPointRef");
    QTest::addColumn<MatrixSizeType<int>>("gatheredElementsCountRef");

    std::function<bool(const int&)> isNegativeInt{[](const int& element) {return element < 0;}};

    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};
    QTest::newRow("1a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{3, 2} << isNegativeInt << c_IntMatrixRef1a << MatrixPoint<int>{0, 1} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{25};

    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef1b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};

    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1c") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef1c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1d") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef1d << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};

    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, 0} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};
    QTest::newRow("1f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, -1} << isNegativeInt << c_IntMatrixRef1ef << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 4} << MatrixSizeType<int>{25};

    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1g") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1h") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, 9} << isNegativeInt << c_IntMatrixRef1gh << MatrixPoint<int>{0, 5} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 8} << isNegativeInt << c_IntMatrixRef1i << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};
    QTest::newRow("1j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef1j << MatrixPoint<int>{0, 4} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{25};

    QTest::newRow("2a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{3, 5} << isNegativeInt << c_IntMatrixRef2a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 7} << MatrixSizeType<int>{12};
    QTest::newRow("2a") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{3, 5} << isNegativeInt << c_IntMatrixRef2a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 7} << MatrixSizeType<int>{12};

    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{5, 1} << isNegativeInt << c_IntMatrixRef2b << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{5, 1} << isNegativeInt << c_IntMatrixRef2b << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{5, 1} << isNegativeInt << c_IntMatrixRef2b << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2b") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{5, 1} << isNegativeInt << c_IntMatrixRef2b << MatrixPoint<int>{3, 0} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};

    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{1, 6} << isNegativeInt << c_IntMatrixRef2c << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 6} << isNegativeInt << c_IntMatrixRef2c << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{1, 6} << isNegativeInt << c_IntMatrixRef2c << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2c") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 6} << isNegativeInt << c_IntMatrixRef2c << MatrixPoint<int>{0, 4} << MatrixPoint<int>{2, 8} << MatrixSizeType<int>{4};

    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2d") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 2} << isNegativeInt << c_IntMatrixRef2d << MatrixPoint<int>{0, 1} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};

    QTest::newRow("2e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{5, 9} << isNegativeInt << c_IntMatrixRef2e << MatrixPoint<int>{2, 6} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{12};
    QTest::newRow("2e") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{5, 9} << isNegativeInt << c_IntMatrixRef2e << MatrixPoint<int>{2, 6} << MatrixPoint<int>{8, 9} << MatrixSizeType<int>{12};

    QTest::newRow("2f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{8, 1} << isNegativeInt << c_IntMatrixRef2f << MatrixPoint<int>{4, 0} << MatrixPoint<int>{8, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2f") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 1} << isNegativeInt << c_IntMatrixRef2f << MatrixPoint<int>{4, 0} << MatrixPoint<int>{8, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{8, 1} << isNegativeInt << c_IntMatrixRef2f << MatrixPoint<int>{4, 0} << MatrixPoint<int>{8, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2f") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 1} << isNegativeInt << c_IntMatrixRef2f << MatrixPoint<int>{4, 0} << MatrixPoint<int>{8, 3} << MatrixSizeType<int>{7};

    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{-1, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{0, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{-1, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{0, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};
    QTest::newRow("2g") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 6} << isNegativeInt << c_IntMatrixRef2g << MatrixPoint<int>{0, 4} << MatrixPoint<int>{1, 8} << MatrixSizeType<int>{4};

    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};
    QTest::newRow("2h") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef2h << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 1} << MatrixSizeType<int>{2};

    QTest::newRow("2i") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 9} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2i << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{12};
    QTest::newRow("2i") << mPrimaryIntMatrix << MatrixPoint<int>{8, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2i << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 6} << MatrixSizeType<int>{12};

    QTest::newRow("2j") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, -1} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2j << MatrixPoint<int>{2, 1} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2j") << mPrimaryIntMatrix << MatrixPoint<int>{8, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2j << MatrixPoint<int>{2, 1} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2j") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{8, 0} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2j << MatrixPoint<int>{2, 1} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};
    QTest::newRow("2j") << mPrimaryIntMatrix << MatrixPoint<int>{8, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2j << MatrixPoint<int>{2, 1} << MatrixPoint<int>{7, 3} << MatrixSizeType<int>{7};

    QTest::newRow("2k") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2k << MatrixPoint<int>{0, 3} << MatrixPoint<int>{2, 5} << MatrixSizeType<int>{4};
    QTest::newRow("2k") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2k << MatrixPoint<int>{0, 3} << MatrixPoint<int>{2, 5} << MatrixSizeType<int>{4};
    QTest::newRow("2k") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 9} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2k << MatrixPoint<int>{0, 3} << MatrixPoint<int>{2, 5} << MatrixSizeType<int>{4};
    QTest::newRow("2k") << mPrimaryIntMatrix << MatrixPoint<int>{0, 9} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2k << MatrixPoint<int>{0, 3} << MatrixPoint<int>{2, 5} << MatrixSizeType<int>{4};

    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{-1, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, -1} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{0, -1} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{2, 3} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};
    QTest::newRow("2l") << mPrimaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{2, 3} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef2l << MatrixPoint<int>{0, 2} << MatrixPoint<int>{2, 3} << MatrixSizeType<int>{2};

    QTest::newRow("3a") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 4} << isNegativeInt << c_IntMatrixRef3a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 6} << MatrixSizeType<int>{7};
    QTest::newRow("3a") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 4} << isNegativeInt << c_IntMatrixRef3a << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 6} << MatrixSizeType<int>{7};

    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3b") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 3} << isNegativeInt << c_IntMatrixRef3b << MatrixPoint<int>{2, 2} << MatrixPoint<int>{5, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 5} << isNegativeInt << c_IntMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 5} << isNegativeInt << c_IntMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 5} << isNegativeInt << c_IntMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3c") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << isNegativeInt << c_IntMatrixRef3c << MatrixPoint<int>{2, 3} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{4, 0} << isNegativeInt << c_IntMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{4, 0} << isNegativeInt << c_IntMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{4, 2} << isNegativeInt << c_IntMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3d") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{4, 2} << isNegativeInt << c_IntMatrixRef3d << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 8} << isNegativeInt << c_IntMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 8} << isNegativeInt << c_IntMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{3, 7} << isNegativeInt << c_IntMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3e") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{3, 7} << isNegativeInt << c_IntMatrixRef3e << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << isNegativeInt << c_IntMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3f") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 2} << isNegativeInt << c_IntMatrixRef3f << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 7} << isNegativeInt << c_IntMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3g") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{2, 7} << isNegativeInt << c_IntMatrixRef3g << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 1} << isNegativeInt << c_IntMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 1} << isNegativeInt << c_IntMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 2} << isNegativeInt << c_IntMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};
    QTest::newRow("3h") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 2} << isNegativeInt << c_IntMatrixRef3h << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 5} << MatrixSizeType<int>{7};

    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{7, 8} << isNegativeInt << c_IntMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << MatrixPoint<int>{6, 7} << isNegativeInt << c_IntMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};
    QTest::newRow("3i") << mPrimaryIntMatrix << MatrixPoint<int>{6, 7} << MatrixPoint<int>{2, 2} << MatrixPoint<int>{6, 7} << isNegativeInt << c_IntMatrixRef3i << MatrixPoint<int>{2, 4} << MatrixPoint<int>{6, 7} << MatrixSizeType<int>{7};

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

    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};
    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};
    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};
    QTest::newRow("8a") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef8a << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 6} << MatrixSizeType<int>{4};

    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 0} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};
    QTest::newRow("8b") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef8b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{4};

    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 8} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 8} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 8} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 8} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{1, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{1, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};
    QTest::newRow("8c") << mSecondaryIntMatrix << MatrixPoint<int>{1, 9} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, 9} << isNegativeInt << c_IntMatrixRef8c << MatrixPoint<int>{0, 5} << MatrixPoint<int>{1, 9} << MatrixSizeType<int>{4};

    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{0, 1} << MatrixPoint<int>{1, 5} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};
    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{1, 5} << MatrixPoint<int>{0, 1} << MatrixPoint<int>{0, 3} << isNegativeInt << c_IntMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};
    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{-1, 1} << MatrixPoint<int>{1, 5} << MatrixPoint<int>{1, 3} << isNegativeInt << c_IntMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};
    QTest::newRow("8d") << mSecondaryIntMatrix << MatrixPoint<int>{1, 5} << MatrixPoint<int>{-1, 1} << MatrixPoint<int>{1, 3} << isNegativeInt << c_IntMatrixRef8d << MatrixPoint<int>{0, 2} << MatrixPoint<int>{1, 4} << MatrixSizeType<int>{2};

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

    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{4, 0} << isNegativeInt << c_IntMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 0} << isNegativeInt << c_IntMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{4, 0} << isNegativeInt << c_IntMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9a") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{4, 0} << isNegativeInt << c_IntMatrixRef9a << MatrixPoint<int>{2, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{4};

    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, -1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 0} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 0} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{0, 1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, 1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{-1, 1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9b") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{-1, -1} << isNegativeInt << c_IntMatrixRef9b << MatrixPoint<int>{0, 0} << MatrixPoint<int>{4, 1} << MatrixSizeType<int>{4};

    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 0} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, -1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, 0} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 0} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, -1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, -1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{8, 1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{8, 1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, 1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{0, 0} << MatrixPoint<int>{9, 1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, 1} << MatrixPoint<int>{9, -1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};
    QTest::newRow("9c") << mThirdIntMatrix << MatrixPoint<int>{9, 1} << MatrixPoint<int>{-1, -1} << MatrixPoint<int>{9, -1} << isNegativeInt << c_IntMatrixRef9c << MatrixPoint<int>{5, 0} << MatrixPoint<int>{9, 1} << MatrixSizeType<int>{4};

    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{2, 0} << MatrixPoint<int>{7, 1} << MatrixPoint<int>{5, 0} << isNegativeInt << c_IntMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};
    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{7, 1} << MatrixPoint<int>{2, 0} << MatrixPoint<int>{5, 0} << isNegativeInt << c_IntMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};
    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{2, -1} << MatrixPoint<int>{7, 1} << MatrixPoint<int>{5, 1} << isNegativeInt << c_IntMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};
    QTest::newRow("9d") << mThirdIntMatrix << MatrixPoint<int>{7, 1} << MatrixPoint<int>{2, -1} << MatrixPoint<int>{5, 1} << isNegativeInt << c_IntMatrixRef9d << MatrixPoint<int>{4, 0} << MatrixPoint<int>{6, 1} << MatrixSizeType<int>{2};

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
