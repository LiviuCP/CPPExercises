// clang-format off
#include <QTest>

#include <algorithm>
#include <random>

#include "matrixutils.h"

class ModifyingSequenceTests : public QObject
{
    Q_OBJECT

public:
    ModifyingSequenceTests();

private slots:
    void testCopy(); // copy, copy_if
    void testCopyN();
    void testCopyBackward();
    void testMove();
    void testMoveBackward();
    void testFill();
    void testFillN();
    void testTransform();
    void testGenerate();
    void testGenerateN();
    void testRemove(); // remove, remove_if
    void testRemoveCopy(); //remove_copy, remove_copy_if
    void testReplace(); // replace, replace_if
    void testReplaceCopy(); // replace_copy, replace_copy_if
    void testSwap();
    void testSwapRanges();
    void testIterSwap();
    void testReverse();
    void testReverseCopy();
    void testRotate();
    void testRotateCopy();
    void testShift(); // shift_left, shift_right
    void testShuffle();
    void testSample();
    void testUnique();
    void testUniqueCopy();

private:
    struct Generator
    {
        Generator() = default;
        Generator(int value)
            : mValue{value}
        {
        }

        int operator () ()
        {
            mValue += 2;
            return mValue;
        }

        int mValue{1};
    };

    struct Movable
    {
        Movable()
            : mValue{0}
        {
        }

        Movable(int value)
            : mValue{value}
        {
        }

        Movable(const Movable& movable) = default;

        Movable(Movable&& movable)
            : mValue{movable.mValue}
        {
            movable.mValue = 9;
        }

        Movable& operator=(const Movable& movable)
        {
            mValue = movable.mValue;
            return *this;
        }

        Movable& operator=(Movable&& movable)
        {
            mValue = movable.mValue;
            movable.mValue = 9;
            return *this;
        }

        bool operator==(const Movable& movable) const
        {
            return movable.mValue == mValue;
        }

        bool operator!=(const Movable& movable) const
        {
            return movable.mValue != mValue;
        }

        int mValue{0};
    };

    using MovableMatrix = Matrix<Movable>;

    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const IntMatrix mFourthIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
    const StringIntPairMatrix mSecondaryStringIntPairMatrix;
    const MovableMatrix mPrimaryMovableMatrix;
    const MovableMatrix mSecondaryMovableMatrix;
    const IntVector mPrimaryIntVector;
};

ModifyingSequenceTests::ModifyingSequenceTests()
    : mPrimaryIntMatrix{3, 4, {-1, 1, 1, 0,
                                2, 4, 5, 1,
                                3, -3, -3, 2
                        }}
    , mSecondaryIntMatrix{4, 5, {-1, 1, 1, 0, 2,
                                  4, 9, 2, 5, -3,
                                 -3, 2, 4, 2, 4,
                                  0, 1, 3, 5, -2
                          }}
    , mThirdIntMatrix{4, 5, {1, -2, -4, -4, 3,
                             0, -4, -4, -4, 1,
                            -3,  4,  4,  4, 4,
                             4, -2,  3,  5, 1

                      }}
    , mFourthIntMatrix{3, 9, {-1,  2, 4, 5, 6,  7,  2, 4, 5,
                               8, -2, 5, 6, 7,  2,  7, 8, 2,
                               4,  5, 6, 7, 2, -10, 8, -2, 4

                       }}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                         {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}
                                  }}
    , mSecondaryStringIntPairMatrix{2, 5, {{"Anna", 18}, {"Kelly", 12}, {"Annabel", 11}, {"Juan", 10}, {"Jack", 8},
                                         {"Barbara", 10}, {"Barney", 20}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                    }}
    , mPrimaryMovableMatrix{4, 4, {0, 5, 4, 8,
                                  -1, 2, 9, 7,
                                   3, 2, 4, 8,
                                  -2, 1, 7, 1
                            }}
    , mSecondaryMovableMatrix{4, 5, {10, -2, 4, 8, 7,
                                     3, -4, -4, 2, 5,
                                    -3, -7, 2, 3, -2,
                                    -1, 4, 2, 1, 0
                              }}
    , mPrimaryIntVector{2, 4, 5, 1, -2, 3, 8, 9}
{
}

