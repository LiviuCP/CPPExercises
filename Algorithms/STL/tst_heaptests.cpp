#include <QTest>

#include <algorithm>

#include "matrixutils.h"

class HeapTests : public QObject
{
    Q_OBJECT

public:
    HeapTests();

private slots:
    void testIsHeap(); // is_heap, is_heap_until
    void testMakeHeap();
    void testPushPopHeap(); // push_heap, pop_heap
    void testSortHeap();

private:
    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const IntMatrix mFourthIntMatrix;
};

HeapTests::HeapTests()
    : mPrimaryIntMatrix{4, 5, {-9, -7, -5, -5,  -3,
                               -3, -1,  20,  14, 10,
                                14,  12,  8,  2,  5,
                                5,  6,  11, 11,  14
                        }}
    , mSecondaryIntMatrix{4, 5, {32,  20, -5,  -4, -3,
                                 -2,  2,  -3,  4,  1,
                                 -1,  5,   4,  0,  9,
                                 8,  4,  15, -5,  -15
                          }}
    , mThirdIntMatrix{4, 5, {-2, 4,  8,  0,  9,
                             -5, 3,  7, -1,  8,
                             -4, 3,  2,  4, 16,
                             15, -2, 8, 10, 14
                      }}
    , mFourthIntMatrix{4, 5, {-2, 7,  15,  21, -5,
                              -5, -4, -2,  3,  8,
                              10, 11,  4,  18, 16,
                              -9, 20,  9,  20, 14
                       }}
{
}

void HeapTests::testIsHeap()
{
    // max heap
    QVERIFY(std::is_heap(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.getConstZIterator(3, 2)));
    QVERIFY(!std::is_heap(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.constZEnd()));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(3, 3) == std::is_heap_until(mPrimaryIntMatrix.getConstZIterator(1, 2), mPrimaryIntMatrix.constZEnd()));
    QVERIFY(!std::is_heap(mPrimaryIntMatrix.getConstZIterator(1, 3), mPrimaryIntMatrix.getConstZIterator(3, 2)));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(2, 1) == std::is_heap_until(mPrimaryIntMatrix.getConstZIterator(1, 3), mPrimaryIntMatrix.getConstZIterator(3, 2)));
    QVERIFY(mPrimaryIntMatrix.getConstZIterator(3, 2) == std::is_heap_until(mPrimaryIntMatrix.constZRowBegin(2), mPrimaryIntMatrix.getConstZIterator(3, 2)));

    // min heap
    QVERIFY(std::is_heap(mSecondaryIntMatrix.getConstZIterator(0, 2), mSecondaryIntMatrix.getConstZIterator(3, 2), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;}));
    QVERIFY(!std::is_heap(mSecondaryIntMatrix.getConstZIterator(0, 3), mSecondaryIntMatrix.getConstZIterator(3, 2), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;}));
    QVERIFY(mSecondaryIntMatrix.constZRowBegin(2) == std::is_heap_until(mSecondaryIntMatrix.getConstZIterator(0, 3), mSecondaryIntMatrix.getConstZIterator(3, 2), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;}));
    QVERIFY(!std::is_heap(mSecondaryIntMatrix.constZRowBegin(2), mSecondaryIntMatrix.getConstZIterator(3, 2), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;}));
    QVERIFY(mSecondaryIntMatrix.getConstZIterator(2, 3) == std::is_heap_until(mSecondaryIntMatrix.constZRowBegin(2), mSecondaryIntMatrix.getConstZIterator(3, 2), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;}));
    QVERIFY(mSecondaryIntMatrix.getConstZIterator(3, 3) == std::is_heap_until(mSecondaryIntMatrix.getConstZIterator(0, 2), mSecondaryIntMatrix.constZEnd(), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;}));
}

void HeapTests::testMakeHeap()
{
    // max heap
    IntMatrix matrix{mThirdIntMatrix};
    IntMatrix matrixRef{4, 5, {-2, 4, 8, 0, 9,
                               -5, 3, 7, -1, 8,
                               -4, 3, 2, -2, 8,
                               10, 4, 15, 16, 14
                        }};

    std::make_heap(matrix.getReverseZIterator(3, 3), matrix.getReverseZIterator(1, 2));

    QVERIFY(matrixRef == matrix);

    // min heap
    matrix = mThirdIntMatrix;
    matrixRef = {4, 5, {-2, 4, 8, 0,  9,
                        -5, 3, 7, 16, 10,
                        15, 8, 2, 4,  8,
                        3, -2, -1, -4, 14
                 }};

    std::make_heap(matrix.getReverseZIterator(3, 3), matrix.getReverseZIterator(1, 2), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;});

    QVERIFY(matrixRef == matrix);

    // max heap
    matrix = mFourthIntMatrix;
    matrixRef = {4, 5, {-2, 7, 15, 11, -5,
                        -5, -4, 18, 3,  8,
                        10, 21, 20, -2, 16,
                        -9, 20, 9,  4,  14
                 }};

    std::make_heap(matrix.getNIterator(2, 1), matrix.getNIterator(1, 4));

    QVERIFY(matrixRef == matrix);

    // min heap
    matrix = mFourthIntMatrix;
    matrixRef = {4, 5, {-2, 7, 9,  21, 20,
                        -5, -4, 3, 11,  8,
                        10, -5, 4, 18, 16,
                        -9, -2, 15, 20, 14
                 }};

    std::make_heap(matrix.getNIterator(2, 1), matrix.getNIterator(1, 4), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;});

    QVERIFY(matrixRef == matrix);
}

