#include <QTest>

#include <algorithm>

#include "matrixutils.h"

class ComparisonPermutationTests : public QObject
{
    Q_OBJECT

public:
    ComparisonPermutationTests();

private slots:
    void testEqual();
    void testLexicographicalCompare();
#if (!defined (__APPLE__) && !defined (__MACH__))
    void testLexicographicalCompareThreeWay();      // some issues were encountered on MacOS with the three way lexicographical comparison so it has been excluded from MacOS
#endif
    void testIsPermutation();
    void testNextPrevPermutation(); // next_permutation, prev_permutation

private:
    static bool compareStringIntPairsLexicographically(const StringIntPair& firstElement, const StringIntPair& secondElement, bool isOnlyStringComparisonRequired);

    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
    const StringIntPairMatrix mSecondaryStringIntPairMatrix;
    const StringIntPairMatrix mThirdStringIntPairMatrix;
    const StringIntPairMatrix mFourthStringIntPairMatrix;
    const StringIntPairMatrix mFifthStringIntPairMatrix;
    const StringIntPairMatrix mSixthStringIntPairMatrix;
    const StringIntPairMatrix mSeventhStringIntPairMatrix;
};

ComparisonPermutationTests::ComparisonPermutationTests()
    : mPrimaryIntMatrix{4, 5, {-9, -7, -5, -5, -3,
                               -3, -1,  0,  1,  1,
                                1,  2,  3,  3,  4,
                                5,  7,  11, 11, 14
                        }}
    , mSecondaryIntMatrix{4, 5, {32,  20, -5,  1, -6,
                                 28,  20, -1, -7, -6,
                                 25,  18,  0, -3, -9,
                                 20,  -3, -5, -5, -15
                          }}
    , mThirdIntMatrix{4, 5, {1, -2, -4, -4, 3,
                             0, -4, -4, -4, 1,
                            -3,  4,  4,  4, 4,
                             4, -2,  3,  5, 1
                      }}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Kelly", 12}, {"Jack", 8}, {"Annabel", 11}, {"Patricia", 9}, {"Barney", 20},
                                         {"Kelly", 12}, {"John", 8}, {"Annabel", 11}, {"Patricia", 9}, {"Barney", 20}
                                  }}
    , mSecondaryStringIntPairMatrix{2, 5, {{"Amanda", 12}, {"Jack", 8}, {"Annabel", 11}, {"Patricia", 10}, {"Barney", 20},
                                           {"Audrey", 12}, {"Jack", 8}, {"Miranda", 11}, {"Cameron", 9}, {"Abel", 12}
                                    }}
    , mThirdStringIntPairMatrix{2, 5, {{"Kelly", 14}, {"Jack", 8}, {"Annabel", 11}, {"Patricia", 9}, {"Barney", 20},
                                       {"Kelly", 12}, {"John", 8}, {"Annabel", 11}, {"Patricia", 9}, {"Barney", 20}
                                }}
    , mFourthStringIntPairMatrix{2, 5, {{"Amanda", 12}, {"Jack", 8}, {"Mira", 11}, {"Patricia", 10}, {"Barney", 20},
                                        {"Amanda", 12}, {"Jack", 8}, {"Miranda", 11}, {"Patricia", 9}, {"Barney", 20}
                                 }}
    , mFifthStringIntPairMatrix{2, 5, {{"Mark", 7}, {"Alex", 8}, {"Alistair", 8}, {"Cameron", 9}, {"Andrew", 10},
                                       {"Alistair", 8}, {"Andrew", 10}, {"Cameron", 9}, {"Jeff", 8}, {"Mark", 7}
                                }}
    , mSixthStringIntPairMatrix{2, 5, {{"Christian", 13}, {"Patrick", 15}, {"Cameron", 9}, {"Andrew", 10}, {"Reggie", 12},
                                       {"Patrick", 15}, {"Andrew", 10}, {"Alexander", 13}, {"Reggie", 12}, {"Cameron", 9}
                                }}
    , mSeventhStringIntPairMatrix{2, 5, {{"Kelly", 12}, {"Jack", 8}, {"Annabel", 11}, {"Patricia", 9}, {"Barney", 20},
                                         {"Juana", 11}, {"Barbara", 10}, {"Anna", 18}, {"Joseph", 11}, {"Jeff", 15}
                                  }}
{
}