void ModifyingSequenceTests::testCopy()
{
    IntMatrix firstMatrix{mSecondaryIntMatrix};
    IntMatrix firstMatrixRef{4, 5, {-1, 1, 1, 0, 0,
                                     4, 9, 2, 5, 1,
                                    -3, 2, 4, 2, 1,
                                     0, 1, 3, 5, -1
                             }};

    IntMatrix::ConstNIterator it1{std::copy(mPrimaryIntMatrix.constReverseZRowBegin(0), mPrimaryIntMatrix.constReverseZEnd(), firstMatrix.nColumnBegin(4))};

    QVERIFY(firstMatrixRef == firstMatrix && firstMatrix.constNEnd() == it1);

    StringIntPairMatrix secondMatrix{mSecondaryStringIntPairMatrix};
    const StringIntPairMatrix c_SecondMatrixRef{2, 5, {{"Anna", 18}, {"John", 11}, {"Reggie", 12}, {"Cameron", 10}, {"Jack", 8},
                                                       {"Barbara", 10}, {"Barney", 20}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                                }};

    StringIntPairMatrix::ConstReverseZIterator it2{std::copy_if(mPrimaryStringIntPairMatrix.getConstZIterator(1, 1),
                                                                mPrimaryStringIntPairMatrix.constZEnd(),
                                                                secondMatrix.getReverseZIterator(3),
                                                                [](StringIntPair element) {return element.second <= 12;})};

    QVERIFY(c_SecondMatrixRef == secondMatrix && secondMatrix.getReverseZIterator(0) == it2);

    // test overlap
    firstMatrix = mSecondaryIntMatrix;
    firstMatrixRef = {4, 5, {-1, 1,  1, 4,  9,
                              2, 5, -3, 5, -3,
                             -3, 2,  4, 2,  4,
                              0, 1,  3, 5, -2
                      }};

    IntMatrix::ConstZIterator it3{std::copy(firstMatrix.zRowBegin(1), firstMatrix.zRowEnd(1), firstMatrix.getZIterator(3))};

    QVERIFY(firstMatrixRef == firstMatrix && firstMatrix.getConstZIterator(1, 3) == it3);
}

void ModifyingSequenceTests::testCopyN()
{
    IntMatrix matrix{mThirdIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {1, -2, -4, -4, 3,
                                       0, -4, 1,  0, 1,
                                      -3,  8, 7,  2, 4,
                                       4, -2, 3,  5, 1

                                }};

    IntMatrix::ConstZIterator it{std::copy_n(mFourthIntMatrix.getConstReverseZIterator(1, 7), 3, matrix.getZIterator(2, 1))};

    QVERIFY(matrix.getConstZIterator(2, 4) == it);

    it = std::copy_n(matrix.constNBegin(), 2, matrix.getZIterator(1, 2));

    QVERIFY(c_MatrixRef == matrix && matrix.getConstZIterator(1, 4) == it);
}

void ModifyingSequenceTests::testCopyBackward()
{
    IntMatrix firstMatrix{mThirdIntMatrix};
    IntMatrix firstMatrixRef{4, 5, {1, -2, -4, -4, 3,
                                    0, -4, -4, -4, 1,
                                    -3, 2, 5,  4, 1,
                                    4, -2, 3,  5, 1
                             }};

    IntMatrix::ConstZIterator it{std::copy_backward(mSecondaryIntMatrix.constMBegin(0), mSecondaryIntMatrix.constMEnd(0), firstMatrix.zRowEnd(2))};

    QVERIFY(firstMatrixRef == firstMatrix && firstMatrix.getConstZIterator(2, 1) == it);

    // test overlap
    firstMatrix = mSecondaryIntMatrix;
    firstMatrixRef = {4, 5, {-1, 1,  1, 0, 2,
                              4, 9,  4, 9, 2,
                              5, -3, 4, 2, 4,
                              0, 1,  3, 5, -2
                      }};

    it = std::copy_backward(firstMatrix.zRowBegin(1), firstMatrix.zRowEnd(1), firstMatrix.getZIterator(2, 2));

    QVERIFY(firstMatrixRef == firstMatrix && firstMatrix.getConstZIterator(1, 2) == it);
}

