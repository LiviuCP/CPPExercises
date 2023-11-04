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
    void testGatherAlgorithmStdVector_data();

private:
    template<typename BidirectionalIt, typename Predicate>
    std::pair<BidirectionalIt, BidirectionalIt> _gatherSequenceElements(BidirectionalIt startIt, BidirectionalIt endIt, BidirectionalIt gatheringPointIt, Predicate predicate);

    const IntVector mPrimaryIntVector;
    const IntVector mSecondaryIntVector;
    const IntVector mThirdIntVector;
    const IntVector mFourthIntVector;
    const IntVector mFifthIntVector;
};

OtherTests::OtherTests()
    : mPrimaryIntVector{2, -1, 5, -8, -7, 0, 3, 4, -5, 10, 8, -9, 0, 1}
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

QTEST_APPLESS_MAIN(OtherTests)

#include "tst_othertests.moc"