void ComparisonPermutationTests::testEqual()
{
    QVERIFY(std::equal(mPrimaryIntMatrix.getConstZIterator(0, 2), mPrimaryIntMatrix.constZRowEnd(0), mSecondaryIntMatrix.getConstReverseZIterator(3, 3)));

    // same as previous but now the end of the second range is provided explicitly
    QVERIFY(std::equal(mPrimaryIntMatrix.getConstZIterator(0, 2), mPrimaryIntMatrix.constZRowEnd(0), mSecondaryIntMatrix.getConstReverseZIterator(3, 3), mSecondaryIntMatrix.getConstReverseZIterator(3, 0)));

    // ranges don't have the same length
    QVERIFY(!std::equal(mPrimaryIntMatrix.getConstZIterator(0, 2), mPrimaryIntMatrix.constZRowEnd(0), mSecondaryIntMatrix.getConstReverseZIterator(3, 3), mSecondaryIntMatrix.getConstReverseZIterator(3, 1)));

    QVERIFY(!std::equal(mPrimaryIntMatrix.getConstZIterator(0, 2), mPrimaryIntMatrix.getConstZIterator(1, 1), mSecondaryIntMatrix.getConstReverseZIterator(3, 3)));

    // it is mandatory to explicitly put the end iterator for the second range otherwise there will be an attempt to dereference the matrix end iterator (which might lead to a crash)
    QVERIFY(!std::equal(mPrimaryIntMatrix.constReverseNColumnBegin(0), mPrimaryIntMatrix.getConstReverseNIterator(0, 0), mThirdIntMatrix.getConstZIterator(3, 3), mThirdIntMatrix.constZEnd()));

    QVERIFY(std::equal(mPrimaryStringIntPairMatrix.constZBegin(),
                       mPrimaryStringIntPairMatrix.constZRowEnd(0),
                       mPrimaryStringIntPairMatrix.constZRowBegin(1),
                       [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() == secondElement.first.size() && firstElement.second == secondElement.second;}));

    // equality check function not really required (is default), just for illustration purposes
    QVERIFY(!std::equal(mPrimaryStringIntPairMatrix.constZBegin(),
                        mPrimaryStringIntPairMatrix.constZRowEnd(0),
                        mPrimaryStringIntPairMatrix.constZRowBegin(1),
                        [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first == secondElement.first && firstElement.second == secondElement.second;}));

    // this time we will let the default equality check function do its job (=> not equal)
    QVERIFY(std::equal(mPrimaryStringIntPairMatrix.getConstZIterator(0, 2),
                       mPrimaryStringIntPairMatrix.constZRowEnd(0),
                       mPrimaryStringIntPairMatrix.getConstZIterator(1, 2)));

    QVERIFY(std::equal(mPrimaryStringIntPairMatrix.constReverseZBegin(),
                       mPrimaryStringIntPairMatrix.constReverseZRowEnd(1),
                       mPrimaryStringIntPairMatrix.constReverseZRowBegin(0),
                       mPrimaryStringIntPairMatrix.constReverseZEnd(),
                       [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() == secondElement.first.size();}));
    QVERIFY(!std::equal(mSecondaryStringIntPairMatrix.constZBegin(),
                        mSecondaryStringIntPairMatrix.getConstZIterator(0, 4),
                        mSecondaryStringIntPairMatrix.constZRowBegin(1),
                        mSecondaryStringIntPairMatrix.getConstZIterator(1, 4),
                        [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() == secondElement.first.size();}));
}

