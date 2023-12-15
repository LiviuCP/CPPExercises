#include <QtTest>
#include <QString>

#include <array>
#include <string_view>
#include <any>
#include <optional>
#include <type_traits>
#include <cassert>
#include <climits>

#include "variadictemplates.h"
#include "datautils.h"

using namespace std::literals;
namespace vrd = Variadic;

class CPP17ConceptsTests : public QObject
{
    Q_OBJECT

private slots:
    void testVariableDeclarationInIf();
    void testVariableDeclarationInSwitch();
    void testStructuredBindings();
    void testConstexprIf();
    void testConstexprIfIsSame();
    void testStdApply();
    void testStdAny();
    void testStdSize();
    void testStdOptional();
    void testStdStringView();
    void testTemplateTypeDeductionInConstructors();
    void testFoldExpressionsBinaryUnaryLeftRight();
    void testFoldExpressionsAlternativeRecursiveImplementations(); // equivalent to previous test, this time with recursion
    void testFoldExpressionsCommaOperator();

private:
    enum class DataTypes
    {
        BOOLEAN,
        CHARACTER,
        SIGNED_INTEGER,
        UNSIGNED_INTEGER,
        DECIMAL,
        STRING,
        STRING_VIEW,
        CSTYLESTRING,
        STRINGINTPAIR,
        INTMATRIX,
        UNKNOWN,
    };

    template<typename DataType> auto _getSum(const Matrix<DataType>& matrix) const;
    template<typename DataType> DataTypes _getType(DataType myType) const;
    template<typename DataType> std::optional<typename Matrix<DataType>::size_type> _getLowestDiagonalNr(const Matrix<DataType> matrix);
    template<typename DataType> std::optional<typename Matrix<DataType>::size_type> _getHighestDiagonalNr(const Matrix<DataType> matrix);

    class RawContainer
    {
    public:
        RawContainer() : mPtr{nullptr}, mLength{0} {}
        RawContainer(size_t length) : mLength{length} {mPtr = std::malloc(length);}
        ~RawContainer() {free(mPtr); mLength = 0;}

        size_t size() const {return mLength;}

    private:
        void* mPtr;
        size_t mLength;
    };

    class IntMatrixWrapper
    {
    public:
        IntMatrixWrapper() {}
        IntMatrixWrapper(int nrOfRows, int nrOfColumns, int value) : mIntMatrix{nrOfRows, nrOfColumns, value} {}

        // it is possible to also use signed size with std::size
        ssize_t size() const {return mIntMatrix.getNrOfRows() * mIntMatrix.getNrOfColumns();}

    private:
        IntMatrix mIntMatrix;
    };

    class StringMatrixWrapper
    {
    public:
        StringMatrixWrapper() {}
        StringMatrixWrapper(int nrOfRows, int nrOfColumns, std::string value) : mStringMatrix{nrOfRows, nrOfColumns, value} {}

        // a pair can also be used with std::size
        SizePair size() const {return {static_cast<size_t>(mStringMatrix.getNrOfRows()), static_cast<size_t>(mStringMatrix.getNrOfColumns())};}

    private:
        StringMatrix mStringMatrix;
    };
};

template<typename DataType> auto CPP17ConceptsTests::_getSum(const Matrix<DataType>& matrix) const
{
    if constexpr (std::is_integral<DataType>::value)
    {
        return std::accumulate(matrix.constZBegin(), matrix.constZEnd(), 0);
    }
    else
    {
        size_t sum{0};

        for (const auto& element : matrix)
        {
            sum += element.size();
        }

        return sum;
    }
}

