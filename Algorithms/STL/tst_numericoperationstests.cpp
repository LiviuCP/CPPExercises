// clang-format off
#include <QTest>

#include <numeric>

#include "matrixutils.h"

class NumericOperationsTests : public QObject
{
    Q_OBJECT

public:
    NumericOperationsTests();

private slots:
    void testIota();
    void testAccumulate();
    void testInnerProduct();
    void testAdjacentDifference();
    void testPartialSum();
    void testReduce();
    void testExclusiveScan();
    void testInclusiveScan();
    void testTransformReduce();
    void testTransformExclusiveScan();
    void testTransformInclusiveScan();

private:
    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const IntMatrix mThirdIntMatrix;
    const IntMatrix mFourthIntMatrix;
};

NumericOperationsTests::NumericOperationsTests()
    : mPrimaryIntMatrix{4, 5, {-9, -7, -5, -5, -3,
                               -3, -1,  0,  1,  1,
                                1,  2,  3,  3,  4,
                                5,  7,  11, 11, 14
                        }}
    , mSecondaryIntMatrix{{4, 4}, 0}
    , mThirdIntMatrix{5, 4, {1, -2, -3, -4,
                             0, -4, -4, -4,
                            -3,  4,  4,  4,
                             4, -2,  3,  5,
                             3,  1,  4,  1
                      }}
    , mFourthIntMatrix{4, 5, {2,  8, -3,  2, -1,
                              0,  4,  5, -1,  1,
                             -1, -2,  10, 8,  7,
                              5,  7,  -4, 9, 14
                       }}
{
}

void NumericOperationsTests::testIota()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    const IntMatrix c_MatrixRef{4, 5, {-9, -7,  26, 22, -3,
                                       -3, -1,  25, 21,  1,
                                        1,  2,  24, 20,  4,
                                        5,  27, 23, 11, 14
                                 }};

    std::iota(matrix.getReverseNIterator(2, 3), matrix.getReverseNIterator(2, 1), 20);

    QVERIFY(c_MatrixRef == matrix);
}

void NumericOperationsTests::testAccumulate()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    const IntMatrix c_MatrixRef{5, 5, {-9, -7, -5, -5, -3,
                                       -3, -1,  0,  1,  1,
                                        1,  2,  3,  3,  4,
                                        5,  7,  11, 11, 14,
                                       -6,  1,  9,  10, 16
                                }};

    const matrix_size_t c_LastRowNrAfterInsert{matrix.getNrOfRows()};
    matrix.insertRow(c_LastRowNrAfterInsert);

    for(IntMatrix::ZIterator it{matrix.zRowBegin(c_LastRowNrAfterInsert)}; it != matrix.zEnd(); ++it)
    {
        *it = std::accumulate(matrix.constNColumnBegin(*it.getColumnNr()), matrix.getConstNIterator(c_LastRowNrAfterInsert, *it.getColumnNr()), 0);
    }

    QVERIFY(c_MatrixRef == matrix);

    // start with initial value (1), add each element multiplied by 2: 1 + 2 * (-3) + 2 * 1 + ... + 2 * (-6)
    int result{std::accumulate(matrix.constMBegin(0), matrix.constMEnd(0), 1, [](const int& firstElement, const int& secondElement) {return firstElement + 2 * secondElement;})};

    QVERIFY(5 == result);

    // start with initial value, multiply each partial result with 2 before adding to next element: ((((2 * 1 + (-3)) * 2 + 1) * 2 + 3) * 2 + 7) * 2 + (-6)
    result = std::accumulate(matrix.constMBegin(0), matrix.constMEnd(0), 1, [](const int& firstElement, const int& secondElement) {return 2 * firstElement + secondElement;});

    QVERIFY(12 == result);
}