void ComparisonPermutationTests::testLexicographicalCompare()
{
    QVERIFY(std::lexicographical_compare(mPrimaryIntMatrix.constReverseNColumnBegin(2), mPrimaryIntMatrix.constReverseNColumnEnd(2), mPrimaryIntMatrix.constReverseNColumnBegin(3), mPrimaryIntMatrix.constReverseNColumnEnd(3)));

    // lexicographical equality
    QVERIFY(!std::lexicographical_compare(mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.getConstNIterator(3, 2), mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.getConstNIterator(3, 3)) &&
            !std::lexicographical_compare(mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.getConstNIterator(3, 3), mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.getConstNIterator(3, 2)));

    // shorter range which is prefix is lexicographically less
    QVERIFY(std::lexicographical_compare(mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.getConstNIterator(2, 3), mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.getConstNIterator(3, 2)));

    // first range is lexicographically less than second range (this one is shorter but not prefix)
    QVERIFY(std::lexicographical_compare(mSecondaryIntMatrix.getConstZIterator(0, 1), mSecondaryIntMatrix.getConstZIterator(0, 4), mSecondaryIntMatrix.constZRowBegin(3), mSecondaryIntMatrix.getConstZIterator(3, 2)));

    /* StringIntPair case 1: compare string-int pair by using the string only */
    QVERIFY(std::lexicographical_compare(mThirdStringIntPairMatrix.constZBegin(), mThirdStringIntPairMatrix.constZRowEnd(0), mThirdStringIntPairMatrix.constZRowBegin(1), mThirdStringIntPairMatrix.getConstZIterator(1, 3),
                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return compareStringIntPairsLexicographically(firstElement, secondElement, true);}));

    // just for proving that the compare function works correctly (vice-versa is false)
    QVERIFY(!std::lexicographical_compare(mThirdStringIntPairMatrix.constZRowBegin(1), mThirdStringIntPairMatrix.getConstZIterator(1, 3), mThirdStringIntPairMatrix.constZBegin(), mThirdStringIntPairMatrix.constZRowEnd(0),
                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return compareStringIntPairsLexicographically(firstElement, secondElement, true);}));

    /* StringIntPair case 2: compare string-int pair by using the int only */
    QVERIFY(std::lexicographical_compare(mSecondaryStringIntPairMatrix.constZRowBegin(1), mSecondaryStringIntPairMatrix.constZEnd(), mSecondaryStringIntPairMatrix.constZBegin(), mSecondaryStringIntPairMatrix.constZRowEnd(0),
                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second < secondElement.second;}));

    // just for proving that the compare function works correctly (vice-versa is false)
    QVERIFY(!std::lexicographical_compare(mSecondaryStringIntPairMatrix.constZBegin(), mSecondaryStringIntPairMatrix.constZRowEnd(0), mSecondaryStringIntPairMatrix.constZRowBegin(1), mSecondaryStringIntPairMatrix.constZEnd(),
                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second < secondElement.second;}));
    QVERIFY(std::lexicographical_compare(mSecondaryStringIntPairMatrix.constZBegin(), mSecondaryStringIntPairMatrix.constZRowEnd(0), mSecondaryStringIntPairMatrix.constReverseZBegin(), mSecondaryStringIntPairMatrix.constReverseZRowEnd(1),
                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second < secondElement.second;}));

    /* StringIntPair case 3: compare string-int pair by using the string and int parts (only use int if the strings are lexicographically equal) */

    // actually the comp function is not required (this lexicographic compare is done by default in C++20, it was just put for illustration purposes
    QVERIFY(std::lexicographical_compare(mFourthStringIntPairMatrix.constZBegin(), mFourthStringIntPairMatrix.constZRowEnd(0), mFourthStringIntPairMatrix.constZRowBegin(1), mFourthStringIntPairMatrix.constZEnd(),
                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return compareStringIntPairsLexicographically(firstElement, secondElement, false);}));

    // just for proving that the compare function works correctly (vice-versa is false)
    QVERIFY(!std::lexicographical_compare(mFourthStringIntPairMatrix.constZRowBegin(1), mFourthStringIntPairMatrix.constZEnd(), mFourthStringIntPairMatrix.constZBegin(), mFourthStringIntPairMatrix.constZRowEnd(0),
                                         [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return compareStringIntPairsLexicographically(firstElement, secondElement, false);}));

    // comp no longer added, let the default comparison operators do their job
    QVERIFY(std::lexicographical_compare(mFourthStringIntPairMatrix.constReverseZBegin(),
                                         mFourthStringIntPairMatrix.constReverseZRowEnd(1),
                                         mFourthStringIntPairMatrix.constReverseZRowBegin(0),
                                         mFourthStringIntPairMatrix.constReverseZEnd()));
}