template<typename DataType> CPP17ConceptsTests::DataTypes CPP17ConceptsTests::_getType(DataType myType) const
{
    (void)myType; // this function only needs to check the type, value is discarded

    if constexpr (std::is_same_v<DataType, bool>)
    {
        return DataTypes::BOOLEAN;
    }
    else if constexpr (std::is_same_v<DataType, char>)
    {
        return DataTypes::CHARACTER;
    }
    else if constexpr (std::is_same_v<DataType, int> ||
                       std::is_same_v<DataType, short> ||
                       std::is_same_v<DataType, long> ||
                       std::is_same_v<DataType, long long>)
    {
        return DataTypes::SIGNED_INTEGER;
    }
    else if constexpr (std::is_same_v<DataType, unsigned int> ||
                       std::is_same_v<DataType, unsigned char> ||
                       std::is_same_v<DataType, unsigned short> ||
                       std::is_same_v<DataType, unsigned long> ||
                       std::is_same_v<DataType, unsigned long long>)
    {
        return DataTypes::UNSIGNED_INTEGER;
    }
    else if constexpr (std::is_same_v<DataType, float> ||
                       std::is_same_v<DataType, double>)
    {
        return DataTypes::DECIMAL;
    }
    else if constexpr (std::is_same_v<DataType, std::string>)
    {
        return DataTypes::STRING;
    }
    else if constexpr(std::is_same_v<DataType, std::string_view>)
    {
        return DataTypes::STRING_VIEW;
    }
    else if constexpr (std::is_same_v<DataType, const char*>)
    {
        return DataTypes::CSTYLESTRING;
    }
    else if constexpr(std::is_same_v<DataType, StringIntPair>)
    {
        return DataTypes::STRINGINTPAIR;
    }
    else if constexpr(std::is_same_v<DataType, IntMatrix>)
    {
        return DataTypes::INTMATRIX;
    }
    else
    {
        return DataTypes::UNKNOWN;
    }
}

template<typename DataType> std::optional<typename Matrix<DataType>::size_type> CPP17ConceptsTests::_getLowestDiagonalNr(const Matrix<DataType> matrix)
{
    return !matrix.isEmpty() ? std::optional<typename Matrix<DataType>::size_type>{1 - matrix.getNrOfRows()} : std::nullopt;
}

template<typename DataType> std::optional<typename Matrix<DataType>::size_type> CPP17ConceptsTests::_getHighestDiagonalNr(const Matrix<DataType> matrix)
{
    return !matrix.isEmpty() ? std::optional<typename Matrix<DataType>::size_type>{matrix.getNrOfColumns() - 1} : std::nullopt;
}

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

    const Matrix<IntVector> c_IntVectorMatrix{2, 3, {{1, 4, -2, 3              }, {-2, 2}, {        },
                                                     {2, -4, 2, 5, 10, 14, 3, 2}, {0    }, {4, 2, -1}
                                              }};

    QVERIFY(18 == _getSum(c_IntVectorMatrix));
}

void CPP17ConceptsTests::testConstexprIfIsSame()
{
    QVERIFY(DataTypes::BOOLEAN == _getType(true));
    QVERIFY(DataTypes::BOOLEAN == _getType(false));
    QVERIFY(DataTypes::CHARACTER == _getType('a'));
    QVERIFY(DataTypes::SIGNED_INTEGER == _getType(5));
    QVERIFY(DataTypes::SIGNED_INTEGER == _getType(-7));
    QVERIFY(DataTypes::UNSIGNED_INTEGER == _getType(5u));
    QVERIFY(DataTypes::UNSIGNED_INTEGER == _getType(-1u));
    QVERIFY(DataTypes::DECIMAL == _getType(-1.1));
    QVERIFY(DataTypes::DECIMAL == _getType(1.0000));
    QVERIFY(DataTypes::DECIMAL == _getType(-1.0));
    QVERIFY(DataTypes::DECIMAL == _getType(1.0000001));
    QVERIFY(DataTypes::STRING == _getType(std::string{"abcd"}));
    QVERIFY(DataTypes::STRING_VIEW == _getType("abcd"sv));
    QVERIFY(DataTypes::CSTYLESTRING == _getType("abcd"));
    QVERIFY(DataTypes::STRINGINTPAIR == _getType(StringIntPair{"Robert", 16}));
    QVERIFY(DataTypes::INTMATRIX == _getType(IntMatrix{2, 3, 5}));
    QVERIFY(DataTypes::UNKNOWN == _getType(StringMatrix{2, 3, "abcd"}));
    QVERIFY(DataTypes::UNKNOWN == _getType(Matrix<unsigned int>{2, 3, 5}));

    const unsigned char ch{'a'};
    QVERIFY(DataTypes::UNSIGNED_INTEGER == _getType(ch));

    std::string str{"abcd"};
    char* pCh{&str[1]};
    QVERIFY(DataTypes::UNKNOWN == _getType(pCh));

    QString qstr{"abcd"};
    QVERIFY(DataTypes::UNKNOWN == _getType(qstr));
}

