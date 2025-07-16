#include <QTest>

#include <algorithm>

#include "datautils.h"

class PartitioningTests : public QObject
{
    Q_OBJECT

public:
    PartitioningTests();

private slots:
    void testIsPartitioned();
    void testPartition();
    void testPartitionCopy();
    void testStablePartition();
    void testPartitionPoint();

private:
    const IntMatrix mPrimaryIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
    const StringIntPairMatrix mSecondaryStringIntPairMatrix;
};

PartitioningTests::PartitioningTests()
    : mPrimaryIntMatrix{4, 5, {-1, 1, -1, 0, -3,
                                2, 4, 5, -2, 3,
                                3, 3, -3, -4, -2,
                               -5, -5, 2, -1, 4
                        }}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Alex", 10}, {"Alistair", 10}, {"Mark", 9}, {"Kevin", 11}, {"George", 14},
                                         {"Andrew", 11}, {"Reggie", 12}, {"Cameron", 10}, {"Patrick", 14}, {"John", 11}
                                  }}
    , mSecondaryStringIntPairMatrix{2, 5, {{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                           {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}
                                    }}
{
}

void PartitioningTests::testIsPartitioned()
{
    QVERIFY(std::is_partitioned(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZRowEnd(0), [](StringIntPair element) {return element.second < 11;}));
    QVERIFY(!std::is_partitioned(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZRowEnd(0), [](StringIntPair element) {return element.first.size() < 5;}));
    QVERIFY(std::is_partitioned(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return 6 == element.first.size();}));
    QVERIFY(!std::is_partitioned(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return element.second < 14;}));
}

void PartitioningTests::testPartition()
{
    IntMatrix matrix{mPrimaryIntMatrix};

    IntMatrix::ConstZIterator partitioningIt{std::partition(matrix.zBegin(), matrix.zEnd(), [](int element) {return element > 0;})};
    const std::pair<matrix_size_t, matrix_size_t> c_RequiredCoordinates{1, 4};

    bool isValidPartitioning{c_RequiredCoordinates.first == partitioningIt.getRowNr() && c_RequiredCoordinates.second == partitioningIt.getColumnNr()};

    for (IntMatrix::ConstZIterator it{matrix.constZBegin()}; it != partitioningIt; ++it)
    {
        isValidPartitioning = isValidPartitioning && (*it > 0);
    }

    for (IntMatrix::ConstZIterator it{partitioningIt}; it != matrix.constZEnd(); ++it)
    {
        isValidPartitioning = isValidPartitioning && (*it <= 0);
    }

    IntPair initialElementsSumProd{0, 1};

    for (const auto& element : mPrimaryIntMatrix)
    {
        initialElementsSumProd.first += element;
        initialElementsSumProd.second *= element;
    }

    IntPair newElementsSumProd{0, 1};

    for (const auto& element : matrix)
    {
        newElementsSumProd.first += element;
        newElementsSumProd.second *= element;
    }

    isValidPartitioning = isValidPartitioning && (newElementsSumProd == initialElementsSumProd);

    QVERIFY(isValidPartitioning);
}

void PartitioningTests::testPartitionCopy()
{
    StringIntPairMatrix sourceMatrix{mSecondaryStringIntPairMatrix};
    const matrix_size_t c_DestinationElementsCount{sourceMatrix.getNrOfColumns()};

    StringIntPairMatrix firstDestinationMatrix{{c_DestinationElementsCount, 1}, {"Zorba", 100}};
    StringIntPairMatrix secondDestinationMatrix{{1, c_DestinationElementsCount}, {"Zorba's brother", 101}};

    std::partition_copy(sourceMatrix.constZBegin(), sourceMatrix.constZRowEnd(0), firstDestinationMatrix.nBegin(), secondDestinationMatrix.zBegin(), [](StringIntPair element) {return element.second < 11;});

    std::sort(firstDestinationMatrix.zBegin(), firstDestinationMatrix.zEnd());
    std::sort(secondDestinationMatrix.zBegin(), secondDestinationMatrix.zEnd(), [](StringIntPair firstElement, StringIntPair secondElement) {return firstElement.second < secondElement.second;});

    const StringIntPairMatrix c_FirstDestinationMatrixRef{c_DestinationElementsCount, 1, {{"Alex", 10}, {"Alistair", 10}, {"Mark", 9}, {"Zorba", 100}, {"Zorba", 100}}};
    const StringIntPairMatrix c_SecondDestinationMatrixRef{1, c_DestinationElementsCount, {{"Kevin", 11}, {"George", 14}, {"Zorba's brother", 101}, {"Zorba's brother", 101}, {"Zorba's brother", 101}}};

    QVERIFY(c_FirstDestinationMatrixRef == firstDestinationMatrix && c_SecondDestinationMatrixRef == secondDestinationMatrix);
}

void PartitioningTests::testStablePartition()
{
    IntMatrix firstMatrix{mPrimaryIntMatrix};
    const IntMatrix c_FirstMatrixRef{4, 5, {4, 1, -1, 3, -3,
                                            2, -1, 5, -2, 0,
                                            3, 3, -3, 4, -2,
                                           -5, -5, 2, -1, -4
                                     }};

    for (matrix_diff_t diagonalNr{1 - static_cast<matrix_diff_t>(firstMatrix.getNrOfRows())}; diagonalNr < static_cast<matrix_diff_t>(firstMatrix.getNrOfColumns()); ++diagonalNr)
    {
        std::stable_partition(firstMatrix.dBegin(diagonalNr), firstMatrix.dEnd(diagonalNr), [](int element) {return element > 0;});
    }

    QVERIFY(c_FirstMatrixRef == firstMatrix);

    StringIntPairMatrix secondMatrix{mSecondaryStringIntPairMatrix};
    const StringIntPairMatrix c_SecondMatrixRef{mPrimaryStringIntPairMatrix};

    std::stable_partition(secondMatrix.zBegin(), secondMatrix.zRowEnd(0), [](StringIntPair element) {return element.second < 11;});
    std::stable_partition(secondMatrix.zRowBegin(1), secondMatrix.zEnd(), [](StringIntPair element) {return 6 == element.first.size();});

    QVERIFY(c_SecondMatrixRef == secondMatrix);
}

void PartitioningTests::testPartitionPoint()
{
    QVERIFY(mPrimaryStringIntPairMatrix.getConstZIterator(0,3) == std::partition_point(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZRowEnd(0), [](StringIntPair element) {return element.second < 11;}));
    QVERIFY(mPrimaryStringIntPairMatrix.getConstZIterator(1,2) == std::partition_point(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return 6 == element.first.size();}));
    QVERIFY(mPrimaryStringIntPairMatrix.constZEnd() == std::partition_point(mPrimaryStringIntPairMatrix.constZBegin(), mPrimaryStringIntPairMatrix.constZEnd(), [](StringIntPair element) {return element.second > 8;}));
    QVERIFY(mPrimaryStringIntPairMatrix.getConstZIterator(1,2) == std::partition_point(mPrimaryStringIntPairMatrix.constZRowBegin(1), mPrimaryStringIntPairMatrix.getConstZIterator(1,2), [](StringIntPair element) {return 6 == element.first.size();}));
}

QTEST_APPLESS_MAIN(PartitioningTests)

#include "tst_partitioningtests.moc"
