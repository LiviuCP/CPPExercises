#include <QtTest>

#include <algorithm>

#include "datautils.h"

class MinMaxTests : public QObject
{
    Q_OBJECT

public:
    MinMaxTests();

private slots:
    void testMinMax(); // min, max, min_max
    void testMinMaxElement(); // min_element, max_element, minmax_element
    void testClamp();

private:
    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
    const StringIntPairMatrix mSecondaryStringIntPairMatrix;
    const StringIntPairMatrix mThirdStringIntPairMatrix;
};

MinMaxTests::MinMaxTests()
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
    , mThirdIntMatrix{4, 5, {1, -2, -4, -4, 3,
                             0, -4, -4, -4, 1,
                            -3,  4,  4,  4, 4,
                             4, -2,  3,  5, 1
                      }}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Mark", 7}, {"Alex", 8}, {"Alistair", 8}, {"Cameron", 9}, {"Andrew", 10},
                                         {"Kevin", 11}, {"John", 11}, {"Reggie", 12}, {"George", 14}, {"Patrick", 15}
                                  }} // sorted increasingly by child age
    , mSecondaryStringIntPairMatrix{2, 5, {{"Christian", 13}, {"Patrick", 15}, {"Cameron", 9}, {"Andrew", 10}, {"Reggie", 12},
                                           {"George", 14}, {"Gordon", 7}, {"John", 11}, {"Alex", 8}, {"Mark", 7}
                                  }} // sorted decreasingly by number of characters of the child name
    , mThirdStringIntPairMatrix{2, 5, {{"Anna", 18}, {"Kelly", 12}, {"Annabel", 11}, {"Juan", 10}, {"Jack", 8},
                                       {"Barbara", 10}, {"Barney", 20}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                }}
{
}