void CPP17ConceptsTests::testStdApply()
{
    auto floorAverage = [](const IntMatrix& matrix, int initValue)
    {
        int average{INT_MIN};

        if (!matrix.isEmpty())
        {
            average = std::accumulate(matrix.constZBegin(), matrix.constZEnd(), initValue) / (matrix.getNrOfRows() * matrix.getNrOfColumns());
        }

        return average;
    };

    IntMatrix matrix{2, 3, {-5,  50, 40,
                            20, -10, 4
                     }};

    const int initValue{15};

    QVERIFY(19 == std::apply(floorAverage, std::make_tuple(matrix, initValue)));

    matrix.resizeWithValue(3, 4, 11);

    QVERIFY(13 == std::apply(floorAverage, std::make_tuple(matrix, 0)));

    matrix.clear();

    QVERIFY(INT_MIN == std::apply(floorAverage, std::make_tuple(matrix, initValue)));
}

void CPP17ConceptsTests::testStdAny()
{
    std::any anyType{std::string{"abcd"}};

    QVERIFY(anyType.has_value() && "abcd" == any_cast<std::string>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<const char*>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<std::string_view>(anyType));

    anyType = "abcd"sv;

    QVERIFY(anyType.has_value() && "abcd"sv == any_cast<std::string_view>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<const char*>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<std::string>(anyType));

    anyType.reset();
    QVERIFY(!anyType.has_value());
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<std::string_view>(anyType));

    anyType = IntMatrix{2, 3, {-1, 4, 5, 2, 3, 6}};

    QVERIFY(anyType.has_value() && IntMatrix(2, 3, {-1, 4, 5, 2, 3, 6}) == any_cast<IntMatrix>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<Matrix<unsigned int>>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<Matrix<short>>(anyType));

    anyType = 6;

    QVERIFY_THROWS_NO_EXCEPTION(any_cast<int>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<short>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<unsigned int>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<long long>(anyType));

    anyType = short{6};

    QVERIFY_THROWS_NO_EXCEPTION(any_cast<short>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<int>(anyType));

    unsigned int value{8};
    anyType = value;

    QVERIFY_THROWS_NO_EXCEPTION(any_cast<unsigned int>(anyType));
    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<int>(anyType));

    std::any anyOtherType{"abcd"};
    anyOtherType.swap(anyType);

    QVERIFY_THROWS_EXCEPTION(std::bad_any_cast, any_cast<unsigned int>(anyType));
    QVERIFY(anyType.has_value() && "abcd" == std::string{any_cast<const char*>(anyType)});
    QVERIFY(anyOtherType.has_value());

    anyType.reset();
    anyType.swap(anyOtherType);

    QVERIFY(anyType.has_value() && 8 == any_cast<unsigned int>(anyType) &&
            !anyOtherType.has_value());

    anyType = IntMatrix{3, 2, {2, 0, -2, 5, 8, 7}};
    anyOtherType = StringMatrix{2, 3, {"ab", "", "baC", "-d", "ef", "g"}};

    std::swap(anyType, anyOtherType);

    QVERIFY(StringMatrix(2, 3, {"ab", "", "baC", "-d", "ef", "g"}) == any_cast<StringMatrix>(anyType) &&
            IntMatrix(3, 2, {2, 0, -2, 5, 8, 7}) == any_cast<IntMatrix>(anyOtherType));
}

void CPP17ConceptsTests::testStdSize()
{
    std::vector<int> vect{1, -2, 3, 4};
    QVERIFY(4 == std::size(vect));

    vect.push_back(2);
    QVERIFY(5 == std::size(vect));

    const RawContainer c_EmptyContainer;
    QVERIFY(0 == std::size(c_EmptyContainer));

    const RawContainer c_Container{10};
    QVERIFY(10 == std::size(c_Container));

    const IntMatrixWrapper c_EmptyIntMatrix;
    QVERIFY(0 == std::size(c_EmptyIntMatrix));

    const IntMatrixWrapper c_IntMatrix{10, 15, -2};
    QVERIFY(150 == std::size(c_IntMatrix));

    const StringMatrixWrapper c_EmptyStringMatrix;
    QVERIFY(SizePair(0, 0) == std::size(c_EmptyStringMatrix));

    const StringMatrixWrapper c_StringMatrix{4, 6, "abc"};
    QVERIFY(SizePair(4, 6) == std::size(c_StringMatrix));
}