void NumericOperationsTests::testInnerProduct()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    const IntMatrix c_MatrixRef{4, 4,{-23, 33,  8, 16,
                                       4,   9, 20, 22,
                                       16,  0, 26, 19,
                                       58, -2, 90, 65
                                }};

    for(IntMatrix::ZIterator it{matrix.zBegin()}; it != matrix.zEnd(); ++it)
    {
        *it = std::inner_product(mPrimaryIntMatrix.constZRowBegin(*it.getRowNr()), mPrimaryIntMatrix.constZRowEnd(*it.getRowNr()), mThirdIntMatrix.constNColumnBegin(*it.getColumnNr()), 0);
    }

    QVERIFY(c_MatrixRef == matrix);

    /* Initialize the resulting value (acc) with the init value
       Then apply: acc = f1(acc, f2(firstElement, secondElement)), where on the right hand side of the expression we have:
            acc = current result value
            firstElement, secondElement = current dereferenced inner product operands
            f1 = first lambda function (here: firstElement - 2 * secondElement)
            f2 = second lambda function (here: 2 * firstElement - secondElement)

       ---- the concrete example ----

       Init: acc = 2
       Then apply: acc = acc - 2 * f2 in each step as follows:
            acc = acc - 2 * (2 * firstElement - secondElement) =   2 - 2 * (2 * 8 - (-3))  = -36
            acc = acc - 2 * (2 * firstElement - secondElement) = -36 - 2 * (2 * 4 - 5)     = -42
            acc = acc - 2 * (2 * firstElement - secondElement) = -42 - 2 * (2 * (-2) - 10) = -14
            acc = acc - 2 * (2 * firstElement - secondElement) = -14 - 2 * (2 * 7 - (-4))  = -50
    */
    const int c_Result{std::inner_product(mFourthIntMatrix.constNColumnBegin(1),
                                          mFourthIntMatrix.constNColumnEnd(1),
                                          mFourthIntMatrix.constNColumnBegin(2),
                                          2,
                                          [](const int& firstElement, const int& secondElement){return firstElement - 2 * secondElement;},
                                          [](const int& firstElement, const int& secondElement){return 2 * firstElement - secondElement;})};

    QVERIFY(-50 == c_Result);
}

void NumericOperationsTests::testAdjacentDifference()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    IntMatrix matrixRef{4, 4,{0,  2,  1, 0,
                             -4,  1,  2, 0,
                              11, 2,  0, 0,
                             -2, -12, 0, 0
                        }};

    IntMatrix::ConstNIterator it{std::adjacent_difference(mFourthIntMatrix.getConstReverseZIterator(3, 2), mFourthIntMatrix.getConstReverseZIterator(1, 3), matrix.getNIterator(1, 0))};

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(2, 2) == it);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 4,{0,  8, 6, 0,
                       0,  8, 1, 0,
                      -4,  9, 2, 0,
                       3, 11, 0, 0
                 }};

    it = std::adjacent_difference(mFourthIntMatrix.getConstReverseZIterator(3, 2),
                                  mFourthIntMatrix.getConstReverseZIterator(1, 3),
                                  matrix.getNIterator(2, 0),
                                  [](const int& firstElement, const int& secondElement) {return 2 + (firstElement + secondElement) / 2;});

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(3, 2) == it);
}