void HeapTests::testPushPopHeap()
{
    // max heap (2 pushes followed by 2 pops)
    IntMatrix matrix{mPrimaryIntMatrix};
    IntMatrix matrixRef{4, 5, {-9, -7, -5, -5, -3,
                               -3, -1,  20, 14, 11,
                                14, 12, 10, 2,  5,
                                5,  6,  11, 8, 14
                        }};

    std::push_heap(matrix.getZIterator(1, 2), matrix.getZIterator(3, 4));

    QVERIFY(matrixRef == matrix);

    matrixRef = {4, 5, {-9, -7, -5, -5, -3,
                        -3, -1,  20, 14, 14,
                        14,  12,  11,  2, 5,
                         5,  6,   11, 8,  10
                 }};

    std::push_heap(matrix.getZIterator(1, 2), matrix.zEnd());

    QVERIFY(matrixRef == matrix);

    matrixRef = {4, 5, {-9, -7, -5, -5,  -3,
                        -3, -1,  14,  14, 11,
                        14,  12, 10,  2,  5,
                         5,  6,  11, 8,  20
                 }};

    /* This is required as std::pop_heap apparently has different implementations on various platforms.
       In some implementations when the children are equal the right child is chosen when performing a "heapify" operation.
       In other implementations the left child is taken into account.
       This scenario contains a "heapify" operation involving two equal value children so two reference matrixes are required to ensure both possible outcomes are covered.
    */
    IntMatrix secondMatrixRef {4, 5, {-9, -7, -5, -5, -3,
                                      -3, -1,  14,  14, 14,
                                      10,  12,  11,  2, 5,
                                       5,  6,  11,  8,  20
                               }};

    std::pop_heap(matrix.getZIterator(1, 2), matrix.zEnd());

    QVERIFY(matrixRef == matrix || secondMatrixRef == matrix);

    matrixRef = {4, 5, {-9, -7, -5, -5, -3,
                        -3, -1,  14, 14, 11,
                         8,  12, 10,  2, 5,
                         5,  6,  11, 14, 20
                 }};

    /* Here the second reference is also required as it continues the above outcome(s). */
    secondMatrixRef = {4, 5, {-9, -7, -5,  -5, -3,
                              -3, -1,  14,  12, 14,
                              10,  11, 11,  2,  5,
                               5,  6,  8,  14,  20
                       }};

    std::pop_heap(matrix.getZIterator(1, 2), matrix.getZIterator(3, 4));

    QVERIFY(matrixRef == matrix || secondMatrixRef == matrix);

    // min heap (1 push followed by 1 pop on the full heap)
    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {32,  20, -5,  -5, -3,
                        -4,  2,  -3,  4,  -2,
                        -1,  5,   4,  0,  9,
                         8,  4,  15, 1, -15
                 }};

    std::push_heap(matrix.getZIterator(0, 2), matrix.getZIterator(3, 4), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;});

    QVERIFY(matrixRef == matrix);

    matrixRef = {4, 5, {32,  20, -5,  -4, -3,
                        -2,  2,  -3,  4,  1,
                        -1,  5,   4,  0,  9,
                         8,  4,  15, -5, -15
                 }};

    std::pop_heap(matrix.getZIterator(0, 2), matrix.getZIterator(3, 4), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;});

    QVERIFY(matrixRef == matrix);

    // min heap (1 push followed by 1 pop on a sub-heap)
    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {32,  20, -5,  -4, -3,
                        -2,  2,  -3,  4,  1,
                        -5,  -1,  4,  5,  9,
                         8,  4,  15, 0,  -15
                 }};

    std::push_heap(matrix.zRowBegin(2), matrix.getZIterator(3, 4), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;});

    QVERIFY(matrixRef == matrix);

    matrixRef = {4, 5, {32,  20, -5,  -4, -3,
                        -2,  2,  -3,  4,  1,
                        -1,  0,   4,  5,  9,
                         8,  4,  15, -5, -15
                 }};

    std::pop_heap(matrix.zRowBegin(2), matrix.getZIterator(3, 4), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;});

    QVERIFY(matrixRef == matrix);
}

void HeapTests::testSortHeap()
{
    // max heap, sort ascending
    IntMatrix matrix{mPrimaryIntMatrix};
    IntMatrix matrixRef{4, 5, {-9, -7, -5, -5,  -3,
                               -3, -1,  2,  5,   5,
                                6,  8,  10,  11, 12,
                               14,  14,  20, 11, 14
                        }};

    std::sort_heap(matrix.getZIterator(1, 2), matrix.getZIterator(3, 3));

    QVERIFY(matrixRef == matrix && !std::is_heap(matrix.getZIterator(1, 2), matrix.getZIterator(3, 3)));

    // min heap, sort descending
    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 5, {32, 20,  15,  9,  8,
                         5,  4,   4,  4,  2,
                         1,  0,  -1, -2, -3,
                        -3,  -4, -5, -5, -15
                 }};

    std::sort_heap(matrix.getZIterator(0, 2), matrix.getZIterator(3, 3), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;});

    QVERIFY(matrixRef == matrix && !std::is_heap(matrix.getZIterator(0, 2), matrix.getZIterator(3, 3), [](const int& firstElement, const int& secondElement){return firstElement > secondElement;}));
}

QTEST_APPLESS_MAIN(HeapTests)

#include "tst_heaptests.moc"