void CPP17ConceptsTests::testStdOptional()
{
    IntMatrix intMatrix;
    std::optional<IntMatrix::size_type> intMatrixLowestDiagonalNr{_getLowestDiagonalNr(intMatrix)};

    QVERIFY(!intMatrixLowestDiagonalNr.has_value());

    intMatrix.resizeWithValue(3, 2, -5);
    intMatrixLowestDiagonalNr = _getLowestDiagonalNr(intMatrix);

    QVERIFY(intMatrixLowestDiagonalNr.has_value() && -2 == intMatrixLowestDiagonalNr.value());

    intMatrixLowestDiagonalNr.reset();

    QVERIFY(std::nullopt == intMatrixLowestDiagonalNr);
    QVERIFY(INT_MIN == intMatrixLowestDiagonalNr.value_or(INT_MIN));

    StringMatrix stringMatrix{1, 1, "abcd"};
    std::optional<StringMatrix::size_type> stringMatrixLowestDiagonalNr{_getLowestDiagonalNr(stringMatrix)};
    std::optional<StringMatrix::size_type> stringMatrixHighestDiagonalNr{_getHighestDiagonalNr(stringMatrix)};

    QVERIFY(stringMatrixLowestDiagonalNr.has_value() && stringMatrixLowestDiagonalNr == stringMatrixHighestDiagonalNr && 0 == stringMatrixHighestDiagonalNr.value());

    stringMatrix.resize(3, 2);
    stringMatrixLowestDiagonalNr = _getLowestDiagonalNr(stringMatrix);
    stringMatrixHighestDiagonalNr = _getHighestDiagonalNr(stringMatrix);

    QVERIFY(stringMatrixLowestDiagonalNr.has_value() && stringMatrixHighestDiagonalNr.has_value() && stringMatrixLowestDiagonalNr < stringMatrixHighestDiagonalNr);
    QVERIFY(-2 == stringMatrixLowestDiagonalNr.value_or(INT_MIN) && 1 == stringMatrixHighestDiagonalNr.value_or(INT_MAX));

    stringMatrixLowestDiagonalNr.swap(stringMatrixHighestDiagonalNr);

    QVERIFY(stringMatrixLowestDiagonalNr.has_value() && stringMatrixHighestDiagonalNr.has_value() && 1 == stringMatrixLowestDiagonalNr.value() && -2 == stringMatrixHighestDiagonalNr.value());

    stringMatrixLowestDiagonalNr.reset();
    std::swap(stringMatrixHighestDiagonalNr, stringMatrixLowestDiagonalNr);

    QVERIFY(stringMatrixLowestDiagonalNr.has_value() && !stringMatrixHighestDiagonalNr.has_value() && -2 == stringMatrixLowestDiagonalNr.value());

    stringMatrixLowestDiagonalNr.reset();
    stringMatrixHighestDiagonalNr.reset();

    QVERIFY(!stringMatrixLowestDiagonalNr.has_value() && !stringMatrixHighestDiagonalNr.has_value() && stringMatrixLowestDiagonalNr == stringMatrixHighestDiagonalNr);
    QVERIFY_THROWS_EXCEPTION(std::bad_optional_access, {const StringMatrix::size_type c_DiagNumber{stringMatrixLowestDiagonalNr.value()}; (void)c_DiagNumber;});
}