void ModifyingSequenceTests::testMove()
{
    IntMatrix firstMatrix{mPrimaryIntMatrix};
    const IntMatrix c_FirstMatrixRef{3, 4, {-1, 1,  1, -1,
                                             2, 4,  5,  4,
                                             3, -3, -3, -3
                                     }};

    IntMatrix::ConstNIterator it1{std::move(firstMatrix.dBegin(0), firstMatrix.dEnd(0), firstMatrix.nColumnBegin(3))};

    QVERIFY(c_FirstMatrixRef == firstMatrix && firstMatrix.constNEnd() == it1);

    MovableMatrix secondMatrix{mPrimaryMovableMatrix};
    MovableMatrix secondMatrixRef{4, 4, {0, 9, 4, 5,
                                        -1, 9, 9, 2,
                                         3, 9, 4, 2,
                                        -2, 9, 7, 1
                                  }};

    MovableMatrix::ConstNIterator it2{std::move(secondMatrix.nColumnBegin(1), secondMatrix.nColumnEnd(1), secondMatrix.nColumnBegin(3))};

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.constNEnd() == it2);

    // test overlap
    secondMatrix = mSecondaryMovableMatrix;
    secondMatrixRef = {4, 5, {10, -2, 2, 8,  7,
                              3, -4, 2,  2,  5,
                             -3,  4, 9,  3, -2,
                             -1, -4, 9,  1,  0
                       }};

    it2 = std::move(secondMatrix.nColumnBegin(2), secondMatrix.nColumnEnd(2), secondMatrix.getNIterator(2, 1));

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstNIterator(2, 2) == it2);
}

void ModifyingSequenceTests::testMoveBackward()
{
    IntMatrix firstMatrix{mSecondaryIntMatrix};
    const IntMatrix c_FirstMatrixRef{4, 5, {-1, 1, 1, 0, 2,
                                             4, 9, 2, 5, -3,
                                            -3, 2, 4, 2, 4,
                                             4, 9, 2, 5, -3
                                     }};

    IntMatrix::ConstZIterator it1{std::move_backward(firstMatrix.zRowBegin(1), firstMatrix.zRowEnd(1), firstMatrix.zEnd())};

    QVERIFY(c_FirstMatrixRef == firstMatrix && firstMatrix.getConstZIterator(3, 0) == it1);

    MovableMatrix secondMatrix{mPrimaryMovableMatrix};
    MovableMatrix secondMatrixRef{4, 4, {0, 5, 4, 8,
                                         9, 9, 9, 9,
                                        -1, 2, 9, 7,
                                        -2, 1, 7, 1
                                  }};

    MovableMatrix::ConstZIterator it2{std::move_backward(secondMatrix.zRowBegin(1), secondMatrix.zRowEnd(1), secondMatrix.zRowEnd(2))};

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.zRowBegin(2) == it2);

    // test overlap
    secondMatrix = mSecondaryMovableMatrix;
    secondMatrixRef = {4, 5, {10,  9, -7, 8,  7,
                               3,  9,  4, 2,  5,
                              -3, -2,  2, 3, -2,
                              -1, -4,  2, 1,  0
                       }};

    MovableMatrix::ConstNIterator it3{std::move_backward(secondMatrix.nColumnBegin(1), secondMatrix.nColumnEnd(1), secondMatrix.getNIterator(2, 2))};

    QVERIFY(secondMatrixRef == secondMatrix && secondMatrix.getConstNIterator(2, 1) == it3);
}

void ModifyingSequenceTests::testFill()
{
    IntMatrix matrix{mThirdIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {1, -2, -4, -10, 3,
                                       0, -4, -10, -4, 1,
                                      -3, -10, 4,  4,  4,
                                      -10, -2, 3,  5,  1

                                }};

    std::fill(matrix.mBegin(1), matrix.mEnd(1), -10);

    QVERIFY(c_MatrixRef == matrix);
}

void ModifyingSequenceTests::testFillN()
{
    IntMatrix matrix{mFourthIntMatrix};
    const IntMatrix c_MatrixRef{3, 9, {-1,  2, 4,  5, 6,  7,  2,  4,  5,
                                        8, -2, 5, 14, 14, 14, 14, 14, 2,
                                        4,  5, 6,  7, 2, -10,  8, -2, 4

                                }};

    IntMatrix::ConstReverseZIterator it{std::fill_n(matrix.getReverseZIterator(1, 7), 5, 14)};

    QVERIFY(c_MatrixRef == matrix && matrix.getConstReverseZIterator(1, 2) == it);
}

