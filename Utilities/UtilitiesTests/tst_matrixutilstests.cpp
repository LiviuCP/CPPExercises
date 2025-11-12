// clang-format off
#include <QTest>

#include "matrixutils.h"

class MatrixUtilsTests : public QObject
{
    Q_OBJECT

private slots:
    void testDiagonalIndexToRowAndColumnNrMapping();
    void testDiagonalIndexToRowAndColumnNrMappingWithEmptyMatrix();
    void testRowAndColumnNrToDiagonalIndexMapping();
    void testRowAndColumnNrToDiagonalIndexMappingWithEmptyMatrix();

    void testDiagonalIndexToRowAndColumnNrMapping_data();
    void testRowAndColumnNrToDiagonalIndexMapping_data();
};

void MatrixUtilsTests::testDiagonalIndexToRowAndColumnNrMapping()
{
    QFETCH(matrix_size_t, nrOfRows);
    QFETCH(matrix_size_t, nrOfColumns);
    QFETCH(std::vector<MatrixPoint>, expectedCoordinates);

    QVERIFY(nrOfRows > 0 && nrOfColumns > 0);

    const IntMatrix c_Matrix{{nrOfRows, nrOfColumns}, 0};
    const matrix_diff_t c_MatrixElementsCount{static_cast<matrix_diff_t>(c_Matrix.getNrOfRows()) * static_cast<matrix_diff_t>(c_Matrix.getNrOfColumns())};

    QVERIFY2(static_cast<matrix_diff_t>(expectedCoordinates.size()) == c_MatrixElementsCount,
             "Number of expected coordinates should match the number of matrix elements!");

    // the indexes of the expectedCoordinates vector elements are diagonal indexes to be converted
    for (size_t diagonalIndex{0}; diagonalIndex < expectedCoordinates.size(); ++diagonalIndex)
    {
        const auto c_Result{Utilities::mapDiagonalIndexToRowAndColumnNr(c_Matrix, static_cast<matrix_diff_t>(diagonalIndex))};
        const auto&[coordinates, convertedDiagonalIndex]{c_Result};
        const auto&[rowNr, columnNr]{coordinates};
        const auto&[expectedRowNr, expectedColumnNr]{expectedCoordinates[diagonalIndex]};

        QVERIFY(rowNr == expectedRowNr &&
                columnNr == expectedColumnNr &&
                convertedDiagonalIndex == static_cast<matrix_diff_t>(diagonalIndex));
    }

    // clamp negative diagonal index
    {
        const matrix_diff_t c_NegativeDiagonalIndex{-1};
        const auto c_Result{Utilities::mapDiagonalIndexToRowAndColumnNr(c_Matrix, c_NegativeDiagonalIndex)};
        const auto&[coordinates, convertedDiagonalIndex]{c_Result};
        const auto&[rowNr, columnNr]{coordinates};
        const auto&[expectedRowNr, expectedColumnNr]{expectedCoordinates[0]};

        QVERIFY(rowNr == expectedRowNr && columnNr == expectedColumnNr && convertedDiagonalIndex == 0);
    }

    // clamp diagonal index exceeding maximum matrix index
    {
        const matrix_diff_t c_ExceedingDiagonalIndex{c_MatrixElementsCount};
        const auto c_Result{Utilities::mapDiagonalIndexToRowAndColumnNr(c_Matrix, c_ExceedingDiagonalIndex)};
        const auto&[coordinates, convertedDiagonalIndex]{c_Result};
        const auto&[rowNr, columnNr]{coordinates};
        const auto&[expectedRowNr, expectedColumnNr]{expectedCoordinates[expectedCoordinates.size() - 1]};

        QVERIFY(rowNr == expectedRowNr && columnNr == expectedColumnNr && convertedDiagonalIndex == c_MatrixElementsCount - 1);
    }
}

void MatrixUtilsTests::testDiagonalIndexToRowAndColumnNrMappingWithEmptyMatrix()
{
    const IntMatrix c_Matrix;
    const std::vector<matrix_diff_t> c_DiagonalIndexesToCheck{-1, 0, 1};

    for (auto diagonalIndex : c_DiagonalIndexesToCheck)
    {
        const auto c_Result{Utilities::mapDiagonalIndexToRowAndColumnNr(c_Matrix, diagonalIndex)};
        const auto&[coordinates, convertedDiagonalIndex]{c_Result};
        const auto&[rowNr, columnNr]{coordinates};

        QVERIFY(!rowNr.has_value() && !columnNr.has_value() && !convertedDiagonalIndex.has_value());
    }
}

