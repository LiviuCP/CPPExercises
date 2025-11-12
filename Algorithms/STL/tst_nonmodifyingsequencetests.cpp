// clang-format off
#include <QTest>

#include <algorithm>
#include <cmath>

#include "matrixutils.h"

class NonModifyingSequenceTests : public QObject
{
    Q_OBJECT

public:
    NonModifyingSequenceTests();

private slots:
    void testAllAnyNoneOf(); // all_of, any_of, none_of
    void testForEach();
    void testForEachN();
    void testCount(); // count, count_if
    void testMismatch();
    void testFind(); // find, find_if, find_if_not
    void testFindEnd();
    void testFindFirstOf();
    void testAdjacentFind();
    void testSearch();
    void testSearchN();

private:
    struct AbsAccumulator // used by for_each and for_each_n
    {
        void operator () (int value)
        {
            mAbsSum += std::abs(value);
        }

        int mAbsSum{0};
    };

    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const IntMatrix mFourthIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
    const IntVector mPrimaryIntVector;
    const IntVector mSecondaryIntVector;
    const IntVector mThirdIntVector;
};

NonModifyingSequenceTests::NonModifyingSequenceTests()
    : mPrimaryIntMatrix{3, 4, {-1, 1, 1, 0,
                                2, 4, 5, 1,
                                3, -3, -3, 2
                        }}
    , mSecondaryIntMatrix{4, 5, {-1, 1, 1, 0, 2,
                                  4, 9, 2, 5, -3,
                                  -3, 2, 4, 2, 4,
                                  0, 1, 3, 5, -2
                          }}
    , mThirdIntMatrix{3, 9, {-1, 2, 4, 5, 6,  7,  2,  4, 5,
                              8, -2, 5, 6, 7, 2,  7,  8, 2,
                              4, 5, 6, 7, 2, -10, 8, -2, 4

                      }}
    , mFourthIntMatrix{4, 5, {1, -2, -4, -4, 3,
                              0, -4, -4, -4, 1,
                             -3,  4,  4,  4, 4,
                              4, -2,  3,  5, 1

                       }}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                         {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}
                                  }}
    , mPrimaryIntVector{2, 4, 5, 1, -2, 3, 8, 9}
    , mSecondaryIntVector{5, 6, 7, 2}
    , mThirdIntVector{-2, 2, 4}
{
}

void NonModifyingSequenceTests::testAllAnyNoneOf()
{
    QVERIFY(std::all_of(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return element.second > 9;}));
    QVERIFY(!std::all_of(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return element.first.size() > 4;}));
    QVERIFY(std::any_of(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return 5 == element.first.size();}));
    QVERIFY(!std::any_of(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZRowEnd(0), [](StringIntPair element) {return element.second < 4;}));
    QVERIFY(std::none_of(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return 9 == element.first.size();}));
    QVERIFY(!std::none_of(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZRowEnd(0), [](StringIntPair element) {return 14 == element.second;}));
}

void NonModifyingSequenceTests::testForEach()
{
    // for_each gets the functor by value, modifies the copy and returns it by value
    AbsAccumulator absAccumulator;
    absAccumulator = std::for_each(mPrimaryIntMatrix.constNBegin(), mPrimaryIntMatrix.constNEnd(), absAccumulator);

    QVERIFY(26 == absAccumulator.mAbsSum);
}

void NonModifyingSequenceTests::testForEachN()
{
    AbsAccumulator absAccumulator;
    IntMatrix::ConstReverseZIterator it{std::for_each_n(mPrimaryIntMatrix.getConstReverseZIterator(2, 1), 4, [&absAccumulator](int element){absAccumulator(element);})};

    QVERIFY(mPrimaryIntMatrix.getConstReverseZIterator(1, 1) == it && 12 == absAccumulator.mAbsSum);
}

void NonModifyingSequenceTests::testCount()
{
    QVERIFY(2 == std::count(mPrimaryIntMatrix.getConstZIterator(0, 1), mPrimaryIntMatrix.getConstZIterator(1, 3), 1));
    QVERIFY(0 == std::count(mPrimaryIntMatrix.getConstNIterator(0, 1), mPrimaryIntMatrix.getConstNIterator(2, 1), -3));
    QVERIFY(3 == std::count_if(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZRowEnd(0), [](StringIntPair element) {return element.first.size() <= 5;}));
    QVERIFY(0 == std::count_if(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return element.second > 14;}));
}