void ModifyingSequenceTests::testTransform()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    IntMatrix matrixRef{3, 4, {-1, 1, 1, 0,
                                4, 8, 8, 3,
                                3, -3, -3, 2
                        }};

    IntMatrix::ConstReverseZIterator it1{std::transform(mSecondaryIntMatrix.constDBegin(1), mSecondaryIntMatrix.constDEnd(1), mThirdIntMatrix.constMBegin(0), matrix.reverseZRowBegin(1), [](int firstElement, int secondElement) {return std::abs(firstElement * secondElement);})};

    QVERIFY(matrixRef == matrix && matrix.getConstReverseZIterator(3) == it1);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {2, 2, 2, 1, 3,
                        5, 10, 3, 6, 4,
                        4, 3, 5, 3, 5,
                        1, 2, 4, 6, 3
                 }};

    IntMatrix::ConstZIterator it2{std::transform(matrix.constZBegin(), matrix.constZEnd(), matrix.zBegin(), [](int element) {return std::abs(element) + 1;})};

    QVERIFY(matrixRef == matrix && matrix.constZEnd() == it2);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {-2, 3, 5, 4, -1,
                         4, 13, 6, 9, -4,
                         0, -2, 0, -2, 0,
                        -4, 3, 0, 0, -3
                 }};

    it2 = std::transform(matrix.constZBegin(), matrix.constZEnd(), mThirdIntMatrix.constZBegin(), matrix.zBegin(), [](int firstElement, int secondElement) {return firstElement - secondElement;});
    QVERIFY(matrixRef == matrix && matrix.constZEnd() == it2);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {-1, 1, -2, 0, 2,
                         4, 9, 4, 5, -3,
                        -3, 2, 4, 2,  4,
                         0, 1, 1, 5, -2
                 }};

    IntMatrix::ConstReverseNIterator it3{std::transform(matrix.constDBegin(1), matrix.constDEnd(1), matrix.constMBegin(1), matrix.reverseNColumnBegin(2), [](int firstElement, int secondElement) {return firstElement + secondElement;})};

    QVERIFY(matrixRef == matrix && matrix.constReverseNColumnBegin(1) == it3);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {1, 5, 3, 0, 4,
                        4, 4, 4, 4, 0,
                        1, 0, 0, 0, 0,
                        3, 0, 0, 0, 1
                 }};

    it2 = std::transform(mThirdIntMatrix.constReverseZBegin(), mThirdIntMatrix.constReverseZEnd(), matrix.zBegin(), [](int element) {return element < 0 ? 0 : element;});

    QVERIFY(matrixRef == matrix && matrix.zEnd() == it2);
}

void ModifyingSequenceTests::testGenerate()
{
    IntMatrix matrix{mThirdIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {4,  6,  8,  10, 12,
                                       14, 16, 18, 20, 22,
                                       24, 26, 28, 30, 32,
                                       34, 36, 38, 40, 42
                                }};

    Generator generator{2};
    std::generate(matrix.zBegin(), matrix.zEnd(), generator);

    QVERIFY(c_MatrixRef == matrix);
}

void ModifyingSequenceTests::testGenerateN()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {-1, 1, 1, 13,  2,
                                        4, 9, 2, 11, -3,
                                        -3, 2, 4, 9,  4,
                                        0, 1, 15, 7, -2
                                }};

    Generator generator{5};
    IntMatrix::ConstReverseNIterator it{std::generate_n(matrix.reverseNColumnBegin(3), 5, generator)};

    QVERIFY(c_MatrixRef == matrix && matrix.getConstReverseNIterator(2, 2) == it);
}