void MinMaxTests::testMinMax()
{
    QVERIFY(mSecondaryIntMatrix.at(3, 3) == std::min(mSecondaryIntMatrix.at(3, 3), mSecondaryIntMatrix.at(2, 1)));
    QVERIFY(mPrimaryIntMatrix.at(3, 1) == std::max(mPrimaryIntMatrix.at(1, 3), mPrimaryIntMatrix.at(3, 1)));

    const IntPair c_IntMinMaxValues{std::minmax(mSecondaryIntMatrix.at(1, 2), mPrimaryIntMatrix.at(3, 2))};
    const IntPair c_IntMinMaxValuesRef{mPrimaryIntMatrix.at(3, 2), mSecondaryIntMatrix.at(1, 2)};

    QVERIFY(c_IntMinMaxValuesRef == c_IntMinMaxValues);

    StringIntPair minElement{std::min(mPrimaryStringIntPairMatrix.at(1, 1),
                                      mPrimaryStringIntPairMatrix.at(0, 1),
                                      [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();})};
    StringIntPair maxElement{std::max(mPrimaryStringIntPairMatrix.at(1, 1),
                                      mPrimaryStringIntPairMatrix.at(0, 1),
                                      [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();})};

    QVERIFY(mPrimaryStringIntPairMatrix.at(1, 1) == minElement && mPrimaryStringIntPairMatrix.at(1, 1) == maxElement);

    minElement = std::min(mPrimaryStringIntPairMatrix.at(0, 1),
                          mPrimaryStringIntPairMatrix.at(1, 1),
                          [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();});
    maxElement = std::max(mPrimaryStringIntPairMatrix.at(0, 1),
                          mPrimaryStringIntPairMatrix.at(1, 1),
                          [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(mPrimaryStringIntPairMatrix.at(0, 1) == minElement && mPrimaryStringIntPairMatrix.at(0, 1) == maxElement);

    minElement = std::min(mPrimaryStringIntPairMatrix.at(0, 3),
                          mPrimaryStringIntPairMatrix.at(1, 2),
                          [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();});
    maxElement = std::max(mPrimaryStringIntPairMatrix.at(0, 3),
                          mPrimaryStringIntPairMatrix.at(1, 2),
                          [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(mPrimaryStringIntPairMatrix.at(1, 2) == minElement && mPrimaryStringIntPairMatrix.at(0, 3) == maxElement);

    minElement = std::min(mPrimaryStringIntPairMatrix.at(1, 2),
                          mPrimaryStringIntPairMatrix.at(0, 3),
                          [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();});
    maxElement = std::max(mPrimaryStringIntPairMatrix.at(1, 2),
                          mPrimaryStringIntPairMatrix.at(0, 3),
                          [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(mPrimaryStringIntPairMatrix.at(1, 2) == minElement && mPrimaryStringIntPairMatrix.at(0, 3) == maxElement);

    const std::pair<StringIntPair, StringIntPair> c_StringIntMinMaxValues{std::minmax(mSecondaryStringIntPairMatrix.at(1, 0),
                                                                                      mSecondaryStringIntPairMatrix.at(0, 3),
                                                                                      [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;})};
    const std::pair<StringIntPair, StringIntPair> c_StringIntMinMaxValuesRef{mSecondaryStringIntPairMatrix.at(0, 3), mSecondaryStringIntPairMatrix.at(1, 0)};

    QVERIFY(c_StringIntMinMaxValuesRef == c_StringIntMinMaxValues);
}

void MinMaxTests::testMinMaxElement()
{
    QVERIFY(mPrimaryIntMatrix.getConstReverseZIterator(2, 0) == std::min_element(mPrimaryIntMatrix.constReverseZRowBegin(2), mPrimaryIntMatrix.getConstReverseZIterator(1, 2)));
    QVERIFY(mPrimaryIntMatrix.getConstReverseZIterator(2, 3) == std::max_element(mPrimaryIntMatrix.getConstReverseZIterator(2, 3), mPrimaryIntMatrix.getConstReverseZIterator(1, 2)));
    QVERIFY(mThirdIntMatrix.getConstDIterator(1, 1, true) == std::min_element(mThirdIntMatrix.constDBegin(1), mThirdIntMatrix.constDEnd(1)));
    QVERIFY(mThirdIntMatrix.getConstMIterator(0, 2, true) == std::max_element(mThirdIntMatrix.constMBegin(0), mThirdIntMatrix.constMEnd(0)));

    ConstIntZIteratorPair itPair1{std::minmax_element(mThirdIntMatrix.getConstZIterator(2, 1), mThirdIntMatrix.constZRowEnd(2))};
    ConstIntZIteratorPair itPairRef1{mThirdIntMatrix.getConstZIterator(2, 1), mThirdIntMatrix.getConstZIterator(2, 4)};

    QVERIFY(itPairRef1 == itPair1);

    itPair1 = std::minmax_element(mSecondaryIntMatrix.getConstZIterator(1, 2), mSecondaryIntMatrix.getConstZIterator(2, 3));
    itPairRef1 = {mSecondaryIntMatrix.getConstZIterator(1, 4), mSecondaryIntMatrix.constZRowBegin(2)};

    QVERIFY(itPairRef1 == itPair1);

    QVERIFY(mThirdStringIntPairMatrix.getConstZIterator(0, 3) == std::min_element(mThirdStringIntPairMatrix.getConstZIterator(0, 1),
                                                                                  mThirdStringIntPairMatrix.getConstZIterator(1, 2),
                                                                                  [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));
    QVERIFY(mThirdStringIntPairMatrix.getConstZIterator(0, 2) == std::max_element(mThirdStringIntPairMatrix.getConstZIterator(0, 1),
                                                                                  mThirdStringIntPairMatrix.getConstZIterator(1, 2),
                                                                                  [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));
    QVERIFY(mThirdStringIntPairMatrix.constReverseZRowBegin(0) == std::min_element(mThirdStringIntPairMatrix.getConstReverseZIterator(1, 3),
                                                                                   mThirdStringIntPairMatrix.getConstReverseZIterator(0, 2),
                                                                                   [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));
    QVERIFY(mThirdStringIntPairMatrix.getConstReverseZIterator(1, 1) == std::max_element(mThirdStringIntPairMatrix.getConstReverseZIterator(1, 3),
                                                                                         mThirdStringIntPairMatrix.getConstReverseZIterator(0, 2),
                                                                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    ConstStringIntZIteratorPair itPair2{std::minmax_element(mThirdStringIntPairMatrix.getConstZIterator(0, 1),
                                                            mThirdStringIntPairMatrix.getConstZIterator(1, 1),
                                                            [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();})};
    ConstStringIntZIteratorPair itPairRef2{mThirdStringIntPairMatrix.getConstZIterator(0, 3), mThirdStringIntPairMatrix.constZRowBegin(1)};

    QVERIFY(itPairRef2 == itPair2);

    itPair2 = std::minmax_element(mThirdStringIntPairMatrix.getConstZIterator(0, 1),
                                  mThirdStringIntPairMatrix.getConstZIterator(1, 1),
                                  [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;});
    itPairRef2 = {mThirdStringIntPairMatrix.getConstZIterator(0, 4), mThirdStringIntPairMatrix.getConstZIterator(0, 1)};

    QVERIFY(itPairRef2 == itPair2);
}

void MinMaxTests::testClamp()
{
    QVERIFY(mPrimaryIntMatrix.at(3, 0) == std::clamp(mPrimaryIntMatrix.at(2, 4), mPrimaryIntMatrix.at(3, 0), mPrimaryIntMatrix.at(3, 2)));
    QVERIFY(mPrimaryIntMatrix.at(3, 0) == std::clamp(mPrimaryIntMatrix.at(3, 0), mPrimaryIntMatrix.at(3, 0), mPrimaryIntMatrix.at(3, 2)));
    QVERIFY(mPrimaryIntMatrix.at(3, 1) == std::clamp(mPrimaryIntMatrix.at(3, 1), mPrimaryIntMatrix.at(3, 0), mPrimaryIntMatrix.at(3, 2)));
    QVERIFY(mPrimaryIntMatrix.at(3, 2) == std::clamp(mPrimaryIntMatrix.at(3, 2), mPrimaryIntMatrix.at(3, 0), mPrimaryIntMatrix.at(3, 2)));
    QVERIFY(mPrimaryIntMatrix.at(3, 2) == std::clamp(mPrimaryIntMatrix.at(3, 3), mPrimaryIntMatrix.at(3, 0), mPrimaryIntMatrix.at(3, 2)));
    QVERIFY(mPrimaryIntMatrix.at(3, 2) == std::clamp(mPrimaryIntMatrix.at(3, 4), mPrimaryIntMatrix.at(3, 0), mPrimaryIntMatrix.at(3, 2)));

    QVERIFY(mPrimaryStringIntPairMatrix.at(0, 2) == std::clamp(mPrimaryStringIntPairMatrix.at(0, 0),
                                                               mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(1, 0),
                                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    // (0, 1) is not < (0, 2) and (1, 0) is not < (0, 1) according to the compare function => the clamp function returns the (0, 1) element itself (although it looks to be outside the low-high interval)
    QVERIFY(mPrimaryStringIntPairMatrix.at(0, 1) == std::clamp(mPrimaryStringIntPairMatrix.at(0, 1),
                                                               mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(1, 0),
                                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    QVERIFY(mPrimaryStringIntPairMatrix.at(0, 2) == std::clamp(mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(1, 0),
                                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));
    QVERIFY(mPrimaryStringIntPairMatrix.at(0, 3) == std::clamp(mPrimaryStringIntPairMatrix.at(0, 3),
                                                               mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(1, 0),
                                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));
    QVERIFY(mPrimaryStringIntPairMatrix.at(1, 0) == std::clamp(mPrimaryStringIntPairMatrix.at(1, 0),
                                                               mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(1, 0),
                                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    // same here, the element itself is returned although it looks to be outside the interval
    QVERIFY(mPrimaryStringIntPairMatrix.at(1, 1) == std::clamp(mPrimaryStringIntPairMatrix.at(1, 1),
                                                               mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(1, 0),
                                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    QVERIFY(mPrimaryStringIntPairMatrix.at(1, 0) == std::clamp(mPrimaryStringIntPairMatrix.at(1, 3),
                                                               mPrimaryStringIntPairMatrix.at(0, 2),
                                                               mPrimaryStringIntPairMatrix.at(1, 0),
                                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.second < secondElement.second;}));

    // same here, different comp function
    QVERIFY(mSecondaryStringIntPairMatrix.at(1, 4) == std::clamp(mSecondaryStringIntPairMatrix.at(1, 4),
                                                                 mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));

    QVERIFY(mSecondaryStringIntPairMatrix.at(1, 3) == std::clamp(mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));
    QVERIFY(mSecondaryStringIntPairMatrix.at(1, 2) == std::clamp(mSecondaryStringIntPairMatrix.at(1, 2),
                                                                 mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));
    QVERIFY(mSecondaryStringIntPairMatrix.at(0, 4) == std::clamp(mSecondaryStringIntPairMatrix.at(0, 4),
                                                                 mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));
    QVERIFY(mSecondaryStringIntPairMatrix.at(0, 2) == std::clamp(mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));

    // same here
    QVERIFY(mSecondaryStringIntPairMatrix.at(0, 1) == std::clamp(mSecondaryStringIntPairMatrix.at(0, 1),
                                                                 mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));

    QVERIFY(mSecondaryStringIntPairMatrix.at(0, 2) == std::clamp(mSecondaryStringIntPairMatrix.at(0, 0),
                                                                 mSecondaryStringIntPairMatrix.at(1, 3),
                                                                 mSecondaryStringIntPairMatrix.at(0, 2),
                                                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement){return firstElement.first.size() < secondElement.first.size();}));
}


QTEST_APPLESS_MAIN(MinMaxTests)

#include "tst_minmaxtests.moc"