void MatrixUtilsTests::testRowAndColumnNrToDiagonalIndexMapping()
{
    QFETCH(Matrix<std::optional<matrix_diff_t>>, expectedDiagIndexesMatrix);
    QVERIFY(!expectedDiagIndexesMatrix.isEmpty());

    const matrix_size_t c_NrOfRows{expectedDiagIndexesMatrix.getNrOfRows()};
    const matrix_size_t c_NrOfColumns{expectedDiagIndexesMatrix.getNrOfColumns()};

    const IntMatrix c_Matrix{{c_NrOfRows, c_NrOfColumns}, 0}; // can be a matrix of any type, it was convenient to choose integer Matrix
    Matrix<std::optional<matrix_diff_t>> diagIndexesMatrix{{c_NrOfRows, c_NrOfColumns}, std::nullopt};

    for (auto it{diagIndexesMatrix.zBegin()}; it != diagIndexesMatrix.zEnd(); ++it)
    {
        const auto result{Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {it.getRowNr(), it.getColumnNr()})};
        const auto&[diagonalIndex, coordinates]{result};
        const auto&[rowNr, columnNr]{coordinates};

        QVERIFY(rowNr == it.getRowNr() && columnNr == it.getColumnNr());

        *it = diagonalIndex;
    }

    QVERIFY(diagIndexesMatrix == expectedDiagIndexesMatrix);

    // test clamping coordinates
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {c_NrOfRows, 0}) == Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {c_NrOfRows - 1, 0}));
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {0, c_NrOfColumns}) == Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {0, c_NrOfColumns - 1}));
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {c_NrOfRows, c_NrOfColumns}) == Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {c_NrOfRows - 1, c_NrOfColumns - 1}));

    const std::pair<std::optional<matrix_diff_t>, MatrixPoint> c_NoValue;

    // test (partially) null coordinates
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {std::nullopt, std::nullopt}) == c_NoValue);
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {std::nullopt, 0}) == c_NoValue);
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {std::nullopt, c_NrOfColumns - 1}) == c_NoValue);
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {std::nullopt, c_NrOfColumns}) == c_NoValue);
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {0, std::nullopt}) == c_NoValue);
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {c_NrOfRows - 1, std::nullopt}) == c_NoValue);
    QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, {c_NrOfRows, std::nullopt}) == c_NoValue);
}

void MatrixUtilsTests::testRowAndColumnNrToDiagonalIndexMappingWithEmptyMatrix()
{
    const IntMatrix c_Matrix;
    const std::pair<std::optional<matrix_diff_t>, MatrixPoint> c_NoValue;
    const std::vector<MatrixPoint> c_MatrixPointsToCheck{{0, 0}, {0, 1}, {1, 0}, {1, 1}, {std::nullopt, std::nullopt}, {std::nullopt, 0}, {std::nullopt, 1}, {0, std::nullopt}, {1, std::nullopt}};

    for (const auto& point : c_MatrixPointsToCheck)
    {
        QVERIFY(Utilities::mapRowAndColumnNrToDiagonalIndex(c_Matrix, point) == c_NoValue);
    }
}