void ModifyingSequenceTests::testRemove()
{
    IntMatrix firstMatrix{mThirdIntMatrix};
    IntMatrix firstMatrixRef{3, 5, {1, -2, 3, 0, 1,
                                   -3, 4, 4, 4, 4,
                                    4, -2, 3, 5, 1
                             }};

    IntMatrix::ConstZIterator it1{std::remove(firstMatrix.zBegin(), firstMatrix.zEnd(), -4)};

    QVERIFY(firstMatrix.constZRowBegin(3) == it1);

    // std::remove places the removed elements as unspecified values at the end of the "queue", doesn't actually erase them from container
    firstMatrix.eraseRow(3);

    QVERIFY(firstMatrixRef == firstMatrix);

    firstMatrix = mThirdIntMatrix;
    firstMatrixRef = {4, 4, {1, -2, 4, 3,
                             0, -4, 3, 1,
                            -3, 4, 4, 4,
                             4, -2, 5, 1
                      }};

    IntMatrix::ConstReverseNIterator it2{std::remove(firstMatrix.reverseNColumnBegin(3), firstMatrix.reverseNColumnEnd(2), -4)};

    QVERIFY(firstMatrix.constReverseNColumnBegin(2) == it2 && *firstMatrix.constReverseNColumnBegin(1) == -2);

    firstMatrix.eraseColumn(2);

    QVERIFY(firstMatrixRef == firstMatrix);

    StringIntPairMatrix secondMatrix{mPrimaryStringIntPairMatrix};
    StringIntPairMatrix secondMatrixRef{2, 4, {{"Alex", 10}, {"Kevin", 11}, {"George", 14}, {"Mark", 9},
                                              {"Andrew", 11}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}
                                        }};

    StringIntPairMatrix::ConstNIterator it3{std::remove_if(secondMatrix.nColumnBegin(1), secondMatrix.nColumnEnd(2), [](StringIntPair element) {return 10 == element.second;})};

    QVERIFY(secondMatrix.constNColumnBegin(2) == it3 && (*secondMatrix.constNColumnBegin(3) == StringIntPair{"George", 14}));

    secondMatrix.eraseColumn(2);

    QVERIFY(secondMatrixRef == secondMatrix);
}

void ModifyingSequenceTests::testRemoveCopy()
{
    IntMatrix firstMatrix{mSecondaryIntMatrix};
    const IntMatrix c_FirstMatrixRef{4, 5, {-1, 1, 1, 0, 2,
                                             4, 9, 2, 5, -3,
                                             1, 0, 3, -2, 1,
                                             0, 1, 3, 5, -2
                                     }};

    IntMatrix::ConstZIterator it1{std::remove_copy(mThirdIntMatrix.constReverseZRowBegin(1), mThirdIntMatrix.constReverseZEnd(), firstMatrix.zRowBegin(2), -4)};

    QVERIFY(c_FirstMatrixRef == firstMatrix && firstMatrix.constZRowBegin(3) == it1);

    StringIntPairMatrix secondMatrix{mSecondaryStringIntPairMatrix};
    const StringIntPairMatrix c_SecondMatrixRef{2, 5, {{"Alex", 10}, {"Kevin", 11}, {"Mark", 9}, {"John", 11}, {"Jack", 8},
                                                       {"Barbara", 10}, {"Barney", 20}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                                }};

    StringIntPairMatrix::ConstZIterator it2{std::remove_copy_if(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), secondMatrix.zBegin(), [](StringIntPair element) {return element.first.size() >= 6;})};

    QVERIFY(c_SecondMatrixRef == secondMatrix && secondMatrix.getConstZIterator(4) == it2);
}

void ModifyingSequenceTests::testReplace()
{
    IntMatrix firstMatrix{mFourthIntMatrix};
    const IntMatrix c_FirstMatrixRef{3, 9, {-1, 2,  4, 5, 6, -14, 2,  4, 5,
                                             8, -2, 5, 6, -14, 2, 7,  8, 2,
                                             4, 5,  6, 7, 2, -10, 8, -2, 4

                                     }};

    std::replace(firstMatrix.getReverseNIterator(1, 5), firstMatrix.getReverseNIterator(2, 3), 7, -14);

    QVERIFY(c_FirstMatrixRef == firstMatrix);

    StringIntPairMatrix secondMatrix{mPrimaryStringIntPairMatrix};
    const StringIntPairMatrix c_SecondMatrixRef{2, 5, {{"Alex", 10}, {"RESERVED", 0}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                                      {"RESERVED", 0}, {"Cameron", 10}, {"RESERVED", 0}, {"Patrick", 14}, {"RESERVED", 0}
                                                }};

    std::replace_if(secondMatrix.zBegin(), secondMatrix.zEnd(), [](StringIntPair element) {return element.second > 10 && element.second < 14;}, StringIntPair{"RESERVED", 0});

    QVERIFY(c_SecondMatrixRef == secondMatrix);
}