void NumericOperationsTests::testPartialSum()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    IntMatrix matrixRef{4, 4,{0, 15, 30, 0,
                             -4, 23, 31, 0,
                              3, 33, 30, 0,
                              8, 31,  0, 0
                        }};

    IntMatrix::ConstNIterator it{std::partial_sum(mFourthIntMatrix.getConstReverseZIterator(3, 2), mFourthIntMatrix.getConstReverseZIterator(1, 2), matrix.getNIterator(1, 0))};

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(3, 2) == it);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 4,{0, 3,  8, 3,
                       0, 6, 10, 0,
                       0, 6,  5, 0,
                       9, 7,  3, 0
                 }};

    /* Each new value (partial result, cannot call it sum in this case) is calculated based on:
       - previous value (previous partial result)
       - current element
       -> to which the binary operation is applied:
            newValue = 1 + (prevValue + currentElement) / 2
       First value is the first element (copied from source).

        ---- the concrete example ----

       First partial result is the first element (copied): 9
       Then the binary operation is applied for next partial results as follows:
            second result: 1 + (9 + (-4)) / 2 => 3
            third result: 1 + (3 + 7) / 2 => 6
            fourth result: 1 + (6 + 5) / 2 => 6 ...
    */
    it = std::partial_sum(mFourthIntMatrix.getConstReverseZIterator(3, 3),
                          mFourthIntMatrix.getConstReverseZIterator(1, 3),
                          matrix.getNIterator(3, 0),
                          [](const int& firstElement, const int& secondElement) {return 1 + (firstElement + secondElement) / 2;});

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(1, 3) == it);
}

// std::reduce is similar with std::accumulate, however it might be used in parallel operations (multi-threading) by applying policy (parallel operations not contained within example)
void NumericOperationsTests::testReduce()
{
    IntMatrix matrix{mPrimaryIntMatrix};
    const IntMatrix c_MatrixRef{5, 5, {-9, -7, -5, -5, -3,
                                       -3, -1,  0,  1,  1,
                                        1,  2,  3,  3,  4,
                                        5,  7,  11, 11, 14,
                                       -6,  1,  9,  10, 16
                                }};

    const matrix_size_t c_LastRowNrAfterInsert{matrix.getNrOfRows()};
    matrix.insertRow(c_LastRowNrAfterInsert);

    // Same example as the first one provided for std::accumulate; both algorithms can be used with same results here (for std::reduce the init value is not provided - it's implicitly 0)
    for(IntMatrix::ZIterator it{matrix.zRowBegin(c_LastRowNrAfterInsert)}; it != matrix.zEnd(); ++it)
    {
        *it = std::reduce(matrix.constNColumnBegin(*it.getColumnNr()), matrix.getConstNIterator(c_LastRowNrAfterInsert, *it.getColumnNr()));
    }

    QVERIFY(c_MatrixRef == matrix);

    // additional case, use an init value
    int result{std::reduce(matrix.constMBegin(1), matrix.constMEnd(1), 4)};

    QVERIFY(6 == result);

    /* additional cases, use binary function
       (if the execution was parallelized, for predictible results the binary function should contain an operation that is either commutative or associative)
    */

    // init value and all elements multiplied: ((((5 * (-9)) * -1) * 3) * 11) * 16
    result = std::reduce(matrix.constDBegin(0), matrix.constDEnd(0), 5, [](const int& firstElement, const int& secondElement) {return firstElement * secondElement;});

    QVERIFY(23760 == result);

    // binary function applied on init value and first element, then recursively for following elements: ((((2 + (-9) - 10) + (-1) -10) + 3 - 10) + 11 - 10) + 16 - 10
    result = std::reduce(matrix.constDBegin(0), matrix.constDEnd(0), 2, [](const int& firstElement, const int& secondElement) {return firstElement + secondElement - 10;});

    QVERIFY(-28 == result);
}

void NumericOperationsTests::testExclusiveScan()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    IntMatrix matrixRef{4, 4, { 0, 18, 41, 0,
                               10, 25, 40, 0,
                                6, 33,  0, 0,
                               13, 43,  0, 0
                        }};

    IntMatrix::ConstNIterator it{std::exclusive_scan(mFourthIntMatrix.getConstReverseZIterator(3, 2), mFourthIntMatrix.getConstReverseZIterator(1, 3), matrix.getNIterator(1, 0), 10)};

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(2, 2) == it);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 4, {0,   72, 141120, 0,
                        0,  504,   0,    0,
                       -2,  2520,  0,    0,
                       -18, 17640, 0,    0
                 }};

    it = std::exclusive_scan(mFourthIntMatrix.getConstReverseZIterator(3, 3),
                             mFourthIntMatrix.getConstReverseZIterator(2, 1),
                             matrix.getNIterator(2, 0),
                             -2,
                             [](const int& firstElement, const int& secondElement) {return (firstElement * secondElement);});

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(1, 2) == it);
}