void CPP17ConceptsTests::testStdStringView()
{
    // scenario 1: various initializations of string_views / string init based on string_view
    {
        constexpr std::string_view c_FirstStringView{"My first string_view"};
        constexpr char c_CharArray[21]{'M', 'y', ' ', 'f', 'i', 'r', 's', 't', ' ', 's', 't', 'r', 'i', 'n', 'g', '_', 'v', 'i', 'e', 'w', '\0'};
        const std::string_view c_SecondStringView{c_CharArray}; // cannot constexpr when being initialized by char array (not a literal)

        QVERIFY(20 == c_SecondStringView.size());

        constexpr std::string_view c_ThirdStringView{"My first string_view"sv};

        const std::string c_FirstString{"My first string_view"};
        const std::string_view c_FourthStringView{c_FirstString}; // cannot constexpr when being initialized by std::string
        const std::string_view c_FifthStringView{c_FourthStringView}; // cannot constexpr (c_FourthStringView was initialized const)
        constexpr std::string_view c_SixthStringView{c_FirstStringView};
        constexpr std::string_view c_SeventhStringView{c_ThirdStringView};

        QVERIFY("My first string_view" == c_FirstStringView &&
                c_FirstStringView == c_SecondStringView && // the char array has terminating character which results in successful match of the views
                c_SecondStringView == c_ThirdStringView &&
                c_ThirdStringView == c_FourthStringView &&
                c_FourthStringView == c_FifthStringView &&
                c_FifthStringView == c_SixthStringView &&
                c_SixthStringView == c_SeventhStringView);

        const std::string c_SecondString{c_FirstStringView};

        QVERIFY(c_FirstString == c_SecondString);
    }

    // scenario 2: initialization starting point is char array, this gets modified in the meantime
    {
        char charArray[20]{'A', 'n', 'o', 't', 'h', 'e', 'r', ' ', 's', 't', 'r', 'i', 'n', 'g', '_', 'v', 'i', 'e', 'w', '\0'};
        std::string_view firstStringView{charArray};
        std::string_view secondStringView{"Another string_view"};

        charArray[8] = 'S';

        QVERIFY(secondStringView != firstStringView); // case sensitive compare

        charArray[7] = '_';
        firstStringView.remove_prefix(7);
        firstStringView.remove_suffix(3);

        QVERIFY("_String_v" == firstStringView);

        firstStringView.remove_prefix(3);
        firstStringView.remove_suffix(2);

        QVERIFY(firstStringView == secondStringView.substr(10, 4));

        secondStringView.remove_prefix(5);
        secondStringView.remove_suffix(4);

        // although a prefix and a suffix have been chopped the suffix part is still visible due to the null terminating character (string_view initialized with a C-style string)
        QVERIFY(14 == std::strlen(secondStringView.data()));

        firstStringView = secondStringView;
        QVERIFY("er string_"sv == firstStringView &&
                14 == std::strlen(firstStringView.data())); // same here, when creating a string_view copy both views are the same (point to same null-terminated string in this case)
    }

    // scenario 3: char array with multiple terminating characters
    {
        char charArray[14]{'A', 'r', 'r', 'a', 'y', 'O', 'f', 'C', 'h', 'a', 'r', 's', '\0', '\0'};
        std::string_view firstStringView{charArray};

        QVERIFY("ArrayOfChars"sv == firstStringView);

        charArray[5] = '\0';
        QVERIFY(12 == firstStringView.size() &&
                "ArrayOfChars"sv != firstStringView &&
                5 == std::strlen(firstStringView.data()));

        firstStringView.remove_suffix(6);
        QVERIFY(6 == firstStringView.size() &&
                "Array" == firstStringView.substr(0, 5) &&
                '\0' == firstStringView[5]);

        const std::string c_FirstString{firstStringView.data()};
        QVERIFY("Array" == c_FirstString);
    }

    // scenario 4: a string that grows its size
    {
        std::string firstString;
        firstString.reserve(30); // this is to ensure string can grow without re-allocating memory (which might have an unwanted effect on string views bound to it)
        firstString = "aString";
        std::string_view firstStringView{firstString};

        QVERIFY("aString" == firstStringView);

        /* Generally speaking care should be taken when modifying strings to which string views are bound (best to avoid it if possible) */
        firstString.append("FromManyOthers");
        QVERIFY(7 == firstStringView.size() &&
                21 == firstString.size()); // the string_view cannot grow while the string is able to do this

        firstString.at(4) = '1';
        QVERIFY("aStr1ng" == firstStringView); // however if one or more chars get replaced same happens within string_view

        firstStringView.remove_prefix(1);
        firstStringView.remove_suffix(2);

        QVERIFY(4 == firstStringView.size() &&
                firstStringView.substr(1, 3) == firstString.substr(2, 3));

        QVERIFY("aStr1ngFromManyOthers" == firstString); // string retains its characters, no prefix/suffix removed
        QVERIFY(static_cast<std::string>(firstStringView) == firstString.substr(1, 4)); // test cast to std::string too
    }

    // scenario 5: two string_views pointing to the same std::string
    {
        std::string firstString{"Abcdefghijkl"};
        std::string_view firstStringView{firstString};
        std::string_view secondStringView{firstStringView};

        firstString.at(8) = '1';

        QVERIFY("Abcdefgh1jkl"sv == firstStringView &&
                firstStringView == secondStringView);
    }
}