void ModifyingSequenceTests::testReplaceCopy()
{
    IntMatrix firstMatrix{mThirdIntMatrix};
    const IntMatrix c_FirstMatrixRef{4, 5, {1, -2, -4, 1, 3,
                                            0, -4, 20, -4, 1,
                                           -3, 20, 4, 4, 4,
                                           -2, -2, 3, 5, 1
                                     }};

    IntMatrix::ConstMIterator it1{std::replace_copy(mSecondaryIntMatrix.constDBegin(1), mSecondaryIntMatrix.constDEnd(1), firstMatrix.mBegin(1), 2, 20)};

    QVERIFY(c_FirstMatrixRef == firstMatrix && firstMatrix.constMEnd(1) == it1);

    StringIntPairMatrix secondMatrix{mPrimaryStringIntPairMatrix};
    const StringIntPairMatrix c_SecondMatrixRef{2, 5, {{"John", 11}, {"RESERVED", 0}, {"Reggie", 12}, {"RESERVED", 0},  {"Mark", 9},
                                                       {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}
                                                }};

    StringIntPairMatrix::ConstZIterator it2{std::replace_copy_if(secondMatrix.constReverseZBegin(),
                                                                 secondMatrix.getConstReverseZIterator(1, 0),
                                                                 secondMatrix.zBegin(),
                                                                 [](StringIntPair element) {return 7 == element.first.size();},
                                                                 StringIntPair{"RESERVED", 0})};

    QVERIFY(c_SecondMatrixRef == secondMatrix && secondMatrix.getConstZIterator(4) == it2);
}

void ModifyingSequenceTests::testSwap()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    const IntMatrix c_MatrixRef{3, 4, {-1, 1, 1, 0,
                                        2, 4, -3, 1,
                                        3, 5, -3, 2
                                }};

    std::swap(matrix.at(1,2), matrix.at(2, 1));

    QVERIFY(c_MatrixRef == matrix);
}

void ModifyingSequenceTests::testSwapRanges()
{
    IntMatrix matrix{mFourthIntMatrix};
    const IntMatrix c_MatrixRef{3, 9, {-1, 4,  4,  5,  6, 7, 2, 2,  5,
                                        8, -2, 7,  6,  7, 2, 5, 8,  2,
                                        4, 5,  6, -10, 2, 7, 8, -2, 4
                                }};

    IntMatrix::ConstMIterator it{std::swap_ranges(matrix.dBegin(1), matrix.dEnd(1), matrix.mBegin(1))};

    QVERIFY(c_MatrixRef == matrix && matrix.constMEnd(1) == it);
}

void ModifyingSequenceTests::testIterSwap()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    const IntMatrix c_MatrixRef{3, 4, {-1, 1, 1, 0,
                                        2, 4, -3, 1,
                                        3, 5, -3, 2
                                }};

    // iterators don't need to be same type (any type of non-const iterator pointing to the element to be swapped can be used)
    std::iter_swap(matrix.getZIterator(1,2), matrix.getNIterator(2, 1));

    QVERIFY(c_MatrixRef == matrix);

    std::iter_swap(matrix.reverseZRowBegin(1), matrix.getNIterator(1, 3)); // no change, same element

    QVERIFY(c_MatrixRef == matrix);
}

void ModifyingSequenceTests::testReverse()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    IntMatrix matrixRef{3, 4, {2, -3, -3, 3,
                               1,  5,  4, 2,
                               0,  1,  1, -1
                        }};
    std::reverse(matrix.zBegin(), matrix.zEnd());

    QVERIFY(matrixRef == matrix);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {5, 1, 1,  0,  2,
                        3, 4, 2,  5, -3,
                        1, 2, 9,  2,  4,
                        0, -3, 4, -1, -2
                 }};

    for(matrix_diff_t diagonalNr{1 - static_cast<matrix_diff_t>(matrix.getNrOfRows())}; diagonalNr <= 0; ++diagonalNr)
    {
        std::reverse(matrix.dBegin(diagonalNr), matrix.dEnd(diagonalNr));
    }

    QVERIFY(matrixRef == matrix);
}