void NumericOperationsTests::testInclusiveScan()
{
    IntMatrix matrix{mSecondaryIntMatrix};
    IntMatrix matrixRef{4, 4,{0, 15, 30, 0,
                             -4, 23, 31, 0,
                              3, 33, 30, 0,
                              8, 31, 0,  0
                        }};

    // same example (and same result) as for partial_sum(), see above
    IntMatrix::ConstNIterator it{std::inclusive_scan(mFourthIntMatrix.getConstReverseZIterator(3, 2), mFourthIntMatrix.getConstReverseZIterator(1, 2), matrix.getNIterator(1, 0))};

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(3, 2) == it);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 4, {0, 1, 4, 0,
                        8, 3, -2, 0,
                        -1, 6, -6, 0,
                        1, 11, 0, 0
                 }};

    // init value and first element used for calculating the first partial result: 9 + 4 - 5 = 8
    // second partial result: 8 + (-4) - 5 = -1 etc.
    it = std::inclusive_scan(mFourthIntMatrix.getConstReverseZIterator(3, 3),
                             mFourthIntMatrix.getConstReverseZIterator(1, 3),
                             matrix.getNIterator(1, 0),
                             [](const int& firstElement, const int& secondElement) {return (firstElement + secondElement - 5);},
                             4);

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(3, 2) == it);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 4, {0, 12, 43, 0,
                        0, 21, 44, 0,
                       -4, 31, 47, 0,
                        5, 43, 48, 0
                 }};

    // first partial result is the first element: -4
    // then the binary operation is being applied recursively starting with second element: -4 + 7 + 2 = 5 ...
    it = std::inclusive_scan(mFourthIntMatrix.getConstReverseZIterator(3, 2),
                             mFourthIntMatrix.getConstReverseZIterator(1, 2),
                             matrix.getNIterator(2, 0),
                             [](const int& firstElement, const int& secondElement) {return (firstElement + secondElement + 2);});

    QVERIFY(matrixRef == matrix && matrix.getConstNIterator(0, 3) == it);
}

void NumericOperationsTests::testTransformReduce()
{
    // transform is multiplication, reduce is addition which makes the operation equivalent to inner product: 5 + (-7) * (-4) + (-1) * 10 + ...
    int result{std::transform_reduce(mPrimaryIntMatrix.constNColumnBegin(1), mPrimaryIntMatrix.constNColumnEnd(1), mFourthIntMatrix.constReverseNColumnBegin(2), 5)};

    QVERIFY(12 == result);

    // binary transform function example
    result = std::transform_reduce(mPrimaryIntMatrix.constReverseNColumnBegin(2),
                                   mPrimaryIntMatrix.constReverseNColumnEnd(2),
                                   mFourthIntMatrix.constNColumnBegin(3),
                                   -3,
                                   [](const int& firstElement, const int& secondElement) {return 1 + firstElement + secondElement;},    // reduce
                                   [](const int& firstElement, const int& secondElement) {return (firstElement + secondElement) / 2;}); // transform

    QVERIFY(14 == result); // -3 + (1 + (11 + 2) / 2) + (1 + (3 + (-1)) / 2) + ...

    // unary transform function example
    // result = 2 * init value * (mFourthIntMatrix[0][1] / 3)
    // result = 2 * result     * (mFourthIntMatrix[1][2] / 3)
    // result = 2 * result     * (mFourthIntMatrix[2][3] / 3)
    // result = 2 * result     * (mFourthIntMatrix[3][4] / 3)
    result = std::transform_reduce(mFourthIntMatrix.constDBegin(1),
                                   mFourthIntMatrix.constDEnd(1),
                                   5,
                                   [](const int& firstElement, const int& secondElement) {return 2 * firstElement * secondElement;}, // reduce
                                   [](const int& element){return element / 3;});                                                     // transform

    QVERIFY(1280 == result);
}