void CPP17ConceptsTests::testTemplateTypeDeductionInConstructors()
{
    // std::string
    const Matrix c_FirstStringMatrix{2, 4, std::string{"stringValue"}}; // std::string should be explicitly mentioned otherwise const char* assumed
    const Matrix<std::string> c_SecondStringMatrix{2, 4, "stringValue"};

    QVERIFY(c_FirstStringMatrix == c_SecondStringMatrix);

    // std::pair<const char*, int>
    const Matrix c_FirstConstCharPtrIntPairMatrix{8, 4, std::pair{"constCharValue", 2}}; // template type deduction both for pair and for matrix
    const Matrix<std::pair<const char*, int>> c_SecondConstCharPtrIntPairMatrix{8, 4, std::pair<const char*, int>{"constCharValue", 2}};

    QVERIFY(c_FirstConstCharPtrIntPairMatrix == c_SecondConstCharPtrIntPairMatrix);

    // int
    const Matrix c_FirstIntMatrix{2, 3, 5};
    const Matrix<int> c_SecondIntMatrix{2, 3, 5};

    QVERIFY(c_FirstIntMatrix == c_SecondIntMatrix);

    // unsigned int
    const Matrix c_FirstUnsignedIntMatrix{4, 5, 3u};
    const Matrix<unsigned int> c_SecondUnsignedIntMatrix{4, 5, 3};

    QVERIFY(c_FirstUnsignedIntMatrix == c_SecondUnsignedIntMatrix);

    // unsigned short
    unsigned short c_UnsignedShortVal{3u};
    const Matrix c_FirstUnsignedShortMatrix{9, 8, c_UnsignedShortVal};
    const Matrix<unsigned short> c_SecondUnsignedShortMatrix{9, 8, 3};

    QVERIFY(c_FirstUnsignedShortMatrix == c_SecondUnsignedShortMatrix);
}

void CPP17ConceptsTests::testFoldExpressionsBinaryUnaryLeftRight()
{
    const auto[c_Result1, c_Count1]{vrd::binaryLeftFoldMinus(5, 2, -3, 5, 4, 9)}; // ((((5 - 2) - (-3)) - 5) - 4) - 9; initial value 5 is at the start of the expression
    const auto[c_Result2, c_Count2]{vrd::binaryLeftFoldMinus(-3, -4, 9, 3, -8)};
    const auto[c_Result3, c_Count3]{vrd::binaryLeftFoldMinus(8, 3, -2)};
    const auto[c_Result4, c_Count4]{vrd::binaryLeftFoldMinus(4, -5)};
    const auto[c_Result5, c_Count5]{vrd::binaryLeftFoldMinus(-2)}; // initial value -2

    QVERIFY(-12 == c_Result1 && 5 == c_Count1);
    QVERIFY(-3 == c_Result2 && 4 == c_Count2);
    QVERIFY(7 == c_Result3 && 2 == c_Count3);
    QVERIFY(9 == c_Result4 && 1 == c_Count4);
    QVERIFY(-2 == c_Result5 && 0 == c_Count5);

    const auto[c_Result6, c_Count6]{vrd::unaryLeftFoldMinus<int>(2, -3, 5, 4, 9)}; // (((2 - (-3)) - 5) - 4) - 9; no initial value
    const auto[c_Result7, c_Count7]{vrd::unaryLeftFoldMinus<int>(-4, 9, 3, -8)};
    const auto[c_Result8, c_Count8]{vrd::unaryLeftFoldMinus<int>(3, -2)};
    const auto[c_Result9, c_Count9]{vrd::unaryLeftFoldMinus<int>(-5)};

    // empty arguments pack not allowed
    QVERIFY(-13 == c_Result6 && 5 == c_Count6);
    QVERIFY(-8 == c_Result7 && 4 == c_Count7);
    QVERIFY(5 == c_Result8 && 2 == c_Count8);
    QVERIFY(-5 == c_Result9 && 1 == c_Count9);

    const auto[c_Result10, c_Count10]{vrd::binaryRightFoldMinus(5, 2, -3, 5, 4, 9)}; // 2 - (-3 - (5 - (4 - (9 - 5)))); initial value 5 is at the end of the expression
    const auto[c_Result11, c_Count11]{vrd::binaryRightFoldMinus(-3, -4, 9, 3, -8)};
    const auto[c_Result12, c_Count12]{vrd::binaryRightFoldMinus(8, 3, -2)};
    const auto[c_Result13, c_Count13]{vrd::binaryRightFoldMinus(4, -5)};
    const auto[c_Result14, c_Count14]{vrd::binaryRightFoldMinus(-2)}; // initial value -2

    QVERIFY(10 == c_Result10 && 5 == c_Count10);
    QVERIFY(-5 == c_Result11 && 4 == c_Count11);
    QVERIFY(13 == c_Result12 && 2 == c_Count12);
    QVERIFY(-9 == c_Result13 && 1 == c_Count13);
    QVERIFY(-2 == c_Result14 && 0 == c_Count14);

    const auto[c_Result15, c_Count15]{vrd::unaryRightFoldMinus<int>(2, -3, 5, 4, 9)}; // 2 - (-3 - (5 - (4 - 9))); no initial value
    const auto[c_Result16, c_Count16]{vrd::unaryRightFoldMinus<int>(-4, 9, 3, -8)};
    const auto[c_Result17, c_Count17]{vrd::unaryRightFoldMinus<int>(3, -2)};
    const auto[c_Result18, c_Count18]{vrd::unaryRightFoldMinus<int>(-5)};

    // empty arguments pack not allowed
    QVERIFY(15 == c_Result15 && 5 == c_Count15);
    QVERIFY(-2 == c_Result16 && 4 == c_Count16);
    QVERIFY(5 == c_Result17 && 2 == c_Count17);
    QVERIFY(-5 == c_Result18 && 1 == c_Count18);
}