void ModifyingSequenceTests::testReverseCopy()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    IntMatrix matrixRef{4, 5, {-1, 1, 1, 1,  2,
                                4, 9, 4, 5, -3,
                               -3, -4, 4, 2, 4,
                               -2, 1, 3, 5, -2
                        }};

    IntMatrix::ConstMIterator it1{std::reverse_copy(mThirdIntMatrix.constDBegin(1), mThirdIntMatrix.constDEnd(1), matrix.mBegin(1))};

    QVERIFY(matrixRef == matrix && matrix.constMEnd(1) == it1);

    matrix = mThirdIntMatrix;
    matrixRef = {4, 5, {1, -2, -4, 2, 3,
                        0, -4, -4, 4, 1,
                        -3, 4, 4, 5,  4,
                        4, -2, 3, 1,  1

                 }};

    IntMatrix::ConstNIterator it2{std::reverse_copy(mPrimaryIntMatrix.constReverseZRowBegin(1), mPrimaryIntMatrix.constReverseZRowEnd(1), matrix.nColumnBegin(3))};

    QVERIFY(matrixRef == matrix && matrix.constNColumnBegin(4) == it2);

    matrixRef = {4, 5, {1, 4, -4, 2, 3,
                        3, 2, -4, 4, 1,
                        1, 4, 4, 5, 4,
                        5, -2, 3, 1, 1

                 }};

    it2 = std::reverse_copy(matrix.constNColumnBegin(3), matrix.getConstNIterator(1, 4), matrix.getNIterator(1, 0));

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(2, 1) == it2);
}

void ModifyingSequenceTests::testRotate()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    IntMatrix matrixRef{3, 4, {5, 1, 3, -3,
                              -3, 2, -1, 1,
                               1, 0, 2,  4
                        }};

    IntMatrix::ConstZIterator it1{std::rotate(matrix.zBegin(), matrix.getZIterator(1, 2), matrix.zEnd())};

    QVERIFY(matrixRef == matrix && matrix.getConstZIterator(1, 2) == it1);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {4,  0, 1, 3, 5,
                       -2, -1, 1, 1, 0,
                        2,  4, 9, 2, 5,
                       -3, -3, 2, 4, 2
                 }};

    IntMatrix::ConstReverseZIterator it2{std::rotate(matrix.reverseZBegin(), matrix.getReverseZIterator(2, 3), matrix.reverseZEnd())};

    QVERIFY(matrixRef == matrix && matrix.getReverseZIterator(1, 0) == it2);

    matrix = mThirdIntMatrix;
    matrixRef = {4, 5, {1, -2, -4, -4, -4,
                        0, -4, -4,  5,  4,
                       -3,  4,  4,  3,  4,
                        4, -2,  3,  1,  1

                 }};

    IntMatrix::ConstNIterator it3{std::rotate(matrix.getNIterator(1, 3), matrix.getNIterator(3, 3), matrix.getNIterator(2, 4))};

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(0, 4) == it3);
}

void ModifyingSequenceTests::testRotateCopy()
{
    IntMatrix matrix{mThirdIntMatrix};
    IntMatrix matrixRef{4, 5, {1, -2, 4, -4, 3,
                               0, -4, 5, -4, 1,
                              -3, -3, -3, 4, 4,
                               4,  2,  3, 5, 1

                        }};

    IntMatrix::ConstNIterator it{std::rotate_copy(mSecondaryIntMatrix.getConstZIterator(1, 3), mSecondaryIntMatrix.getConstZIterator(2, 0), mSecondaryIntMatrix.getConstZIterator(2, 3), matrix.getNIterator(2, 1))};

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(3, 2) == it);

    matrix = mFourthIntMatrix;
    matrixRef = {3, 9, {-1,  2, 4, 5, 6, 7, 2, 4, 5,
                         8, -2, 5, 6, 7, 2, 7, 8, 2,
                         4,  5, 6, 5, 4, 2, 7, 6, 4

                 }};

    IntMatrix::ConstReverseZIterator it2{std::rotate_copy(matrix.getConstZIterator(2), matrix.getConstZIterator(4), matrix.getConstZIterator(7), matrix.getReverseZIterator(2, 7))};

    QVERIFY(matrixRef == matrix && matrix.getConstReverseZIterator(2, 2) == it2);
}

