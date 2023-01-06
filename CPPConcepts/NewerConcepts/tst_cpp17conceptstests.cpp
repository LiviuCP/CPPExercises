#include <QtTest>
#include <array>
#include <type_traits>
#include <cassert>

#include "datautils.h"

class CPP17ConceptsTests : public QObject
{
    Q_OBJECT

private slots:
    void testVariableDeclarationInIf();
    void testVariableDeclarationInSwitch();
    void testStructuredBindings();
    void testConstexprIf();

private:
    template<typename DataType> auto _getSum(const Matrix<DataType>& matrix)
    {
        auto sum{0};

        if (!matrix.isEmpty())
        {
            if constexpr (std::is_integral<DataType>::value)
            {
                sum = std::accumulate(matrix.constZBegin(), matrix.constZEnd(), 0);
            }
            else
            {
                for (const auto& element : matrix)
                {
                    sum += element.size();
                }
            }
        }

        return sum;
    }
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

void CPP17ConceptsTests::testStructuredBindings()
{
    StringIntPairMatrix matrix{2, 5, {{"Alex", 4}, {"George", 8}, {"Reggie", 6}, {"Maria", 7}, {"Barron", 5},
                                      {"Anna", 8}, {"John", 12}, {"Barbie", 10}, {"Helen", 11}, {"Jack", 14}}};

    const StringIntPairMatrix c_MatrixRef{2, 5, {{"Alex", 4}, {"George", 8}, {"Micky", 12}, {"Maria", 7}, {"Robert", 5},
                                                 {"Anna", 8}, {"John", 18}, {"Barbie", 10}, {"Helen", 11}, {"Jack", 14}}};

    // scenario 1: a more expressive loop
    size_t charsCount{0};
    int totalAge{0};

    for (const auto& [name, age] : matrix)
    {
        charsCount += name.size();
        totalAge += age;
    }

    QVERIFY(50 == charsCount && 85 == totalAge);

    // scenario 2: single matrix element split into two parts (with copy)
    const auto [name, age] = matrix.at(1, 2);

    QVERIFY("Barbie" == name && 10 == age);

    // scenario 3: sample array copied from matrix
    std::array<StringIntPair, 3> matrixSample{matrix.at(0, 1), matrix.at(1, 3), matrix.at(0, 2)};
    const std::array<StringIntPair, 3> c_MatrixSampleRef{StringIntPair{"Jimmy", 8}, StringIntPair{"Helen", 14}, StringIntPair{"Jackie", 5}};

    // 3a: structured binding by copying the array elements
    const auto [firstPerson, secondPerson, thirdPerson] = matrixSample;

    QVERIFY(firstPerson.first.size() == 6 && secondPerson.second == 11 && thirdPerson.first == "Reggie");

    // 3b: structured binding by reference used for modifying the array
    auto& [firstPersonRef, secondPersonRef, thirdPersonRef] = matrixSample;

    firstPersonRef.first = "Jimmy";
    secondPersonRef.second = 14;
    thirdPersonRef.first = "Jackie";
    thirdPersonRef.second = 5;

    QVERIFY(c_MatrixSampleRef == matrixSample);

    // scenario 4: modify matrix via structured binding by reference
    std::tuple<StringIntPair&, StringIntPair&, StringIntPair&> matrixElementsTuple(matrix.at(0, 2), matrix.at(1, 1), matrix.at(0, 4));

    auto& [personOne, personTwo, personThree] = matrixElementsTuple;
    auto& [firstName, firstAge] = personOne;

    firstName = "Micky";
    firstAge = 12;
    personTwo.second = 18;
    personThree.first = "Robert";

    QVERIFY(c_MatrixRef == matrix);

    // scenario 5: modify a struct via structured binding by reference
    struct Person
    {
        std::string mName;
        int mAge;
        bool mIsMale;
    };

    Person person{"Robbie", 32, true};

    auto& [personName, personAge, personIsMale] = person;
    personName += " T.";
    personAge = 40;

    QVERIFY("Robbie T." == person.mName && 40 == person.mAge && personIsMale);
}

void CPP17ConceptsTests::testConstexprIf()
{
    const IntMatrix c_IntMatrix{3, 4, {-1, 3, 5, 4,
                                        2, 0, 0, 9,
                                        8, -7, 3, 2
                                }};

    QVERIFY(28 == _getSum(c_IntMatrix));

    const StringMatrix c_StringMatrix{2, 5, {"Alex", "George", "Micky", "Maria", "Robert",
                                             "Anna", "John", "Barbie", "Helen", "Jack"
                                      }};

    QVERIFY(49 == _getSum(c_StringMatrix));

    const BoolMatrix c_BoolMatrix{4, 3, {true,  false, true,
                                         false, false, true,
                                         true,  true, false,
                                         false, true,  true
                                  }};

    QVERIFY(7 == _getSum(c_BoolMatrix));
}

QTEST_APPLESS_MAIN(CPP17ConceptsTests)

#include "tst_cpp17conceptstests.moc"