void NumericOperationsTests::testTransformExclusiveScan()
{
    IntMatrix matrix{mThirdIntMatrix};
    const IntMatrix c_MatrixRef{5, 4, {1, -2, 7, -4, // init: 7
                                       0, -4, 7, -4, // 7 + 2 * 1 - 2
                                      -3,  4, 3,  4, // 7 + 2 * (-1) - 2
                                       4, -2, 11, 5, // 3 + 2 * 5 - 2
                                       3,  1, 17, 1  // 11 + 2 * 4 - 2
                                }};

    // first partial result is the init value (7)
    // each new partial result: (preceding partial result (not transformed) + previous element (transformed) - 2)
    IntMatrix::ConstNIterator it{std::transform_exclusive_scan(mFourthIntMatrix.constReverseZRowBegin(1),
                                                               mFourthIntMatrix.constReverseZRowEnd(1),
                                                               matrix.nColumnBegin(2),
                                                               7,
                                                               [](const int& firstElement, const int& secondElement) {return firstElement + secondElement - 2;},
                                                               [](const int& element) {return 2 * element;})};

    QVERIFY(c_MatrixRef == matrix && matrix.constNColumnEnd(2) == it);
}

void NumericOperationsTests::testTransformInclusiveScan()
{
    IntMatrix matrix{mThirdIntMatrix};
    IntMatrix matrixRef{5, 4, {1, -2, 7, -4, // 7 + 2 * 1 - 2: init value 7 is taken into account as previous partial result ("seed")
                               0, -4, 3, -4, // 7 + 2 * (-1) - 2
                              -3,  4, 11, 4, // 3 + 2 * 5 - 2
                               4, -2, 17, 5, // 11 + 2 * 4 - 2
                               3,  1, 15, 1  // 17 + 2 * 0 - 2
                        }};

    // first partial result ("seed", not written to any element) is the init value (see matrixRef comments)
    // each new partial result is obtained as follows: (preceding partial result (not transformed) + current element (transformed) - 2)
    IntMatrix::ConstNIterator it{std::transform_inclusive_scan(mFourthIntMatrix.constReverseZRowBegin(1),
                                                               mFourthIntMatrix.constReverseZRowEnd(1),
                                                               matrix.nColumnBegin(2),
                                                               [](const int& firstElement, const int& secondElement) {return firstElement + secondElement - 2;},
                                                               [](const int& element) {return 2 * element;},
                                                               7)};

    QVERIFY(matrixRef == matrix && matrix.constNColumnEnd(2) == it);

    matrix = mSecondaryIntMatrix;
    matrixRef = {4, 4, {0,  4,  0, 0, // 1 + 7 / 2 => 4 : first element (7) is only transformed and written back (no binary operation)
                        0, -16, 0, 0, // 4 * (1 + 3 / 2) * (-2) : then binary operation is applied recursively along with transform ...
                        0,  32, 0, 0, // -16 * (1 + 1 / 2) * (-2)
                        0,  0,  0, 0  // 32 * (1 + (-3) / 2) * (-2)
                 }};

    it = std::transform_inclusive_scan(mPrimaryIntMatrix.constReverseMBegin(0),
                                       mPrimaryIntMatrix.constReverseMEnd(0),
                                       matrix.nColumnBegin(1),
                                       [](const int& firstElement, const int& secondElement) {return firstElement * secondElement * (-2);},
                                       [](const int& element) {return 1 + element / 2;});

    QVERIFY(matrixRef == matrix && matrix.constNColumnEnd(1) == it);
}

QTEST_APPLESS_MAIN(NumericOperationsTests)

#include "tst_numericoperationstests.moc"
// clang-format on
