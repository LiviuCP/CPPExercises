#include <QtTest>
#include <cassert>

#include "datautils.h"

class CPP17ConceptsTests : public QObject
{
    Q_OBJECT

private slots:
    void testVariableDeclarationInIf();
    void testVariableDeclarationInSwitch();
};

void CPP17ConceptsTests::testVariableDeclarationInIf()
{
    IntMatrix matrix{3, 3, {-1, 5, 4,
                             0, 2, 10,
                             0, 4, 6
                     }};

    const IntMatrix c_FirstMatrixRef{3, 3, {-1, 12, 4,
                                             0, 2, 12,
                                             0, 4, 6
                                     }};

    const IntMatrix c_SecondMatrixRef{3, 3, {-1, 12, 16,
                                              0, 16, 12,
                                              16, 4, 6
                                      }};

    const IntMatrix c_ThirdMatrixRef{3, 3, -7};

    // scenario 1: one variable
    if (int value{std::accumulate(matrix.constNColumnBegin(0), matrix.constNColumnEnd(0), 0)};
        value < 0)
    {
        value = std::accumulate(matrix.constZRowBegin(1), matrix.constZRowEnd(1), 0);
        std::fill(matrix.dBegin(1), matrix.dEnd(1), value);
    }

    QVERIFY(c_FirstMatrixRef == matrix);

    // scenario 2: two constants
    if (const int firstValue{std::accumulate(matrix.constNColumnBegin(1), matrix.constNColumnEnd(1), 0)},
                  secondValue{std::accumulate(matrix.constZRowBegin(1), matrix.constZRowEnd(1), 0)};
                  firstValue > secondValue)
    {
        std::fill(matrix.mBegin(0), matrix.mEnd(0), (firstValue + secondValue) / 2);
    }

    QVERIFY(c_SecondMatrixRef == matrix);

    // scenario 3: else branching (here comes the real power: the scope of "value" covers both "if" AND "else", yet the variable is not visible outside the if-else construct)
    if (int value{std::accumulate(matrix.constDBegin(0), matrix.constDEnd(0), 0) / (matrix.constDEnd(0) - matrix.constDBegin(0))}; value > matrix.at(1, 1))
    {
        matrix.setAllItemsToValue(0);
        std::fill(matrix.dBegin(0), matrix.dEnd(0), value);
    }
    else
    {
        matrix.setAllItemsToValue(-value);
    }

    QVERIFY(c_ThirdMatrixRef == matrix);
}

void CPP17ConceptsTests::testVariableDeclarationInSwitch()
{
    enum class SizeOrder : unsigned short
    {
        SMALL = 0,
        MEDIUM,
        LARGE,
        EXTRA_LARGE
    };

    IntMatrix matrix{9, 8, -7};
    const IntMatrix c_MatrixRef{9, 8, 24};

    switch (const IntMatrix::size_type c_ElementsCount{matrix.getNrOfRows() * matrix.getNrOfColumns()};
            const SizeOrder c_SizeOrder{c_ElementsCount < 50 ? SizeOrder::SMALL
                                                             : c_ElementsCount < 90 ? SizeOrder::MEDIUM
                                                                                    : c_ElementsCount < 140 ? SizeOrder::LARGE
                                                                                                            : SizeOrder::EXTRA_LARGE})
    {
    case SizeOrder::SMALL:
        matrix.setAllItemsToValue(c_ElementsCount);
        break;
    case SizeOrder::MEDIUM:
        matrix.setAllItemsToValue(c_ElementsCount / 3);
        break;
    case SizeOrder::LARGE:
        matrix.setAllItemsToValue(c_ElementsCount / 5);
        break;
    case SizeOrder::EXTRA_LARGE:
        matrix.setAllItemsToValue(c_ElementsCount / 8);
        break;
    default:
        assert(false);
        break;
    }

    QVERIFY(c_MatrixRef == matrix);
}

QTEST_APPLESS_MAIN(CPP17ConceptsTests)

#include "tst_cpp17conceptstests.moc"