#if (!defined (__APPLE__) && !defined (__MACH__))
void ComparisonPermutationTests::testLexicographicalCompareThreeWay()
{
    QVERIFY(std::lexicographical_compare_three_way(mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.constNColumnEnd(2), mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.constNColumnEnd(3)) < 0);
    QVERIFY(std::lexicographical_compare_three_way(mThirdIntMatrix.getConstReverseMIterator(2, 1), mThirdIntMatrix.constReverseMEnd(1), mThirdIntMatrix.getConstReverseNIterator(2, 2), mThirdIntMatrix.constReverseNColumnEnd(2)) == 0);
    QVERIFY(std::lexicographical_compare_three_way(mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.constNColumnEnd(3), mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.getConstNIterator(2, 2)) > 0);
    QVERIFY(std::lexicographical_compare_three_way(mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.constNColumnEnd(3), mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.constNColumnEnd(2)) > 0);
    QVERIFY(std::lexicographical_compare_three_way(mThirdIntMatrix.constDBegin(1), mThirdIntMatrix.constDEnd(1), mThirdIntMatrix.getConstDIterator(0, 1), mThirdIntMatrix.constDEnd(1)) == 0); // same sequence
    QVERIFY(std::lexicographical_compare_three_way(mThirdIntMatrix.constReverseZRowBegin(1), mThirdIntMatrix.getConstReverseZIterator(1, 0), mThirdIntMatrix.constReverseZRowBegin(1), mThirdIntMatrix.constReverseZRowEnd(1)) < 0);

    QVERIFY(std::lexicographical_compare_three_way(mSecondaryStringIntPairMatrix.constZBegin(),
                                                   mSecondaryStringIntPairMatrix.constZRowEnd(0),
                                                   mSecondaryStringIntPairMatrix.constZRowBegin(1),
                                                   mSecondaryStringIntPairMatrix.constZEnd(),
                                                   [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second < secondElement.second ? std::strong_ordering::less
                                                                                                                                                                                : firstElement.second > secondElement.second ? std::strong_ordering::greater
                                                                                                                                                                                                                             : std::strong_ordering::equal;})
            > 0);
    QVERIFY(std::lexicographical_compare_three_way(mSecondaryStringIntPairMatrix.constZBegin(),
                                                   mSecondaryStringIntPairMatrix.constZRowEnd(0),
                                                   mSecondaryStringIntPairMatrix.constZRowBegin(1),
                                                   mSecondaryStringIntPairMatrix.constZEnd())
            < 0); // default lexicographical comparison (compare integers only if strings are equal, otherwise only strings have a word to say)
    QVERIFY(std::lexicographical_compare_three_way(mThirdStringIntPairMatrix.constZBegin(),
                                                   mThirdStringIntPairMatrix.constZRowEnd(0),
                                                   mThirdStringIntPairMatrix.constZRowBegin(1),
                                                   mThirdStringIntPairMatrix.constZEnd(),
                                                   [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size() ? std::strong_ordering::less
                                                                                                                                                                                            : firstElement.first.size() > secondElement.first.size() ? std::strong_ordering::greater
                                                                                                                                                                                                                                                     : std::strong_ordering::equal;})
            == 0);
    QVERIFY(std::lexicographical_compare_three_way(mPrimaryStringIntPairMatrix.getConstZIterator(0, 2),
                                                   mPrimaryStringIntPairMatrix.constZRowEnd(0),
                                                   mPrimaryStringIntPairMatrix.getConstZIterator(1, 2),
                                                   mPrimaryStringIntPairMatrix.constZEnd())
            == 0);
    QVERIFY(std::lexicographical_compare_three_way(mPrimaryStringIntPairMatrix.getConstZIterator(0, 2),
                                                   mPrimaryStringIntPairMatrix.constZRowEnd(0),
                                                   mPrimaryStringIntPairMatrix.getConstZIterator(1, 2),
                                                   mPrimaryStringIntPairMatrix.getConstZIterator(1, 4))
            > 0);
    QVERIFY(std::lexicographical_compare_three_way(mFourthStringIntPairMatrix.constReverseZBegin(),
                                                   mFourthStringIntPairMatrix.constReverseZRowEnd(1),
                                                   mFourthStringIntPairMatrix.getConstReverseZIterator(1, 4),
                                                   mFourthStringIntPairMatrix.getConstReverseZIterator(0, 4))
            == 0);
}
#endif