void MatrixUtilsTests::testDiagonalIndexToRowAndColumnNrMapping_data()
{
    QTest::addColumn<matrix_size_t>("nrOfRows");
    QTest::addColumn<matrix_size_t>("nrOfColumns");
    QTest::addColumn<std::vector<MatrixPoint>>("expectedCoordinates");

    QTest::newRow("1: square matrix") << matrix_size_t{5u} << matrix_size_t{5u} << std::vector<MatrixPoint>{{0, 0}, {1, 0}, {0, 1}, {2, 0}, {1, 1}, {0, 2}, {3, 0}, {2, 1}, {1, 2}, {0, 3}, {4, 0}, {3, 1}, {2, 2}, {1, 3}, {0, 4}, {4, 1}, {3, 2}, {2, 3}, {1, 4}, {4, 2}, {3, 3}, {2, 4}, {4, 3}, {3, 4}, {4, 4}};
    QTest::newRow("2: less rows than columns") << matrix_size_t{5u} << matrix_size_t{6u} << std::vector<MatrixPoint>{{0, 0}, {1, 0}, {0, 1}, {2, 0}, {1, 1}, {0, 2}, {3, 0}, {2, 1}, {1, 2}, {0, 3}, {4, 0}, {3, 1}, {2, 2}, {1, 3}, {0, 4}, {4, 1}, {3, 2}, {2, 3}, {1, 4}, {0, 5}, {4, 2}, {3, 3}, {2, 4}, {1, 5}, {4, 3}, {3, 4}, {2, 5}, {4, 4}, {3, 5}, {4, 5}};
    QTest::newRow("3: more rows than columns") << matrix_size_t{6u} << matrix_size_t{5u} << std::vector<MatrixPoint>{{0, 0}, {1, 0}, {0, 1}, {2, 0}, {1, 1}, {0, 2}, {3, 0}, {2, 1}, {1, 2}, {0, 3}, {4, 0}, {3, 1}, {2, 2}, {1, 3}, {0, 4}, {5, 0}, {4, 1}, {3, 2}, {2, 3}, {1, 4}, {5, 1}, {4, 2}, {3, 3}, {2, 4}, {5, 2}, {4, 3}, {3, 4}, {5, 3}, {4, 4}, {5, 4}};
    QTest::newRow("4: row matrix") << matrix_size_t{1u} << matrix_size_t{5u} << std::vector<MatrixPoint>{{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}};
    QTest::newRow("5: column matrix") << matrix_size_t{5u} << matrix_size_t{1u} << std::vector<MatrixPoint>{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}};
    QTest::newRow("6: element matrix") << matrix_size_t{1u} << matrix_size_t{1u} << std::vector<MatrixPoint>{{0, 0}};
}

void MatrixUtilsTests::testRowAndColumnNrToDiagonalIndexMapping_data()
{
    QTest::addColumn<Matrix<std::optional<matrix_diff_t>>>("expectedDiagIndexesMatrix");

    QTest::newRow("1: square matrix") << Matrix<std::optional<matrix_diff_t>>{5, 5, {
                                                                                        {0}, {2}, {5}, {9}, {14},
                                                                                        {1}, {4}, {8}, {13}, {18},
                                                                                        {3}, {7}, {12}, {17}, {21},
                                                                                        {6}, {11}, {16}, {20}, {23},
                                                                                        {10}, {15}, {19}, {22}, {24}
                                                                              }};

    QTest::newRow("2: less rows than columns") << Matrix<std::optional<matrix_diff_t>>{5, 6, {
                                                                                                 {0}, {2}, {5}, {9}, {14}, {19},
                                                                                                 {1}, {4}, {8}, {13}, {18}, {23},
                                                                                                 {3}, {7}, {12}, {17}, {22}, {26},
                                                                                                 {6}, {11}, {16}, {21}, {25}, {28},
                                                                                                 {10}, {15}, {20}, {24}, {27}, {29}
                                                                                       }};

    QTest::newRow("3: more rows than columns") << Matrix<std::optional<matrix_diff_t>>{6, 5, {
                                                                                                 {0}, {2}, {5}, {9}, {14},
                                                                                                 {1}, {4}, {8}, {13}, {19},
                                                                                                 {3}, {7}, {12}, {18}, {23},
                                                                                                 {6}, {11}, {17}, {22}, {26},
                                                                                                 {10}, {16}, {21}, {25}, {28},
                                                                                                 {15}, {20}, {24}, {27}, {29}
                                                                                       }};

    QTest::newRow("4: row matrix") << Matrix<std::optional<matrix_diff_t>>{1, 5, {
                                                                                     {0}, {1}, {2}, {3}, {4}
                                                                           }};

    QTest::newRow("5: column matrix") << Matrix<std::optional<matrix_diff_t>>{5, 1, {
                                                                                        {0},
                                                                                        {1},
                                                                                        {2},
                                                                                        {3},
                                                                                        {4}
                                                                              }};

    QTest::newRow("6: element matrix") << Matrix<std::optional<matrix_diff_t>>{1, 1, {{0}}};
}

QTEST_APPLESS_MAIN(MatrixUtilsTests)

#include "tst_matrixutilstests.moc"
// clang-format on