void ModifyingSequenceTests::testShift()
{
// currently there are some issues with std::shift_left and std::shift_right on Linux when using Matrix iterators so for the moment this snippet has been restricted to Mac only
#if (defined (__APPLE__) && defined (__MACH__))
    IntMatrix matrix{mSecondaryIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {-1, 1, 3,  -10, 2,
                                        4, 1, -5, -10, 5,
                                       -3, 2, -5,  0,  2,
                                        0, 4, -5,  5, -3
                                }};

    IntMatrix::ConstNIterator it1{std::shift_left(matrix.nColumnBegin(1), matrix.nColumnEnd(2), 3)};
    std::fill(matrix.reverseNColumnBegin(2), matrix.reverseNColumnBegin(2) + 3, -5); // just for ensuring a predictible result

    QVERIFY(matrix.getConstNIterator(1, 2) == it1);

    it1 = std::shift_right(matrix.nColumnBegin(3), matrix.nEnd(), 2);
    std::fill(matrix.nColumnBegin(3), matrix.nColumnBegin(3) + 2, -10); // just for ensuring a predictible result

    QVERIFY(c_MatrixRef == matrix && matrix.getConstNIterator(2, 3) == it1);
#endif
    std::vector<int> intVector{mPrimaryIntVector};
    std::vector<int> intVectorRef = {-2, -2, 1, -2, 3, 8, 9, -1};

    IntVector::const_iterator it2{std::shift_left(intVector.begin(), intVector.end(), 3)};
    std::fill(intVector.rbegin(), intVector.rbegin() + 3, -1); // just for ensuring a predictible result

    QVERIFY(intVector.cbegin() + 5 == it2);

    it2 = std::shift_right(intVector.begin(), intVector.end(), 2);
    std::fill(intVector.begin(), intVector.begin() + 2, -2); // just for ensuring a predictible result

    QVERIFY(intVectorRef == intVector && intVector.cbegin() + 2 == it2);
}

void ModifyingSequenceTests::testShuffle()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    const IntMatrix c_MatrixRef{matrix};

    std::random_device randDev;
    std::mt19937 mersenne(randDev());

    std::shuffle(matrix.zBegin(), matrix.zEnd(), mersenne);

    QString shuffledOutput;

    for (const auto& element : matrix)
    {
        shuffledOutput.append(QString::number(element) + " ");
    }

    qDebug().noquote() << "Shuffled matrix: " << shuffledOutput;

    QVERIFY(std::is_permutation(matrix.constZBegin(), matrix.constZEnd(), c_MatrixRef.constZBegin()));
}

void ModifyingSequenceTests::testSample()
{
    IntVector intVector{mPrimaryIntVector};

    std::random_device randDev;
    std::mt19937 mersenne(randDev());

    IntVector::const_iterator it{std::sample(mSecondaryIntMatrix.constZBegin(), mSecondaryIntMatrix.constZEnd(), intVector.begin(), intVector.size(), mersenne)};

    QVERIFY(intVector.cend() == it);

    QString sampledOutput;

    for (const auto& element : intVector)
    {
        sampledOutput.append(QString::number(element) + " ");
        QVERIFY(std::count(intVector.cbegin(), intVector.cend(), element) <= std::count(mSecondaryIntMatrix.constZBegin(), mSecondaryIntMatrix.constZEnd(), element));
    }

    qDebug().noquote() << "Sampled output: " << sampledOutput;
}

void ModifyingSequenceTests::testUnique()
{
    IntMatrix matrix{mThirdIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {-9, -9, -9, -9, -9,
                                       -9, -9,  1, -2, -4,
                                        3,  0, -4,  1, -3,
                                        4, -2,  3,  5,  1
                                }};

    IntMatrix::ReverseZIterator it{std::unique(matrix.reverseZBegin(), matrix.reverseZEnd())};
    std::fill(it, matrix.reverseZEnd(), -9);

    QVERIFY(c_MatrixRef == matrix && matrix.getConstReverseZIterator(6) == it);
}

void ModifyingSequenceTests::testUniqueCopy()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {1,  0, 4,  1,  2,
                                      -2, -4, -2, 5, -3,
                                      -4,  1, 3,  2,  4,
                                       3, -3, 5,  5, -2
                                }};

    IntMatrix::ConstNIterator it{std::unique_copy(mThirdIntMatrix.constZBegin(), mThirdIntMatrix.constZEnd(), matrix.nBegin())};

    QVERIFY(c_MatrixRef == matrix && matrix.getConstNIterator(1, 3) == it);
}

QTEST_APPLESS_MAIN(ModifyingSequenceTests)

#include "tst_modifyingsequencetests.moc"
// clang-format on