void ComparisonPermutationTests::testIsPermutation()
{
    QVERIFY(std::is_permutation(mPrimaryIntMatrix.getConstZIterator(0, 1), mPrimaryIntMatrix.getConstZIterator(1, 4), mSecondaryIntMatrix.getConstReverseNIterator(2, 3)));
    QVERIFY(!std::is_permutation(mThirdIntMatrix.constNColumnBegin(2), mThirdIntMatrix.constNColumnEnd(2), mThirdIntMatrix.constReverseNColumnBegin(3)));

    // actually the last argument is not necessary (just for the sake of exercise)
    QVERIFY(std::is_permutation(mThirdIntMatrix.getConstReverseNIterator(2, 2), mThirdIntMatrix.constReverseNColumnEnd(2), mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.getConstNIterator(3, 3)));

    // a larger second interval fails the test
    QVERIFY(!std::is_permutation(mThirdIntMatrix.getConstReverseNIterator(2, 2), mThirdIntMatrix.constReverseNColumnEnd(2), mThirdIntMatrix.constNColumnBegin(3), mThirdIntMatrix.constNEnd()));

    // the reversed sequence is a valid permutation
    QVERIFY(std::is_permutation(mPrimaryIntMatrix.constMBegin(1), mPrimaryIntMatrix.constMEnd(1), mPrimaryIntMatrix.constReverseMBegin(1)));

    // so is the sequence to itself
    QVERIFY(std::is_permutation(mPrimaryIntMatrix.constDBegin(1), mPrimaryIntMatrix.constDEnd(1), mPrimaryIntMatrix.constDBegin(1)));

    // only the second pair element is relevant so a row is considered a permutation of the other one from this point of view
    QVERIFY(std::is_permutation(mFifthStringIntPairMatrix.constZBegin(),
                                mFifthStringIntPairMatrix.constZRowEnd(0),
                                mFifthStringIntPairMatrix.constReverseZBegin(),
                                [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second == secondElement.second;}));
    QVERIFY(!std::is_permutation(mFifthStringIntPairMatrix.constZBegin(),
                                 mFifthStringIntPairMatrix.getConstZIterator(0, 4),
                                 mFifthStringIntPairMatrix.constReverseZBegin(),
                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second == secondElement.second;}));

    // check pairs are memberwise equal
    QVERIFY(!std::is_permutation(mFifthStringIntPairMatrix.constZBegin(),
                                 mFifthStringIntPairMatrix.constZRowEnd(0), // actually the comp function is not required (this lexicographic compare is done by default in C++20, it was just put for illustration purposes
                                 mFifthStringIntPairMatrix.constReverseZBegin(),
                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second == secondElement.second && firstElement.first == secondElement.first;}));
    QVERIFY(std::is_permutation(mFifthStringIntPairMatrix.getConstZIterator(0, 2),
                                mFifthStringIntPairMatrix.constZRowEnd(0),
                                mFifthStringIntPairMatrix.getConstReverseZIterator(1, 2))); // let the default equality check do its job

    // this time it's the string size that determines the elements equality
    QVERIFY(std::is_permutation(mSixthStringIntPairMatrix.constReverseZRowBegin(0),
                                mSixthStringIntPairMatrix.constReverseZEnd(),
                                mSixthStringIntPairMatrix.constZRowBegin(1),
                                [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() == secondElement.first.size();}));
    QVERIFY(!std::is_permutation(mSixthStringIntPairMatrix.constReverseZRowBegin(0),
                                 mSixthStringIntPairMatrix.getConstReverseZIterator(0, 0),
                                 mSixthStringIntPairMatrix.constZRowBegin(1),
                                 [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() == secondElement.first.size();}));
}