void CPP17ConceptsTests::testFoldExpressionsAlternativeRecursiveImplementations()
{
    const auto[c_Result1, c_Count1]{vrd::binaryLeftFoldMinusRecursiveImplementation(5, 2, -3, 5, 4, 9)}; // ((((5 - 2) - (-3)) - 5) - 4) - 9; initial value 5 is at the start of the expression
    const auto[c_Result2, c_Count2]{vrd::binaryLeftFoldMinusRecursiveImplementation(-3, -4, 9, 3, -8)};
    const auto[c_Result3, c_Count3]{vrd::binaryLeftFoldMinusRecursiveImplementation(8, 3, -2)};
    const auto[c_Result4, c_Count4]{vrd::binaryLeftFoldMinusRecursiveImplementation(4, -5)};
    const auto[c_Result5, c_Count5]{vrd::binaryLeftFoldMinusRecursiveImplementation(-2)}; // initial value -2

    QVERIFY(-12 == c_Result1 && 5 == c_Count1);
    QVERIFY(-3 == c_Result2 && 4 == c_Count2);
    QVERIFY(7 == c_Result3 && 2 == c_Count3);
    QVERIFY(9 == c_Result4 && 1 == c_Count4);
    QVERIFY(-2 == c_Result5 && 0 == c_Count5);

    const auto[c_Result6, c_Count6]{vrd::unaryLeftFoldMinusRecursiveImplementation<int>(2, -3, 5, 4, 9)}; // (((2 - (-3)) - 5) - 4) - 9; no initial value
    const auto[c_Result7, c_Count7]{vrd::unaryLeftFoldMinusRecursiveImplementation<int>(-4, 9, 3, -8)};
    const auto[c_Result8, c_Count8]{vrd::unaryLeftFoldMinusRecursiveImplementation<int>(3, -2)};
    const auto[c_Result9, c_Count9]{vrd::unaryLeftFoldMinusRecursiveImplementation<int>(-5)};

    // empty arguments pack not allowed
    QVERIFY(-13 == c_Result6 && 5 == c_Count6);
    QVERIFY(-8 == c_Result7 && 4 == c_Count7);
    QVERIFY(5 == c_Result8 && 2 == c_Count8);
    QVERIFY(-5 == c_Result9 && 1 == c_Count9);

    const auto[c_Result10, c_Count10]{vrd::binaryRightFoldMinusRecursiveImplementation(5, 2, -3, 5, 4, 9)}; // 2 - (-3 - (5 - (4 - (9 - 5)))); initial value 5 is at the end of the expression
    const auto[c_Result11, c_Count11]{vrd::binaryRightFoldMinusRecursiveImplementation(-3, -4, 9, 3, -8)};
    const auto[c_Result12, c_Count12]{vrd::binaryRightFoldMinusRecursiveImplementation(8, 3, -2)};
    const auto[c_Result13, c_Count13]{vrd::binaryRightFoldMinusRecursiveImplementation(4, -5)};
    const auto[c_Result14, c_Count14]{vrd::binaryRightFoldMinusRecursiveImplementation(-2)}; // initial value -2

    QVERIFY(10 == c_Result10 && 5 == c_Count10);
    QVERIFY(-5 == c_Result11 && 4 == c_Count11);
    QVERIFY(13 == c_Result12 && 2 == c_Count12);
    QVERIFY(-9 == c_Result13 && 1 == c_Count13);
    QVERIFY(-2 == c_Result14 && 0 == c_Count14);

    const auto[c_Result15, c_Count15]{vrd::unaryRightFoldMinusRecursiveImplementation<int>(2, -3, 5, 4, 9)}; // 2 - (-3 - (5 - (4 - 9))); no initial value
    const auto[c_Result16, c_Count16]{vrd::unaryRightFoldMinusRecursiveImplementation<int>(-4, 9, 3, -8)};
    const auto[c_Result17, c_Count17]{vrd::unaryRightFoldMinusRecursiveImplementation<int>(3, -2)};
    const auto[c_Result18, c_Count18]{vrd::unaryRightFoldMinusRecursiveImplementation<int>(-5)};

    // empty arguments pack not allowed
    QVERIFY(15 == c_Result15 && 5 == c_Count15);
    QVERIFY(-2 == c_Result16 && 4 == c_Count16);
    QVERIFY(5 == c_Result17 && 2 == c_Count17);
    QVERIFY(-5 == c_Result18 && 1 == c_Count18);

    // additional tests for the add...() function

    const auto[c_Result19, c_Count19]{vrd::addArgumentsToInitialValue(5, 2, -3, 5, 4, 9)};
    const auto[c_Result20, c_Count20]{vrd::addArgumentsToInitialValue(-3, -4, 9, 3, -8)};
    const auto[c_Result21, c_Count21]{vrd::addArgumentsToInitialValue(8, 3, -2)};
    const auto[c_Result22, c_Count22]{vrd::addArgumentsToInitialValue(4, -5)};
    const auto[c_Result23, c_Count23]{vrd::addArgumentsToInitialValue(-2)};

    QVERIFY(22 == c_Result19 && 5 == c_Count19);
    QVERIFY(-3 == c_Result20 && 4 == c_Count20);
    QVERIFY(9 == c_Result21 && 2 == c_Count21);
    QVERIFY(-1 == c_Result22 && 1 == c_Count22);
    QVERIFY(-2 == c_Result23 && 0 == c_Count23);
}

