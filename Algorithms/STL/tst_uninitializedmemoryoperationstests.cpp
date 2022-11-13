#include <QtTest>

#include <memory>

#include "datautils.h"


class UninitializedMemoryOperationsTests : public QObject
{
    Q_OBJECT

public:
    UninitializedMemoryOperationsTests();

private slots:
    void testUninitializedCopy();
    void testUninitializedCopyN();
    void testUninitializedFill();
    void testUninitializedFillN();
    void testUninitializedMove();
    void testUninitializedMoveN();
    void testUninitializedDefaultConstruct();
    void testUninitializedDefaultConstructN();
    void testUninitializedValueConstruct();
    void testUninitializedValueConstructN();
    void testDestroy();
    void testDestroyN();
    void testDestroyAt();
    void testConstructAt();

private:
    struct TestData1
    {
    public:
        TestData1()
            : mValue{mDefaultValue}
        {
        }

        TestData1(int firstValue, int secondValue)
            : mValue{(firstValue + secondValue) / 2}
        {
        }

        ~TestData1()
        {
            mValue = mValueOnDestruction;
        }

        int mValue;

    private:
        static const int mDefaultValue{-101};
        static const int mValueOnDestruction{50};
    };

    struct TestData2
    {
    public:
        int mIntValue;
        std::string mStrValue;
    };

    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
    const StringIntPairMatrix mSecondaryStringIntPairMatrix;
    const StringIntPairVector mPrimaryStringIntPairVector;
};

UninitializedMemoryOperationsTests::UninitializedMemoryOperationsTests()
    : mPrimaryIntMatrix{3, 4, {-1, 1, 1, 0,
                                2, 4, 5, 1,
                                3, -3, -3, 2
                        }}
    , mSecondaryIntMatrix{4, 5, {-1, 1, 1, 0, 2,
                                  4, 9, 2, 5, -3,
                                 -3, 2, 4, 2, 4,
                                  0, 1, 3, 5, -2
                          }}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                         {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}
                                  }}
    , mSecondaryStringIntPairMatrix{2, 5, {{"Anna", 18}, {"Kelly", 12}, {"Annabel", 11}, {"Juan", 10}, {"Jack", 8},
                                         {"Barbara", 10}, {"Barney", 20}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                    }}
    , mPrimaryStringIntPairVector{{"Alex", 10}, {"Kevin", 11}, {"Cameron", 10}, {"Alistair", 10}, {"George", 14}, {"Mark", 9}}
{
}