void ComparisonPermutationTests::testNextPrevPermutation()
{
    IntMatrix firstMatrix{mPrimaryIntMatrix};
    IntMatrix firstMatrixRef{4, 5, {-9, -7, -5, -5, -3,
                                    -3, -1,  1,  0,  1,
                                     4,  3,  3,  2,  1,
                                     5,  7,  11, 11, 14
                             }};

    bool isHigherPermutation{std::next_permutation(firstMatrix.zRowBegin(1), firstMatrix.zRowEnd(1))};

    QVERIFY(isHigherPermutation);

    isHigherPermutation = std::next_permutation(firstMatrix.reverseZRowBegin(2), firstMatrix.reverseZRowEnd(2));

    QVERIFY(!isHigherPermutation && firstMatrixRef == firstMatrix);

    firstMatrix = mThirdIntMatrix;
    firstMatrixRef = {4, 5, {1, -2, -4, -4,  3,
                             0, -4, -4, -4,  1,
                             4,  4,  4,  4, -3,
                             4, -2,  3,  1,  5
                      }};

    bool isLowerPermutation{std::prev_permutation(firstMatrix.zRowBegin(3), firstMatrix.zEnd())};

    QVERIFY(isLowerPermutation);

    isLowerPermutation = std::prev_permutation(firstMatrix.zRowBegin(2), firstMatrix.zRowEnd(2));

    QVERIFY(!isLowerPermutation && firstMatrixRef == firstMatrix);

    StringIntPairMatrix secondMatrix{mSeventhStringIntPairMatrix};
    StringIntPairMatrix secondMatrixRef{2, 5, {{"Kelly", 12}, {"Jack", 8}, {"Annabel", 11}, {"Barney", 20}, {"Patricia", 9},
                                               {"Barbara", 10}, {"Anna", 18}, {"Juana", 11}, {"Joseph", 11}, {"Jeff", 15}
                                        }};

    isHigherPermutation = std::next_permutation(secondMatrix.zBegin(),
                                                secondMatrix.zRowEnd(0),
                                                [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second < secondElement.second;});

    QVERIFY(isHigherPermutation);

    isHigherPermutation = std::next_permutation(secondMatrix.reverseZBegin(),
                                                secondMatrix.reverseZRowEnd(1),
                                                [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(isHigherPermutation && secondMatrixRef == secondMatrix);

    secondMatrix = mSeventhStringIntPairMatrix;
    secondMatrixRef = {2, 5, {{"Kelly", 12}, {"Jack", 8}, {"Patricia", 9}, {"Barney", 20}, {"Annabel", 11},
                              {"Barbara", 10}, {"Juana", 11}, {"Anna", 18}, {"Joseph", 11}, {"Jeff", 15}
                       }};

    isLowerPermutation = std::prev_permutation(secondMatrix.zBegin(),
                                               secondMatrix.zRowEnd(0),
                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.second < secondElement.second;});

    QVERIFY(isLowerPermutation);

    isLowerPermutation = std::prev_permutation(secondMatrix.reverseZBegin(),
                                               secondMatrix.reverseZRowEnd(1),
                                               [](const StringIntPair& firstElement, const StringIntPair& secondElement) {return firstElement.first.size() < secondElement.first.size();});

    QVERIFY(isLowerPermutation && secondMatrixRef == secondMatrix);
}

/* This function represents the default lexicographical ordering behavior in C++20
   - compare 2 objects member by member
   - if 2 members are equal the next member comparison determines the ordering
*/
bool ComparisonPermutationTests::compareStringIntPairsLexicographically(const StringIntPair& firstElement, const StringIntPair& secondElement, bool isOnlyStringComparisonRequired)
{
    bool isLexicographicallySmaller{std::lexicographical_compare(firstElement.first.cbegin(), firstElement.first.cend(), secondElement.first.cbegin(), secondElement.first.cend())};

    if (!isOnlyStringComparisonRequired &&
            !isLexicographicallySmaller &&
            !std::lexicographical_compare(secondElement.first.cbegin(), secondElement.first.cend(), firstElement.first.cbegin(), firstElement.first.cend()))
    {
        isLexicographicallySmaller = firstElement.second < secondElement.second;
    }

    return isLexicographicallySmaller;
}


QTEST_APPLESS_MAIN(ComparisonPermutationTests)

#include "tst_comparisonpermutationtests.moc"