void CPP17ConceptsTests::testFoldExpressionsCommaOperator()
{
    auto[c_MinResult1, c_MaxResult1]{vrd::getMinMaxArgument<int>(3, 0, 1, -2, 4, 5, -9, 4, 7, 12, -3, -5)};
    auto[c_MinResult2, c_MaxResult2]{vrd::getMinMaxArgument<int>(4, 4, 4)};
    auto[c_MinResult3, c_MaxResult3]{vrd::getMinMaxArgument<int>(3, -2)};

    QVERIFY(-9 == c_MinResult1 && 12 == c_MaxResult1);
    QVERIFY(4 == c_MinResult2 && 4 == c_MaxResult2);
    QVERIFY(-2 == c_MinResult3 && 3 == c_MaxResult3);

    const double c_Epsilon{1.0e-9};

    auto[c_MinResult4, c_MaxResult4]{vrd::getMinMaxArgument<double>(2.3, -1.2, 3.0, 5.4, -5.6, -5.59, 4.2, 0.0)};
    auto[c_MinResult5, c_MaxResult5]{vrd::getMinMaxArgument<double>(static_cast<double>(5), 2.0000001, 5.00000001, static_cast<double>(2) )};
    auto[c_MinResult6, c_MaxResult6]{vrd::getMinMaxArgument<double>(-4.00000001, -4.0000001)};

    QVERIFY(std::abs(-5.6 - c_MinResult4) < c_Epsilon && std::abs(c_MaxResult4 - 5.4) < c_Epsilon);
    QVERIFY(std::abs(c_MinResult5 - 2.0) < c_Epsilon && std::abs(c_MaxResult5 - 5.00000001) < c_Epsilon);
    QVERIFY(std::abs(-4.0000001 - c_MinResult6) < c_Epsilon && std::abs(-4.00000001 - c_MaxResult6) < c_Epsilon);
}


QTEST_APPLESS_MAIN(CPP17ConceptsTests)

#include "tst_cpp17conceptstests.moc"