void NonModifyingSequenceTests::testMismatch()
{
    std::pair<IntMatrix::ConstZIterator, IntMatrix::ConstZIterator> iteratorPair1{std::mismatch(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd(), mSecondaryIntMatrix.constZBegin())};
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(1, 2) == iteratorPair1.first && mSecondaryIntMatrix.getConstZIterator(1, 1) == iteratorPair1.second);

    std::pair<IntMatrix::ConstReverseZIterator, IntMatrix::ConstZIterator> iteratorPair2{std::mismatch(mSecondaryIntMatrix.constReverseZRowBegin(1), mSecondaryIntMatrix.constReverseZRowEnd(1), mSecondaryIntMatrix.constZRowBegin(2))};
    QVERIFY(mSecondaryIntMatrix.getConstReverseZIterator(1, 3) == iteratorPair2.first && mSecondaryIntMatrix.getConstZIterator(2, 1) == iteratorPair2.second);

    std::pair<IntMatrix::ConstZIterator, IntVector::const_iterator> iteratorPair3{std::mismatch(mPrimaryIntMatrix.constZRowBegin(1), mPrimaryIntMatrix.constZRowEnd(1), mPrimaryIntVector.cbegin())};
    QVERIFY(mPrimaryIntMatrix.constZRowEnd(1) == iteratorPair3.first && mPrimaryIntVector.cbegin() + 4 == iteratorPair3.second);
}

void NonModifyingSequenceTests::testFind()
{
    QVERIFY(mPrimaryIntMatrix.getConstMIterator(1, 1) == std::find(mPrimaryIntMatrix.constMBegin(1), mPrimaryIntMatrix.constMEnd(1), 4));
    QVERIFY(mPrimaryIntMatrix.constNColumnEnd(2) == std::find(mPrimaryIntMatrix.constNColumnBegin(2), mPrimaryIntMatrix.constNColumnEnd(2), 6));
    QVERIFY(mPrimaryStringIntPairMatrix.getConstZIterator(8) == std::find_if(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return element.first.size() > 6 && 14 == element.second;}));
    QVERIFY(mPrimaryStringIntPairMatrix.constZEnd() == std::find_if(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return 7 == element.first.size() && 12 == element.second;}));
    QVERIFY(mPrimaryStringIntPairMatrix.getConstZIterator(0, 3) == std::find_if_not(mPrimaryStringIntPairMatrix.getConstZIterator(0, 1), mPrimaryStringIntPairMatrix.constZRowEnd(0), [](StringIntPair element) {return element.second < 12;}));
    QVERIFY(mPrimaryStringIntPairMatrix.constZEnd() == std::find_if_not(mPrimaryStringIntPairMatrix.getConstZIterator(1, 2), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element){return element.first.size() > 3; }));
}

void NonModifyingSequenceTests::testFindEnd()
{
    QVERIFY(mThirdIntMatrix.getConstZIterator(1, 8) == std::find_end(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mThirdIntMatrix.getConstZIterator(0, 6), mThirdIntMatrix.constZRowEnd(0)));
    QVERIFY(mThirdIntMatrix.getConstZIterator(5) == std::find_end(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mThirdIntMatrix.getConstZIterator(5), mThirdIntMatrix.getConstZIterator(8)));
    QVERIFY(mThirdIntMatrix.constZEnd() == std::find_end(mThirdIntMatrix.constZRowBegin(2), mThirdIntMatrix.constZEnd(), mThirdIntMatrix.getConstZIterator(5), mThirdIntMatrix.getConstZIterator(8)));
    QVERIFY(mThirdIntMatrix.getConstZIterator(2, 1) == std::find_end(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mSecondaryIntVector.cbegin(), mSecondaryIntVector.cend()));
    QVERIFY(mThirdIntMatrix.constZEnd() == std::find_end(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mThirdIntVector.cbegin(), mThirdIntVector.cend()));
}

void NonModifyingSequenceTests::testFindFirstOf()
{
    QVERIFY(mThirdIntMatrix.getConstZIterator(2, 1) == std::find_first_of(mThirdIntMatrix.constZRowBegin(2), mThirdIntMatrix.constZEnd(), mThirdIntMatrix.getConstZIterator(1, 1), mThirdIntMatrix.getConstZIterator(1, 4)));
    QVERIFY(mThirdIntMatrix.constNColumnEnd(3) == std::find_first_of(mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.constNColumnEnd(3), mThirdIntMatrix.getConstReverseZIterator(2, 7), mThirdIntMatrix.getConstReverseZIterator(2, 4)));
    QVERIFY(mThirdIntMatrix.getConstNIterator(1, 1) == std::find_first_of(mThirdIntMatrix.constNBegin(), mThirdIntMatrix.constNEnd(), mThirdIntMatrix.constDBegin(5), mThirdIntMatrix.constDEnd(5)));
}