void UninitializedMemoryOperationsTests::testUninitializedCopy()
{
    const size_t c_UninitializedElementsCount{static_cast<size_t>(mSecondaryIntMatrix.constNColumnEnd(3) - mSecondaryIntMatrix.constNColumnBegin(2))};
    int* pFirst{static_cast<int*>(std::malloc(c_UninitializedElementsCount * sizeof(int)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    // just to show memory is uninitialized (a warning might also trigger)
    qDebug("First element before initialization: %d", *pFirst);

    try
    {
        int* const pLast{std::uninitialized_copy(mSecondaryIntMatrix.constNColumnBegin(2), mSecondaryIntMatrix.constNColumnEnd(3), pFirst)};

        if (pLast != pFirst + c_UninitializedElementsCount)
        {
            FAIL_DEALLOC(pFirst, "std::uninitialized_copy() returned invalid iterator");
        }

        IntMatrix matrix{mPrimaryIntMatrix};
        const IntMatrix c_MatrixRef{3, 4, {-1, 1, 1, 0,
                                            1, 2, 4, 3,
                                            0, 5, 2, 5
                                    }};

        IntMatrix::ZIterator it{matrix.zRowBegin(1)};

        for (int* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = *pIt;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedCopyN()
{
    const size_t c_UninitializedElementsCount{4};
    StringIntPair* pFirst{static_cast<StringIntPair*>(std::aligned_alloc(alignof(StringIntPair), c_UninitializedElementsCount * sizeof(StringIntPair)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    // just to show memory is uninitialized (a warning might also trigger)
    qDebug("First element before initialization: (%s, %d)", pFirst->first.c_str(), pFirst->second);

    try
    {
        StringIntPair* const pLast{std::uninitialized_copy_n(mPrimaryStringIntPairMatrix.getConstReverseZIterator(1, 2), 4, pFirst)};

        if (pLast != pFirst + c_UninitializedElementsCount)
        {
            FAIL_DEALLOC(pFirst, "std::uninitialized_copy_n() returned invalid iterator");
        }

        StringIntPairMatrix matrix{mSecondaryStringIntPairMatrix};
        const StringIntPairMatrix c_MatrixRef{2, 5, {{"Anna", 18}, {"Kelly", 12}, {"Annabel", 11}, {"Reggie", 12}, {"Cameron", 10},
                                                    {"Andrew", 11}, {"Mark", 9}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                               }};

        StringIntPairMatrix::ZIterator it{matrix.getZIterator(0, 3)};

        for (StringIntPair* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = *pIt;
        }

        std::destroy(pFirst, pLast);

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch (...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedFill()
{
    const size_t c_UninitializedElementsCount{static_cast<size_t>(mSecondaryIntMatrix.constNColumnEnd(3) - mSecondaryIntMatrix.constNColumnBegin(2))};
    int* pFirst{static_cast<int*>(std::malloc(c_UninitializedElementsCount * sizeof(int)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    // just to show memory is uninitialized (a warning might also trigger)
    qDebug("First element before initialization: %d", *pFirst);

    try
    {
        int* const pLast{pFirst + c_UninitializedElementsCount};
        std::uninitialized_fill(pFirst, pLast, 20);

        IntMatrix matrix{mPrimaryIntMatrix};
        const IntMatrix c_MatrixRef{3, 4, {-1, 1, 1, 0,
                                            20, 20, 20, 20,
                                            20, 20, 20, 20
                                    }};

        IntMatrix::ZIterator it{matrix.zRowBegin(1)};

        for (int* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = *pIt;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedFillN()
{
    const size_t c_UninitializedElementsCount{4};
    StringIntPair* pFirst{static_cast<StringIntPair*>(std::aligned_alloc(alignof(StringIntPair), c_UninitializedElementsCount * sizeof(StringIntPair)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    // just to show memory is uninitialized (a warning might also trigger)
    qDebug("First element before initialization: (%s, %d)", pFirst->first.c_str(), pFirst->second);

    try
    {
        std::uninitialized_fill_n(pFirst, c_UninitializedElementsCount, StringIntPair{"RESERVED", 0});

        StringIntPairMatrix matrix{mSecondaryStringIntPairMatrix};
        const StringIntPairMatrix c_MatrixRef{2, 5, {{"Anna", 18}, {"Kelly", 12}, {"Annabel", 11}, {"RESERVED", 0}, {"RESERVED", 0},
                                                    {"RESERVED", 0}, {"RESERVED", 0}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                               }};

        StringIntPairMatrix::ZIterator it{matrix.getZIterator(0, 3)};
        StringIntPair* const pLast{pFirst + c_UninitializedElementsCount};

        for (StringIntPair* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = *pIt;
        }

        std::destroy(pFirst, pLast);

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch (...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedMove()
{
    const size_t c_UninitializedElementsCount{static_cast<size_t>(mSecondaryIntMatrix.constNColumnEnd(2) - mSecondaryIntMatrix.constNColumnBegin(2))};
    int* pFirst{static_cast<int*>(std::malloc(c_UninitializedElementsCount * sizeof(int)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    // just to show memory is uninitialized (a warning might also trigger)
    qDebug("First element before initialization: %d", *pFirst);

    try
    {
        IntMatrix srcMatrix{mSecondaryIntMatrix};
        const IntMatrix c_SrcMatrixRef{4, 4, {-1, 1, 0,  2,
                                               4, 9, 5, -3,
                                              -3, 2, 2,  4,
                                               0, 1, 5, -2
                                       }};
        int* const pLast{std::uninitialized_move(srcMatrix.constNColumnBegin(2), srcMatrix.constNColumnEnd(2), pFirst)};

        if (pLast != pFirst + c_UninitializedElementsCount)
        {
            FAIL_DEALLOC(pFirst, "std::uninitialized_move() returned invalid iterator");
        }

        srcMatrix.eraseColumn(2);

        IntMatrix destMatrix{mPrimaryIntMatrix};
        const IntMatrix c_DestMatrixRef{3, 4, {-1,  1,  1, 0,
                                                3,  4,  2, 1,
                                                3, -3, -3, 2
                                        }};

        IntMatrix::ReverseZIterator it{destMatrix.reverseZRowBegin(1)};

        for (int* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = *pIt;
        }

        DEALLOC(pFirst);

        QVERIFY(c_SrcMatrixRef == srcMatrix && c_DestMatrixRef == destMatrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedMoveN()
{
    const size_t c_UninitializedElementsCount{2};
    StringIntPair* pFirst{static_cast<StringIntPair*>(std::aligned_alloc(alignof(StringIntPair), c_UninitializedElementsCount * sizeof(StringIntPair)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    // just to show memory is uninitialized (a warning might also trigger)
    qDebug("First element before initialization: (%s, %d)", pFirst->first.c_str(), pFirst->second);

    try
    {
#ifdef MACOS
        StringIntPairMatrix srcMatrix{mPrimaryStringIntPairMatrix};
        const StringIntPairMatrix c_SrcMatrixRef{2, 4, {{"Alex", 10}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                                        {"Andrew", 11}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}
                                                 }};

        const std::pair<StringIntPairMatrix::ReverseNIterator, StringIntPair*> moveResult{std::uninitialized_move_n(srcMatrix.reverseNColumnBegin(1), 2, pFirst)};
#else
        StringIntPairVector srcVector{mPrimaryStringIntPairVector};
        const StringIntPairVector c_SrcVectorRef{{"Alex", 10}, {"Alistair", 10}, {"George", 14}, {"Mark", 9}};

        const std::pair<StringIntPairVector::reverse_iterator, StringIntPair*> moveResult{std::uninitialized_move_n(srcVector.rbegin() + 3, 2, pFirst)};
#endif

        StringIntPair* const pLast{moveResult.second};

#ifdef MACOS
        if (srcMatrix.getConstReverseNIterator(1, 0) != moveResult.first || pLast != pFirst + c_UninitializedElementsCount)
        {
            FAIL_DEALLOC(pFirst, "std::uninitialized_move_n() returned invalid iterator pair");
        }

        srcMatrix.eraseColumn(1);
#else
        if (srcVector.rend() - 1 != moveResult.first || pLast != pFirst + c_UninitializedElementsCount)
        {
            FAIL_DEALLOC(pFirst, "std::uninitialized_move_n() returned invalid iterator pair");
        }

        srcVector.erase(srcVector.cbegin() + 1, srcVector.cbegin() + 3);
#endif

        StringIntPairMatrix destMatrix{mSecondaryStringIntPairMatrix};
        const StringIntPairMatrix c_DestMatrixRef{2, 6, {{"Anna", 18}, {"Kelly", 12}, {"Cameron", 10}, {"Annabel", 11}, {"Juan", 10}, {"Jack", 8},
                                                         {"Barbara", 10}, {"Barney", 20}, {"Kevin", 11}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                                  }};

        destMatrix.insertColumn(2);

        StringIntPairMatrix::NIterator it{destMatrix.nColumnBegin(2)};

        for (StringIntPair* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = *pIt;
        }

        std::destroy(pFirst, pLast);

        DEALLOC(pFirst);

#ifdef MACOS
        QVERIFY(c_SrcMatrixRef == srcMatrix && c_DestMatrixRef == destMatrix);
#else
        QVERIFY(c_SrcVectorRef == srcVector && c_DestMatrixRef == destMatrix);
#endif
    }
    catch (...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedDefaultConstruct()
{
    const size_t c_UninitializedElementsCount{4};
    TestData1* pFirst{static_cast<TestData1*>(std::malloc(c_UninitializedElementsCount * sizeof(TestData1)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData1* const pLast{pFirst + c_UninitializedElementsCount};
        std::uninitialized_default_construct(pFirst, pLast);

        IntMatrix matrix{mSecondaryIntMatrix};
        const IntMatrix c_MatrixRef{4, 5, {-1,  1,  1, -101, 2,
                                            4,  9, -101, 5, -3,
                                           -3, -101, 4,  2,  4,
                                           -101, 1,  3,  5, -2
                                    }};

        IntMatrix::MIterator it{matrix.mBegin(1)};

        for (TestData1* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = pIt->mValue;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedDefaultConstructN()
{
    const size_t c_UninitializedElementsCount{3};
    TestData2* pFirst{static_cast<TestData2*>(std::aligned_alloc(alignof(TestData2), c_UninitializedElementsCount * sizeof(TestData2)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData2* const pLast{pFirst + c_UninitializedElementsCount};

        StringIntPairMatrix matrix{mPrimaryStringIntPairMatrix};
        StringIntPairMatrix::ZIterator it{matrix.zRowBegin(1)};

        for (TestData2* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            qDebug("Matrix element before modification: (%s, %d)", it->first.c_str(), it->second);

            it->second = pIt->mIntValue;

            // we don't copy the uninitialized string object as this might trigger an exception (however we know that default_construct initializes an empty string)
            (it->first).clear();

            qDebug("Matrix element after modification: (%s, %d)", it->first.c_str(), it->second);

            ++it;
        }

        std::uninitialized_default_construct_n(pFirst, c_UninitializedElementsCount);

        it = matrix.zRowBegin(1);

        for (TestData2* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            if (pIt->mIntValue != it->second || pIt->mStrValue != it->first)
            {
                FAIL_DEALLOC(pFirst, "Error! The uninitialized objects should not be modified by std::uninitialized_default_construct_n");
            }

            ++it;
        }

        std::destroy(pFirst, pLast);
        DEALLOC(pFirst);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedValueConstruct()
{
    const size_t c_UninitializedElementsCount{4};
    TestData1* pFirst{static_cast<TestData1*>(std::malloc(c_UninitializedElementsCount * sizeof(TestData1)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData1* const pLast{pFirst + c_UninitializedElementsCount};
        std::uninitialized_value_construct(pFirst, pLast);

        IntMatrix matrix{mSecondaryIntMatrix};

        // if a default constructor is defined by user then same result is obtained as when applying std::uninitialized_default_construct(_n)
        const IntMatrix c_MatrixRef{4, 5, {-1,  1,  1, -101, 2,
                                            4,  9, -101, 5, -3,
                                           -3, -101, 4,  2,  4,
                                           -101, 1,  3,  5, -2
                                    }};

        IntMatrix::MIterator it{matrix.mBegin(1)};

        for (TestData1* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = pIt->mValue;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testUninitializedValueConstructN()
{
    const size_t c_UninitializedElementsCount{3};
    TestData2* pFirst{static_cast<TestData2*>(std::aligned_alloc(alignof(TestData2), c_UninitializedElementsCount * sizeof(TestData2)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData2* const pLast{pFirst + c_UninitializedElementsCount};
        std::uninitialized_value_construct_n(pFirst, c_UninitializedElementsCount);

        StringIntPairMatrix matrix{mPrimaryStringIntPairMatrix};
        const StringIntPairMatrix c_MatrixRef{2, 5, {{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George",  14}, {"Mark",  9},
                                                     {"",      0},  {"",      0}, {"",          0}, {"Patrick", 14}, {"John", 11}
                                              }};

        StringIntPairMatrix::ZIterator it{matrix.zRowBegin(1)};

        for (TestData2* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            it->first = pIt->mStrValue;
            it->second = pIt->mIntValue;

            ++it;
        }

        std::destroy(pFirst, pLast);
        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testDestroy()
{
    const size_t c_UninitializedElementsCount{4};
    TestData1* pFirst{static_cast<TestData1*>(std::malloc(c_UninitializedElementsCount * sizeof(TestData1)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData1* const pLast{pFirst + c_UninitializedElementsCount};

        /* This is just a proof that std::destroy works correctly.
           In the everyday work it should obviously not be used on uninitialized memory (or to set member variables)! */
        std::destroy(pFirst, pLast);

        IntMatrix matrix{mSecondaryIntMatrix};
        const IntMatrix c_MatrixRef{4, 5, {-1,  1,  1, 50, 2,
                                            4,  9, 50, 5, -3,
                                           -3, 50, 4,  2,  4,
                                           50, 1,  3,  5, -2
                                    }};

        IntMatrix::MIterator it{matrix.mBegin(1)};

        for (TestData1* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = pIt->mValue;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testDestroyN()
{
    const size_t c_UninitializedElementsCount{4};
    const size_t c_ElementsToDestroyCount{2};
    TestData1* pFirst{static_cast<TestData1*>(std::malloc(c_UninitializedElementsCount * sizeof(TestData1)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData1* const pLast{pFirst + c_UninitializedElementsCount};
        std::uninitialized_default_construct(pFirst, pLast);
        std::destroy_n(pFirst + 1, c_ElementsToDestroyCount);

        IntMatrix matrix{mSecondaryIntMatrix};
        const IntMatrix c_MatrixRef{4, 5, {-1, -101, 1,  0,  2,
                                            4,   9,  50, 5, -3,
                                           -3,   2,  4,  50, 4,
                                            0,   1,  3,  5, -101
                                    }};

        IntMatrix::DIterator it{matrix.dBegin(1)};

        for (TestData1* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = pIt->mValue;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testDestroyAt()
{
    const size_t c_UninitializedElementsCount{4};
    const size_t c_ElementToDestroyIndex{2};
    TestData1* pFirst{static_cast<TestData1*>(std::malloc(c_UninitializedElementsCount * sizeof(TestData1)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData1* const pLast{pFirst + c_UninitializedElementsCount};
        std::uninitialized_default_construct(pFirst, pLast);
        std::destroy_at(pFirst + c_ElementToDestroyIndex);

        IntMatrix matrix{mSecondaryIntMatrix};
        const IntMatrix c_MatrixRef{4, 5, {-1, -101, 1,   0,  2,
                                            4,   9, -101, 5, -3,
                                           -3,   2,  4,   50, 4,
                                            0,   1,  3,   5, -101
                                    }};

        IntMatrix::DIterator it{matrix.dBegin(1)};

        for (TestData1* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = pIt->mValue;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch(...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

void UninitializedMemoryOperationsTests::testConstructAt()
{
    const size_t c_UninitializedElementsCount{static_cast<size_t>(mSecondaryIntMatrix.getNrOfRows())};
    TestData1* pFirst{static_cast<TestData1*>(std::malloc(c_UninitializedElementsCount * sizeof(TestData1)))};

    if (!pFirst)
    {
        QFAIL("Alloc failed!");
    }

    try
    {
        TestData1* const pLast{pFirst + c_UninitializedElementsCount};

        IntMatrix::ConstNIterator firstIt{mSecondaryIntMatrix.constNColumnBegin(1)};
        IntMatrix::ConstNIterator secondIt{mSecondaryIntMatrix.constNColumnBegin(2)};

        for (TestData1* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            std::construct_at(pIt, *firstIt, *secondIt);

            ++firstIt;
            ++secondIt;
        }

        IntMatrix matrix{mPrimaryIntMatrix};
        const IntMatrix c_MatrixRef{3, 4, {-1, 1, 1, 0,
                                            1, 5, 3, 2,
                                            3, -3, -3, 2
                                    }};

        IntMatrix::ZIterator it{matrix.zRowBegin(1)};

        for (TestData1* pIt{pFirst}; pIt != pLast; ++pIt)
        {
            *it++ = pIt->mValue;
        }

        DEALLOC(pFirst);

        QVERIFY(c_MatrixRef == matrix);
    }
    catch (...)
    {
        FAIL_DEALLOC(pFirst, "Exception thrown!");
    }
}

QTEST_APPLESS_MAIN(UninitializedMemoryOperationsTests)

#include "tst_uninitializedmemoryoperationstests.moc"