void NonModifyingSequenceTests::testAdjacentFind()
{
    IntMatrix::ConstZIterator it{std::adjacent_find(mPrimaryIntMatrix.constZBegin(), mPrimaryIntMatrix.constZEnd())};
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(1) == it);
    it = std::adjacent_find(it + 1, mPrimaryIntMatrix.constZEnd());
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(2, 1) == it);
    it = std::adjacent_find(it, mPrimaryIntMatrix.constZEnd());
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(2, 1) == it);
    it = std::adjacent_find(it + 1, mPrimaryIntMatrix.constZEnd());
    QVERIFY(mPrimaryIntMatrix.constZEnd() == it);

    QVERIFY(std::adjacent_find(mSecondaryIntMatrix.constZRowBegin(1), mSecondaryIntMatrix.constZEnd()) == mSecondaryIntMatrix.getConstZIterator(1, 4));
    QVERIFY(std::adjacent_find(mThirdIntMatrix.constMBegin(0), mThirdIntMatrix.constMEnd(0)) == mThirdIntMatrix.getConstMIterator(1, 7));
}

void NonModifyingSequenceTests::testSearch()
{
    QVERIFY(mThirdIntMatrix.getConstZIterator(1) == std::search(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mThirdIntMatrix.getConstZIterator(1, 8), mThirdIntMatrix.getConstZIterator(2, 2)));
    QVERIFY(mThirdIntMatrix.getConstZIterator(5) == std::search(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mThirdIntMatrix.getConstZIterator(5), mThirdIntMatrix.getConstZIterator(8)));
    QVERIFY(mThirdIntMatrix.constZEnd() == std::search(mThirdIntMatrix.constZRowBegin(1), mThirdIntMatrix.constZEnd(), mThirdIntMatrix.getConstZIterator(5), mThirdIntMatrix.getConstZIterator(8)));
    QVERIFY(mThirdIntMatrix.getConstZIterator(3) == std::search(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mSecondaryIntVector.cbegin(), mSecondaryIntVector.cend()));
    QVERIFY(mThirdIntMatrix.constZEnd() == std::search(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), mThirdIntVector.cbegin(), mThirdIntVector.cend()));
}

void NonModifyingSequenceTests::testSearchN()
{
    QVERIFY(mFourthIntMatrix.getConstZIterator(2) == std::search_n(mFourthIntMatrix.constZBegin(), mFourthIntMatrix.constZEnd(), 2, -4));
    QVERIFY(mFourthIntMatrix.getConstZIterator(1, 1) == std::search_n(mFourthIntMatrix.constZRowBegin(1), mFourthIntMatrix.constZEnd(), 2, -4));
    QVERIFY(mFourthIntMatrix.getConstZIterator(1, 1) == std::search_n(mFourthIntMatrix.constZRowBegin(1), mFourthIntMatrix.constZEnd(), 3, -4));
    QVERIFY(mFourthIntMatrix.getConstZIterator(1, 2) == std::search_n(mFourthIntMatrix.getConstZIterator(1, 2), mFourthIntMatrix.constZEnd(), 2, -4));
    QVERIFY(mFourthIntMatrix.constZEnd() == std::search_n(mFourthIntMatrix.constZBegin(), mFourthIntMatrix.constZEnd(), 4, -4));
    QVERIFY(mFourthIntMatrix.getConstZIterator(2, 1) == std::search_n(mFourthIntMatrix.constZBegin(), mFourthIntMatrix.getConstZIterator(3, 0), 4, 4));
    QVERIFY(mFourthIntMatrix.getConstZIterator(3, 0) == std::search_n(mFourthIntMatrix.constZBegin(), mFourthIntMatrix.getConstZIterator(3, 0), 5, 4));
    QVERIFY(mFourthIntMatrix.getConstReverseZIterator(3, 0) == std::search_n(mFourthIntMatrix.constReverseZBegin(), mFourthIntMatrix.constReverseZEnd(), 5, 4));
}

QTEST_APPLESS_MAIN(NonModifyingSequenceTests)

#include "tst_nonmodifyingsequencetests.moc"
// clang-format on
