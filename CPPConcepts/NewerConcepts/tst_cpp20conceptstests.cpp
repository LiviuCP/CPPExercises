#include <QtTest>

#include <array>
#include <string_view>
#include <algorithm>
#include <span>

#include "variadictemplates.h"

using namespace std::literals;
namespace vrd = Variadic;

class CPP20ConceptsTests : public QObject
{
    Q_OBJECT

private slots:
    void testAbbreviatedFunctionTemplatesWithAutoParams();
    void testRangeBasedForLoopInit();
    void testMapSetContains();
    void testStringStartsEndsWith();
    void testStdErase();
    void testStdSpan();
    void testConcept(); // test functionality contained in header <concepts>
    void testGenericLambda();
    void testAggregateInitialization(); // includes testing designated initializers (which were introduced in CPP20)
    void testPackExpansionsInLambdaInitCaptures();
    void testPackExpansionsWithAbbreviatedTemplatesAndConcepts();
    void testIndexSequences();

private:
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

    template<typename DataType>
    class MatrixWrapper
    {
    public:
        MatrixWrapper() {}
        MatrixWrapper(int nrOfRows, int nrOfColumns, const DataType& value) : mMatrix{nrOfRows, nrOfColumns, value} {}

        size_t size() const {return mMatrix.getNrOfRows() * mMatrix.getNrOfColumns();}

    private:
        Matrix<DataType> mMatrix;
    };

    class StringWrapper
    {
    public:
        StringWrapper() = delete;
        StringWrapper(const std::string& str) : myStr{str} {}

        ssize_t size() const {return static_cast<ssize_t>(myStr.size());}

    private:
        std::string myStr;
    };

    struct SimpleAggregate
    {
        int firstInteger{-1};
        int secondInteger{2};
        IntPair pairOfIntegers{-4, 5};
        bool boolean{false};
    };

    struct AdvancedAggregate
    {
        IntMatrix matrix{2, 3, {-1, 3, 2, 8, -9, 5}};
        int integer;
        std::string stdString{"MyString"};
        StringVector stringVector{"a2", "b1"};
    };

    TripleSizeTuple _getMinMaxAvgSize(const auto& leftContainer, const auto& middleContainer, const auto& rightContainer) const;

    template<typename DataType>
    requires nonBooleanIntegral<DataType> // "requires" clause
    DataType _getMatrixElementsSum(const Matrix<DataType>& matrix);

    template<typename DataType>
    requires hasSizeMethod<DataType>
    size_t _getTotalElementsSize(const Matrix<DataType>& matrix);

    // shorter way of writing "requires" (std::floating_point) - combined with abbreviated template
    auto _getDecimalValueCopy(const std::floating_point auto& value); // constraint is on argument, not return value; equivalent to template<typename T> requires std::floating_point auto _getDecimalCopy(const T& object)

    template<typename DataType>
    requires std::floating_point<DataType>
    bool _areDecimalValuesEqual(DataType firstValue, DataType secondValue, const DataType epsilon);

    template<numeric DataType> Matrix<DataType> _getCumulativeTotals(const Matrix<DataType> matrix); // another shorter way of writing "requires" (numeric concept defined above)

    template<typename DataType> void _sortMatrixByVectorSize(Matrix<std::vector<DataType>>& matrix);
};

TripleSizeTuple CPP20ConceptsTests::_getMinMaxAvgSize(const auto& leftContainer, const auto& middleContainer, const auto& rightContainer) const
{
    const size_t c_LeftContainerSize{std::size(leftContainer)};
    const size_t c_MiddleContainerSize{std::size(middleContainer)};
    const size_t c_RightContainerSize{std::size(rightContainer)};

    const size_t c_MinSize{std::min(c_LeftContainerSize, std::min(c_MiddleContainerSize, c_RightContainerSize))};
    const size_t c_AvgSize{(c_LeftContainerSize + c_MiddleContainerSize + c_RightContainerSize) / 3};
    const size_t c_MaxSize{std::max(c_LeftContainerSize, std::max(c_MiddleContainerSize, c_RightContainerSize))};

    return std::tuple{c_MinSize, c_AvgSize, c_MaxSize};
}

template<typename DataType>
requires nonBooleanIntegral<DataType>
DataType CPP20ConceptsTests::_getMatrixElementsSum(const Matrix<DataType>& matrix)
{
    return std::accumulate(matrix.constZBegin(), matrix.constZEnd(), 0);
}

template<typename DataType>
requires hasSizeMethod<DataType>
size_t CPP20ConceptsTests::_getTotalElementsSize(const Matrix<DataType>& matrix)
{
    size_t result{0};

    for (const auto& element : matrix)
    {
        result += element.size();
    }

    return result;
}

auto CPP20ConceptsTests::_getDecimalValueCopy(const std::floating_point auto& value)
{
    return value;
}

template<typename DataType>
requires std::floating_point<DataType>
bool CPP20ConceptsTests::_areDecimalValuesEqual(DataType firstValue, DataType secondValue, const DataType epsilon)
{
    return std::abs(firstValue - secondValue) < std::abs(epsilon);
}

template<numeric DataType> Matrix<DataType> CPP20ConceptsTests::_getCumulativeTotals(const Matrix<DataType> matrix)
{
    Matrix result{matrix};

    for (auto it{result.getZIterator(1)}; it != result.zEnd(); ++it)
    {
        *it += it[-1];
    }

    return result;
}

template<typename DataType> void CPP20ConceptsTests::_sortMatrixByVectorSize(Matrix<std::vector<DataType>>& matrix)
{
    std::stable_sort(matrix.zBegin(), matrix.zEnd(), [](const std::vector<DataType>& firstVector, const std::vector<DataType>& secondVector) {return firstVector.size() < secondVector.size();});
}

void CPP20ConceptsTests::testAbbreviatedFunctionTemplatesWithAutoParams()
{
    constexpr std::string_view c_StringView{"This is another string from my code....."sv};
    const StringIntPairVector c_StringIntPairVector{{"ab", 1}, {"cd", 2}, {"ef", 3}, {"gh", 4}, {"ij", 5}, {"kl", 6}};
    const MatrixWrapper c_StringMatrixWrapper{4, 8, std::string{"abcd"}};

    const auto[c_FirstMinSize, c_FirstAvgSize, c_FirstMaxSize]{_getMinMaxAvgSize(c_StringView, c_StringIntPairVector, c_StringMatrixWrapper)};

    QVERIFY(6 == c_FirstMinSize && 26 == c_FirstAvgSize && 40 == c_FirstMaxSize);

    const MatrixWrapper c_IntMatrixWrapper{11, 3, -5};
    const RawContainer c_RawContainer{17};
    constexpr std::array<unsigned short, 10> c_ShortArray{2, 5, 4, 1, 0, 20, 15, 8, 11, 3};

    const auto[c_SecondMinSize, c_SecondAvgSize, c_SecondMaxSize]{_getMinMaxAvgSize(c_IntMatrixWrapper, c_RawContainer, c_ShortArray)};

    QVERIFY(10 == c_SecondMinSize && 20 == c_SecondAvgSize && 33 == c_SecondMaxSize);
}

void CPP20ConceptsTests::testRangeBasedForLoopInit()
{
    int totalAge{0};
    int peopleCount{0};

    for (const StringIntPairMatrix c_Matrix{2, 4, {{"Alexander", 7}, {"Kevin", 14}, {"John", 5}, {"Mary", 10},
                                                 {"Donald", 16}, {"Henrietta", 10}, {"Wilhelm", 8}, {"Ansel", 6}}};
         const auto&[name, age] : c_Matrix)
    {
        if (name.size() > 5)
        {
            totalAge += age;
            ++peopleCount;
        }
    }

    if (peopleCount <= 0)
    {
        QFAIL("Invalid people count");
    }

    const int c_AverageAge{totalAge / peopleCount};

    QVERIFY(10 == c_AverageAge);
}

void CPP20ConceptsTests::testMapSetContains()
{
    const StringIntMap c_PersonsMap
    {
        {"George", 7},
        {"Alex", 6},
        {"Kevin", 10},
        {"Roberta", 8},
        {"Alistair", 15}
    };

    QVERIFY(c_PersonsMap.contains("Roberta"));
    QVERIFY(!c_PersonsMap.contains("Robert"));
    QVERIFY(!c_PersonsMap.contains("roberta"));
    QVERIFY(!c_PersonsMap.contains("Alastair"));

    const StringSet c_PersonsSet
    {
        {"Kyle"},
        {"Albert"},
        {"Anna"},
        {"George"},
        {"Alfred"}
    };

    QVERIFY(c_PersonsSet.contains("Anna"));
    QVERIFY(!c_PersonsSet.contains("Anne"));
    QVERIFY(!c_PersonsSet.contains("Alberto"));
    QVERIFY(!c_PersonsSet.contains("george"));
}

void CPP20ConceptsTests::testStringStartsEndsWith()
{
    const std::string c_aString{"Good that we still code!"};

    // case-sensitive methods
    QVERIFY(c_aString.starts_with("Good"));
    QVERIFY(!c_aString.starts_with("good"));
    QVERIFY(c_aString.ends_with("code!"));
    QVERIFY(!c_aString.ends_with("Code!"));

    std::string_view aStringView{c_aString};
    aStringView.remove_prefix(4);
    aStringView.remove_suffix(5);

    // same methods for string_view
    QVERIFY(aStringView.starts_with(" that") && aStringView.ends_with("still "));
}

void CPP20ConceptsTests::testStdErase()
{
    IntVector intVector{5, -2, 4, 2, -6, 0, 5, 4, 2, -3, 4, 4, -1, 8};
    const IntVector c_IntVectorRef{5, -2, 2, -6, 0, 5, 2, -3, -1, 8};

    std::erase(intVector, 4);

    QVERIFY(c_IntVectorRef == intVector);

    StringIntPairVector stringIntPairVector{{"John", 5}, {"Alistair", 10}, {"Georgia", 8}, {"Kevin", 7}, {"Alexander", 11}, {"Josh", 9}, {"Mary-Anne", 5}, {"Finch", 12}};
    const StringIntPairVector c_StringIntPairVectorRef{{"John", 5}, {"Alistair", 10}, {"Kevin", 7}, {"Alexander", 11}, {"Josh", 9}, {"Finch", 12}};

    std::erase_if(stringIntPairVector, [](const StringIntPair& element) {return element.first.size() > 5 && element.second < 10;});

    QVERIFY(c_StringIntPairVectorRef == stringIntPairVector);
}

void CPP20ConceptsTests::testStdSpan()
{
    /* Scenario 1: std::vector */
    {
        StringIntPairVector stringIntPairVector{{"Jack", 10}, {"Anne", 8}, {"Robert", 5}, {"Annabel", 12}, {"Jim", 9}, {"Alastair", 14}, {"Abigail", 8}, {"Marjorie", 4}};
        const StringIntPairVector c_StringIntPairVectorRef{{"Jack", 10}, {"RESERVED", -1}, {"Robert", 5}, {"Annabelle", 12}, {"Jim", 27}, {"Alastair", 14}, {"UNKNOWN", 0}, {"UNKNOWN", 0}};

        std::span<StringIntPair> stringIntPairSpan{stringIntPairVector};

        QVERIFY(8 == stringIntPairSpan.size() && "Anne" == stringIntPairSpan[1].first && 12 == stringIntPairSpan[3].second);

        std::span<StringIntPair>::iterator firstIt{stringIntPairSpan.begin()};
        std::span<StringIntPair>::iterator secondIt{std::find_if(stringIntPairSpan.begin(), stringIntPairSpan.end(), [](const StringIntPair& element) {return 3 == element.first.size();})};

        QVERIFY(4 == std::distance(firstIt, secondIt));

        std::span<StringIntPair> stringIntPairSubSpan{stringIntPairSpan.subspan(3, 3)};

        QVERIFY(3 == stringIntPairSubSpan.size() && "Jim" == stringIntPairSubSpan[1].first);

        StringIntPair* pStringIntPair{stringIntPairSpan.data() + 5};

        QVERIFY("Alastair" == pStringIntPair->first &&
                14 == pStringIntPair->second &&
                stringIntPairSubSpan.data() + 2 == pStringIntPair);

        std::fill(pStringIntPair + 1, pStringIntPair + 3, StringIntPair{"UNKNOWN", 0});

        pStringIntPair = stringIntPairSubSpan.data();
        pStringIntPair->first.append("le");
        ++pStringIntPair;
        pStringIntPair->second *= 3;

        QVERIFY("Annabelle" == stringIntPairSpan[3].first && 27 == stringIntPairSpan[4].second); // when subspan changes so does the surrounding span

        pStringIntPair = stringIntPairSpan.data() + 1;
        *pStringIntPair = StringIntPair{"RESERVED", -1};

        // finally main span gets shrunk
        stringIntPairSpan = stringIntPairSpan.subspan(2, 2);

        QVERIFY(StringIntPair("Robert", 5) == stringIntPairSpan.front() && 2 == stringIntPairSpan.size());
        QVERIFY(3 == stringIntPairSubSpan.size() && StringIntPair("Alastair", 14) == stringIntPairSubSpan.back());

        QVERIFY(c_StringIntPairVectorRef == stringIntPairVector);
    }

    /* Scenario 2: plain array decayed to pointer (actually a Matrix converted to plain array) */
    {
        IntMatrix matrix{3, 4, {-1, 2, 3, -4, -5, 6, 7, -8, 9, -10, 11, 12}};

        IntMatrix::size_type elementsCount{0};

        // to note: the matrix should NOT be passed to std::span because due to its internal structure its elements are usually not contiguous (std::span supports contiguous containers only)
        int* pArray = (int*)matrix.getBaseArray(elementsCount); // however by converting it to a plain unidimensional array it can be "spanned"
        const int c_ArrayRef[12] {-1, 2, 9, 16, -10, 6, -8, 7, 9, -10, 11, 12};

        std::span<int> intSpan{pArray + 2, static_cast<size_t>(elementsCount / 2)};

        QVERIFY(6 == intSpan.size() && 3 == intSpan[0] && -1 == std::accumulate(intSpan.begin(), intSpan.end(), 0));

        intSpan[2] *= 2; // unlike std::string_view std::span can be used to modify the container elements

        std::span<int> firstIntSubSpan{intSpan.first(2)};
        std::span<int> secondIntSubSpan{intSpan.last(2)};

        // further array modifications done by using the sub-spans
        std::transform(firstIntSubSpan.begin(), firstIntSubSpan.end(), firstIntSubSpan.begin(), [](const int& element) {return element * element;});
        std::swap(secondIntSubSpan[0], secondIntSubSpan[1]);

        QVERIFY(std::equal(pArray, pArray + elementsCount, c_ArrayRef));
    }

    /* Scenario 3: std::array */
    {
        std::array<std::string, 8> namesArray{"Robert", "Christine", "Josh", "Jimmy", "Marie Anne", "Abigail", "Corinne", "Arthur"};
        const std::array<std::string, 8> c_NamesArrayRef{"Christine", "Jimmy", "Josh", "Marie Anne", "Robert", "Corinne", "Arthur", "Abigail"};

        std::span<std::string> stringSpan{namesArray};

        std::span<std::string> leftStringSubSpan{stringSpan.first(4)};
        std::span<std::string> middleStringSubSpan{std::span{namesArray}.subspan(2, 4)}; // just for exercise's sake: it is possible to create sub-spans out of unnamed spans too!
        std::span<std::string> rightStringSubSpan{stringSpan.last(4)};

        QVERIFY("Robert" == leftStringSubSpan.front() && 4 == leftStringSubSpan.size());
        QVERIFY("Josh" == middleStringSubSpan.front() && "Abigail" == middleStringSubSpan.back());
        QVERIFY(4 == rightStringSubSpan.size() && "Arthur" == rightStringSubSpan.back());

        QVERIFY(leftStringSubSpan.data() + 2 == middleStringSubSpan.data() &&
                "Jimmy" == *(middleStringSubSpan.data() + 1) &&
                middleStringSubSpan.data() + 3 == rightStringSubSpan.data() + 1);

        std::sort(leftStringSubSpan.begin(), leftStringSubSpan.end(), [](const std::string& firstElement, const std::string& secondElement) {return std::lexicographical_compare(firstElement.begin(), firstElement.end(), secondElement.begin(), secondElement.end());});
        std::sort(rightStringSubSpan.rbegin(), rightStringSubSpan.rend(), [](const std::string& firstElement, const std::string& secondElement) {return std::lexicographical_compare(firstElement.begin(), firstElement.end(), secondElement.begin(), secondElement.end());});

        QVERIFY("Josh" == middleStringSubSpan.front() && "Corinne" == middleStringSubSpan.back());

        std::iter_swap(middleStringSubSpan.begin() + 1, middleStringSubSpan.end() - 2);

        QVERIFY(std::is_sorted(leftStringSubSpan.begin(), leftStringSubSpan.end()) &&
                middleStringSubSpan.end() - 1 == std::is_sorted_until(middleStringSubSpan.begin(), middleStringSubSpan.end()) &&
                std::is_sorted(rightStringSubSpan.rbegin(), rightStringSubSpan.rend()));

        QVERIFY(c_NamesArrayRef == namesArray);
        QVERIFY(8 == stringSpan.size() && "Christine" == stringSpan.front() && "Abigail" == stringSpan.back());

        // test assignment operator too
        stringSpan = middleStringSubSpan;
        QVERIFY(4 == stringSpan.size() && "Josh" == stringSpan.front() && "Corinne" == stringSpan.back());
    }
}

void CPP20ConceptsTests::testConcept()
{

    const IntMatrix c_IntMatrix{2, 3, {4, 5, -2, 9, 8, -3}};
    const Matrix<size_t> c_SizeMatrix{3, 2, {4, 8, 9, 12, 14, 5}};
    const Matrix c_DecimalMatrix{2, 2, {1.2, 2.5, 5.003, -3.0}};
    const Matrix c_BooleanMatrix{3, 3, {false, true, false, false, true, true, false, true, false}};
    const StringMatrix c_StringMatrix{4, 2, {"abc", "d", "efgh", "", "i", "jkl", "mno", "p"}};
    const Matrix<IntVector> c_IntVectorMatrix{2, 2, {{2, 3, 5}, {1, -2, 4, 10, 2}, {}, {0}}};
    const Matrix<StringWrapper> c_StringWrapperMatrix{2, 4, {{"p"}, {"mno"}, {"jkl"}, {"i"}, {""}, {"efgh"}, {"d"}, {"abc"}}};

    const double c_DoubleEpsilon{1.0e-9};
    const float c_FloatEpsilon{1.0e-9f};

    // scenario 1: template type constrained to integral (but NOT boolean)
    {
        QVERIFY(21 == _getMatrixElementsSum(c_IntMatrix));
        QVERIFY(52 == _getMatrixElementsSum(c_SizeMatrix));
//        QVERIFY(2.703 == _getMatrixElementsSum(c_DecimalMatrix)); // uncomment to reveal compiling error (constraints not satisfied: floating point is not integral)
//        QVERIFY(_getMatrixElementsSum(c_BooleanMatrix)); // uncomment to reveal compiling error (constraints not satisfied: is boolean)
//        QVERIFY(_getMatrixElementsSum(c_StringMatrix)); // uncomment to reveal compiling error (constraints not satisfied: std::string is not integral)
    }

    // scenario 2: template type should have a size() method returning a size_t (unsigned integer) value
    {
        QVERIFY(16 == _getTotalElementsSize(c_StringMatrix));
        QVERIFY(9 == _getTotalElementsSize(c_IntVectorMatrix));
//        QVERIFY(6 == _getTotalElementsSize(c_IntMatrix)); // uncomment to reveal compiling error (constraints not satisfied: the int type does not have size() method)
//        QVERIFY(16 == _getTotalElementsSize(c_StringWrapperMatrix)); // uncomment to reveal compiling error (constraints not satisfied: the template type has a size() method but it returns a signed value)
    }

    // scenario 3: template type should be a floating point, a single argument of the template type is used
    {
        QVERIFY(2.5034 == _getDecimalValueCopy(2.5034));
        QVERIFY(-2.5034f == _getDecimalValueCopy(-2.5034f));
        QVERIFY(-4.0 == _getDecimalValueCopy(-4.0));
        QVERIFY(2.0f == _getDecimalValueCopy(2.0f));
//        QVERIFY(2 == _getDecimalValueCopy(2)); // uncomment to reveal compiling error (constraints not satisfied: auto is deduced to int which in turn is not floating point)
    }

    // scenario 4: template type should be a floating point, multiple arguments of the SAME template type are required
    {
        QVERIFY(_areDecimalValuesEqual(2.5, 2.5000000, c_DoubleEpsilon));
        QVERIFY(_areDecimalValuesEqual(-1.22f, -1.2200f, c_FloatEpsilon)); // all members should be float (not double) - first member determines the required type
        QVERIFY(!_areDecimalValuesEqual(-4.5, -4.500000001, c_DoubleEpsilon));
        QVERIFY(_areDecimalValuesEqual(-4.5, -4.5000000001, c_DoubleEpsilon));
//        QVERIFY(_areDecimalValuesEqual(-1.22f, -1.2200f, c_DoubleEpsilon)); // uncomment to reveal compiling error (conflicting types: float vs. double)
//        QVERIFY(_areDecimalValuesEqual(2.00, 2, c_DoubleEpsilon)); // uncomment to reveal compiling error (conflicting types: double vs. int)
        QVERIFY(_areDecimalValuesEqual(2.00, static_cast<double>(2), c_DoubleEpsilon)); // a static_cast solves the matter
//        QVERIFY(_areDecimalValuesEqual(2.00, 2.000f, c_FloatEpsilon)); // uncomment to reveal compiling error (conflicting types: double vs. float)
//        QVERIFY(_areDecimalValuesEqual(12, 12, 0)); // uncomment to reveal compiling error (type does not satisfy constraints: int is not floating point)
    }

    // scenario 5: template type constrained to either non-boolean integral (like in scenario 1) OR floating point (like in scenarios 3 and 4)
    {
        IntMatrix firstNumericMatrix{2, 4, {3, -2, 5, 7,
                                            0, -1, 14, 10
                                     }};

        const IntMatrix c_FirstNumericMatrixRef{2, 4, {3, 1, 6, 13,
                                                       13, 12, 26, 36
                                                }};

        firstNumericMatrix = _getCumulativeTotals(firstNumericMatrix);

        QVERIFY(c_FirstNumericMatrixRef == firstNumericMatrix);

        Matrix<double> secondNumericMatrix{3, 2, {-2.5, 1.67,
                                                   3.9, -0.1,
                                                   2.4, 0.0
                                           }};

        const Matrix<double> c_SecondNumericMatrixRef{3, 2, {-2.5, -0.83,
                                                              3.07, 2.97,
                                                              5.37, 5.37
                                                      }};

        secondNumericMatrix = _getCumulativeTotals(secondNumericMatrix);

        // this workaround for checking the two Matrix<double> objects for equality is required as the == operator of the Matrix class cannot be used reliably when the Matrix template type is floating point
        QVERIFY(std::equal(c_SecondNumericMatrixRef.constZBegin(),
                           c_SecondNumericMatrixRef.constZEnd(),
                           secondNumericMatrix.constZBegin(),
                           secondNumericMatrix.constZEnd(),
                           [this, c_DoubleEpsilon](const double& firstElement, const double& secondElement) {return _areDecimalValuesEqual(firstElement, secondElement, c_DoubleEpsilon);}));

//        (void)_getCumulativeTotals(c_BooleanMatrix); // uncomment to reveal compiling error (constraints not satisfied, argument is not allowed to be bool)
//        (void)_getCumulativeTotals(c_StringMatrix); // uncomment to reveal compiling error (constraints not satisfied, argument is not allowed to be std::string - can either be non-boolean integer or decimal)

        const auto c_ThirdNumericMatrix{_getCumulativeTotals(c_SizeMatrix)};
        QVERIFY(169 == std::accumulate(c_ThirdNumericMatrix.constZBegin(), c_ThirdNumericMatrix.constZEnd(), 0));
    }
}

void CPP20ConceptsTests::testGenericLambda()
{
    // Scenario 1: compare sizes of two std::vector objects
    {
        auto compareVectorSizes = []<typename DataType>(const std::vector<DataType>& firstVector, const std::vector<DataType>& secondVector){return firstVector.size() < secondVector.size();};

        IntVectorMatrix intVectorMatrix{2, 3, {{-1, 2, 4, 5}, {2, 3}, {},
                                               {2, 3, -2, 1, 8}, {4, 3, 4, 4}, {1, -2, 3}
                                        }};

        const IntVectorMatrix c_IntVectorMatrixRef{2, 3, {{2, 3, -2, 1, 8}, {-1, 2, 4, 5}, {4, 3, 4, 4},
                                                          {1, -2, 3}, {2, 3}, {}
                                                   }};

        std::stable_sort(intVectorMatrix.reverseZBegin(), intVectorMatrix.reverseZEnd(), compareVectorSizes);

        QVERIFY(c_IntVectorMatrixRef == intVectorMatrix);

        StringVectorMatrix stringVectorMatrix{2, 2, {{"ab", "d", "-cd"}, {"acd", "dcb", "abo", "obd"},
                                                     {"abcd", "efgh"}, {"a", "", "bc", "d", "f"}
                                              }};

        const StringVectorMatrix c_StringVectorMatrixRef{2, 2, {{"a", "", "bc", "d", "f"}, {"ab", "d", "-cd"},
                                                                {"acd", "dcb", "abo", "obd"}, {"abcd", "efgh"}
                                                         }};

        std::sort(stringVectorMatrix.reverseNBegin(), stringVectorMatrix.reverseNEnd(), compareVectorSizes);

        QVERIFY(c_StringVectorMatrixRef == stringVectorMatrix);
    }

    // Scenario 2: let's make it more generic now! (with restrictions)
    {
        auto compareContainerSizes = []<hasSizeMethod DataType>(const DataType& firstContainer, const DataType& secondContainer) {return firstContainer.size() < secondContainer.size();};

        StringMatrix stringMatrix{3, 2, {"ab", "dbba", "abcd",
                                         "", "acd", "afghi"
                                  }};

        const StringMatrix c_StringMatrixRef{3, 2, {"afghi", "dbba", "abcd",
                                                    "acd", "ab", ""
                                             }};

        std::stable_sort(stringMatrix.reverseZBegin(), stringMatrix.reverseZEnd(), compareContainerSizes);

        QVERIFY(c_StringMatrixRef == stringMatrix);

        IntVectorMatrix intVectorMatrix{2, 2, {{2, 3, 5}, {-1, 2},
                                               {4, 4, 4, 4}, {-3}
                                        }};

        const IntVectorMatrix c_IntVectorMatrixRef{2, 2, {{-3}, {2, 3, 5},
                                                          {-1, 2}, {4, 4, 4, 4}
                                                   }};

        std::sort(intVectorMatrix.nBegin(), intVectorMatrix.nEnd(), compareContainerSizes);

        QVERIFY(c_IntVectorMatrixRef == intVectorMatrix);

        IntMatrix intMatrix{2, 4, {-2, 5, 3, 4, 5, 6, -1, 0}};

//        std::sort(intMatrix.zBegin(), intMatrix.zEnd(), compareContainerSizes); // uncomment to reveal the compiling error (int does not have size())

        Matrix<StringWrapper> stringWrapperMatrix{2, 2, {{"ab"}, {"dcb"}, {"fxcd"}, {""}}};

//        std::sort(stringWrapperMatrix.reverseNBegin(), stringWrapperMatrix.reverseNEnd(), compareContainerSizes); // uncomment to reveal the compiling error (size() does not return size_t but signed integer)
    }

    // Scenario 3: lambda with abbreviated template type (with restrictions)
    {
        const size_t c_SizeRefValue{5};
        auto isSizeGreaterThanRefValue = [c_SizeRefValue](const hasSizeMethod auto& container) {return container.size() > c_SizeRefValue;};

        const StringMatrix c_StringMatrix{2, 3, {"ab", "hcbadcba", "cdef", "ijklmn", "abab", ""}};

        QVERIFY(2 == std::count_if(c_StringMatrix.constZBegin(), c_StringMatrix.constZEnd(), isSizeGreaterThanRefValue));

        const IntVectorMatrix c_IntVectorMatrix{2, 3, {{2, 3, -1}, {4, 4, 0, 2, 5, 6, 7}, {},
                                                       {2, 8}, {5, 4, 3, 2, -1}, {2, 3, -1}
                                                }};

        QVERIFY(1 == std::count_if(c_IntVectorMatrix.constReverseNBegin(), c_IntVectorMatrix.constReverseNColumnEnd(1), isSizeGreaterThanRefValue));

        const IntMatrix c_IntMatrix{2, 2, {-1, 3, 2, 4}};
//        (void)std::count_if(c_IntMatrix.constReverseZBegin(), c_IntMatrix.constReverseZEnd(), isSizeGreaterThanRefValue); // uncomment to reveal the compiling error (int does not have size())

        const Matrix<StringWrapper> c_StringWrapperMatrix{2, 2, {{"abdfecge"}, {"dcbe"}, {"excd"}, {""}}};
//        (void)std::count_if(c_StringWrapperMatrix.constDBegin(0), c_StringWrapperMatrix.constDEnd(0), isSizeGreaterThanRefValue); // uncomment to reveal the compiling error (size() does not return size_t but signed integer)
    }

    // Scenario 4: lambda included in template method
    {
        IntVectorMatrix intVectorMatrix{2, 3, {{-1, 2, 4, 5}, {2, 3}, {},
                                               {2, 3, -2, 1, 8}, {4, 3, 4, 4}, {1, -2, 3}
                                        }};

        const IntVectorMatrix c_IntVectorMatrixRef{2, 3, {{}, {2, 3}, {1, -2, 3},
                                                          {-1, 2, 4, 5}, {4, 3, 4, 4}, {2, 3, -2, 1, 8}
                                                   }};

        _sortMatrixByVectorSize(intVectorMatrix);

        QVERIFY(c_IntVectorMatrixRef == intVectorMatrix);

        StringVectorMatrix stringVectorMatrix{2, 2, {{"ab", "d", "-cd"}, {"acd", "dcb", "abo", "obd"},
                                                     {"abcd", "efgh"}, {"a", "", "bc", "d", "f"}
                                              }};

        const StringVectorMatrix c_StringVectorMatrixRef{2, 2, {{"abcd", "efgh"}, {"ab", "d", "-cd"},
                                                                {"acd", "dcb", "abo", "obd"}, {"a", "", "bc", "d", "f"}
                                                         }};

        _sortMatrixByVectorSize(stringVectorMatrix);

        QVERIFY(c_StringVectorMatrixRef == stringVectorMatrix);
    }
}

void CPP20ConceptsTests::testAggregateInitialization()
{
    SimpleAggregate firstSimpleAggregate{.firstInteger = 0, .pairOfIntegers = {3, -8}};
    SimpleAggregate secondSimpleAggregate{4, 12, {-3, 5}, true};

    QVERIFY(0 == firstSimpleAggregate.firstInteger && 2 == firstSimpleAggregate.secondInteger && 3 == firstSimpleAggregate.pairOfIntegers.first && -8 == firstSimpleAggregate.pairOfIntegers.second && !firstSimpleAggregate.boolean);
    QVERIFY(4 == secondSimpleAggregate.firstInteger && 12 == secondSimpleAggregate.secondInteger && -3 == secondSimpleAggregate.pairOfIntegers.first && 5 == secondSimpleAggregate.pairOfIntegers.second && secondSimpleAggregate.boolean);

    firstSimpleAggregate = {.secondInteger = 8, .pairOfIntegers = {2, -9}, .boolean = true};
    secondSimpleAggregate = {2, 6, {5, -3}, false};

    QVERIFY(-1 == firstSimpleAggregate.firstInteger && 8 == firstSimpleAggregate.secondInteger && 2 == firstSimpleAggregate.pairOfIntegers.first && -9 == firstSimpleAggregate.pairOfIntegers.second && firstSimpleAggregate.boolean);
    QVERIFY(2 == secondSimpleAggregate.firstInteger && 6 == secondSimpleAggregate.secondInteger && 5 == secondSimpleAggregate.pairOfIntegers.first && -3 == secondSimpleAggregate.pairOfIntegers.second && !secondSimpleAggregate.boolean);

    SimpleAggregate thirdSimpleAggregate{3, 12};

    QVERIFY(3 == thirdSimpleAggregate.firstInteger && 12 == thirdSimpleAggregate.secondInteger && -4 == thirdSimpleAggregate.pairOfIntegers.first && 5 == thirdSimpleAggregate.pairOfIntegers.second && !thirdSimpleAggregate.boolean);

    thirdSimpleAggregate.boolean = true;
    thirdSimpleAggregate = {-3, -15, {-2, 10}};

    QVERIFY(-3 == thirdSimpleAggregate.firstInteger && -15 == thirdSimpleAggregate.secondInteger && -2 == thirdSimpleAggregate.pairOfIntegers.first && 10 == thirdSimpleAggregate.pairOfIntegers.second && !thirdSimpleAggregate.boolean);

    thirdSimpleAggregate = {.boolean = true};

    QVERIFY(-1 == thirdSimpleAggregate.firstInteger && 2 == thirdSimpleAggregate.secondInteger && -4 == thirdSimpleAggregate.pairOfIntegers.first && 5 == thirdSimpleAggregate.pairOfIntegers.second && thirdSimpleAggregate.boolean);

    AdvancedAggregate firstAdvancedAggregate{.stringVector = {"zy", "xw"}};
    IntMatrix intMatrixRef{2, 3, {-1, 3, 2, 8, -9, 5}};
    StringVector stringVectorRef{"zy", "xw"};

    QVERIFY(intMatrixRef == firstAdvancedAggregate.matrix && 0 == firstAdvancedAggregate.integer && "MyString" == firstAdvancedAggregate.stdString && std::equal(stringVectorRef.cbegin(), stringVectorRef.cend(), firstAdvancedAggregate.stringVector.cbegin()));

    AdvancedAggregate secondAdvancedAggregate{.integer = -2, .stringVector = {}};

    QVERIFY(intMatrixRef == secondAdvancedAggregate.matrix && -2 == secondAdvancedAggregate.integer && "MyString" == secondAdvancedAggregate.stdString && secondAdvancedAggregate.stringVector.empty());

    AdvancedAggregate thirdAdvancedAggregate{{3, 2, {-1, 2, -9, 3, 8, 5}}, 4};
    intMatrixRef = {3, 2, {-1, 2, -9, 3, 8, 5}};
    stringVectorRef = {"a2", "b1"};

    QVERIFY(intMatrixRef == thirdAdvancedAggregate.matrix && 4 == thirdAdvancedAggregate.integer && "MyString" == thirdAdvancedAggregate.stdString && std::equal(stringVectorRef.cbegin(), stringVectorRef.cend(), thirdAdvancedAggregate.stringVector.cbegin()));

    firstAdvancedAggregate = {.stdString = "AnotherString", .stringVector = {"Another", "String"}};
    intMatrixRef = {2, 3, {-1, 3, 2, 8, -9, 5}};
    stringVectorRef = {"Another", "String"};

    QVERIFY(intMatrixRef == firstAdvancedAggregate.matrix && 0 == firstAdvancedAggregate.integer && "AnotherString" == firstAdvancedAggregate.stdString && std::equal(stringVectorRef.cbegin(), stringVectorRef.cend(), firstAdvancedAggregate.stringVector.cbegin()));

    secondAdvancedAggregate.stringVector = {"ab", "cd"};
    secondAdvancedAggregate = {.matrix = {}, .stdString = "", .stringVector = {}};

    QVERIFY(secondAdvancedAggregate.matrix.isEmpty() && 0 == secondAdvancedAggregate.integer && secondAdvancedAggregate.stdString.empty() && secondAdvancedAggregate.stringVector.empty());

    thirdAdvancedAggregate = {{3, 2, {-1, 0, 1, 0, 0, -1}}, -8, "aNewString", {"z1, yx, w1, vu"}};
    intMatrixRef = {3, 2, {-1, 0, 1, 0, 0, -1}};
    stringVectorRef = {"z1, yx, w1, vu"};

    QVERIFY(intMatrixRef == thirdAdvancedAggregate.matrix && -8 == thirdAdvancedAggregate.integer && "aNewString" == thirdAdvancedAggregate.stdString && std::equal(stringVectorRef.cbegin(), stringVectorRef.cend(), thirdAdvancedAggregate.stringVector.cbegin()));
}

void CPP20ConceptsTests::testPackExpansionsInLambdaInitCaptures()
{
    QVERIFY(2 == vrd::computeAverageWithVariadicTemplateAndLambda<int>(5, -2, 4, 10, -5));
    QVERIFY(-1 == vrd::computeAverageWithVariadicTemplateAndLambda<int>(-15, 3, 8, -2));
    QVERIFY(15 == vrd::computeAverageWithVariadicTemplateAndLambda<int>(16, 14));
    QVERIFY(-4 == vrd::computeAverageWithVariadicTemplateAndLambda<int>(-4));

    const double c_Epsilon{1.0e-9};

    QVERIFY(std::abs(vrd::computeAverageWithVariadicTemplateAndLambda<double>(5, -2, 4, 10, -5) - 2.4) < c_Epsilon);
    QVERIFY(std::abs(-1.5 - vrd::computeAverageWithVariadicTemplateAndLambda<double>(-15, 3, 8, -2)) < c_Epsilon);
    QVERIFY(std::abs(vrd::computeAverageWithVariadicTemplateAndLambda<double>(16, 14) - 15.0) < c_Epsilon);
    QVERIFY(std::abs(-4.0 - vrd::computeAverageWithVariadicTemplateAndLambda<double>(-4)) < c_Epsilon);

    QVERIFY(std::abs(vrd::computeAverageWithVariadicTemplateAndLambda<double>(2.3, -4.5, 10, 9.1, -3.2, -2, 18.002) - 4.2431428571) < c_Epsilon);

    int intArg1{10}, intArg2{5}, intArg3{-3}, intArg4{2}, intArg5{22};

    vrd::addBeforeAndAfterWithVariadicTemplateAndLambda<int>(2, -5, intArg1, intArg2, intArg3, intArg4, intArg5);
    QVERIFY(7 == intArg1 && 2 == intArg2 && -6 == intArg3 && -1 == intArg4 && 19 == intArg5);

    vrd::addBeforeAndAfterWithVariadicTemplateAndLambda<int>(-5, 9, intArg2, intArg4);
    QVERIFY(7 == intArg1 && 6 == intArg2 && -6 == intArg3 && 3 == intArg4 && 19 == intArg5);

    std::string strArg1{"Andrew"}, strArg2{"George"}, strArg3{"Joanna"};

    vrd::addBeforeAndAfterWithVariadicTemplateAndLambda<std::string>("Hello ", "!", strArg1, strArg2, strArg3);
    QVERIFY("Hello Andrew!" == strArg1 && "Hello George!" == strArg2 &&  "Hello Joanna!" == strArg3);

    vrd::addBeforeAndAfterWithVariadicTemplateAndLambda<std::string>("What a surprise! ", " Cheerio!", strArg2);
    QVERIFY("Hello Andrew!" == strArg1 && "What a surprise! Hello George! Cheerio!" == strArg2 &&  "Hello Joanna!" == strArg3);
}

void CPP20ConceptsTests::testPackExpansionsWithAbbreviatedTemplatesAndConcepts()
{
    const double c_Epsilon{1.0e-9};

    const int c_IntResult1{vrd::getSumOfArgumentSquares(-2, 5, 3, -1, 4)};
    const int c_IntResult2{vrd::getSumOfArgumentSquares(-2, 5.0, 3, -1, 4)};
    const int c_IntResult3{vrd::getSumOfArgumentSquares(-2, 5.1, 3, -1, 4)};
    const int c_IntResult4{vrd::getSumOfArgumentSquares(-2)};

    QVERIFY(55 == c_IntResult1);
    QVERIFY(55 == c_IntResult2);
    QVERIFY(56 == c_IntResult3);
    QVERIFY(4 == c_IntResult4);

    // swapping the first two arguments changes result type and value: (5.1 * 5.1) + (double)(-2 * -2) + (double)(3 * 3) + (double)(-1 * -1) + (double)(4 * 4)
    const double c_IntResult3ChangedToDouble{vrd::getSumOfArgumentSquares(5.1, -2, 3, -1, 4)};
    QVERIFY(std::abs(c_IntResult3ChangedToDouble - 56.01) < c_Epsilon);

    const double c_DoubleResult1{vrd::getSumOfArgumentSquares(2.1, 5.0, -4.3, 8.2)};
    const double c_DoubleResult2{vrd::getSumOfArgumentSquares(2.1, 5, -4.3, 8.2)};
    const double c_DoubleResult3{vrd::getSumOfArgumentSquares(2.1, 5, -4, 8)};
    const double c_DoubleResult4{vrd::getSumOfArgumentSquares(2.1)};

    QVERIFY(std::abs(c_DoubleResult1 - 115.14) < c_Epsilon);
    QVERIFY(std::abs(c_DoubleResult2 - 115.14) < c_Epsilon);
    QVERIFY(std::abs(c_DoubleResult3 - 109.41) < c_Epsilon);
    QVERIFY(std::abs(c_DoubleResult4 - 4.41) < c_Epsilon);

    // swapping the first two arguments changes result type and value: (5 * 5) + (int)(2.1 * 2.1) + (int)(-4.3 * -4.3) + (int)(8.2 * 8.2)
    const int c_DoubleResult2ChangedToInt{vrd::getSumOfArgumentSquares(5, 2.1, -4.3, 8.2)};
    QVERIFY(114 == c_DoubleResult2ChangedToInt);
}

void CPP20ConceptsTests::testIndexSequences()
{
    // first validate the (heterogenous) tuples equality check method as it will be used as helper for testing other methods (see below)
    QVERIFY(vrd::areTuplesEqual(std::tuple{2, 4, 5}, std::tuple{2, 4, 5}, 0));
    QVERIFY(!vrd::areTuplesEqual(std::tuple{2, 4, 5u}, std::tuple{2, 4, 5}, 0));
    QVERIFY(vrd::areTuplesEqual(std::tuple{2, 4, 5u}, std::tuple{2, 4, static_cast<unsigned int>(5)}, 0));
    QVERIFY(!vrd::areTuplesEqual(std::tuple{2, 4, 5}, std::tuple{2, 4, 5, 7}, 0));
    QVERIFY(!vrd::areTuplesEqual(std::tuple{2, 4, 5}, std::tuple{2, 4.0, 5}, 0));
    QVERIFY(vrd::areTuplesEqual(std::tuple{2.5, -2.01, 4.001, -1.552}, std::tuple{2.50, -2.0100, 4.001000, -1.5520e0}, 1.0e-9));
    QVERIFY(!vrd::areTuplesEqual(std::tuple{2.5, -2.01f, 4.001, -1.552}, std::tuple{2.50, -2.0100, 4.001000, -1.5520e0}, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(std::tuple{1.2, -2.0, 4.5f}, std::tuple{static_cast<double>(1.2f), static_cast<double>(-2), static_cast<float>(4.5)}, 1.0e-7));
    QVERIFY(vrd::areTuplesEqual(std::tuple{2, -5.2, 3, -2, 4.52}, std::tuple{2, -5.2000, 3, -2, 4.52000000}, 1.0e-9));
    QVERIFY(!vrd::areTuplesEqual(std::tuple{2, -5.2, 3, -2, 4.52}, std::tuple{2, -5.2000, 3, -2, 4.520000001}, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(std::tuple{2, -5.2, 3, -2, 4.52}, std::tuple{2, -5.2000, 3, -2, 4.520000001}, 1.0e-6));
    QVERIFY(vrd::areTuplesEqual(std::tuple{2, -5.2, 3, -2, 4.52}, std::tuple{static_cast<int>(2.001), -5.2000, static_cast<int>(3.8f), -static_cast<int>(2u), 4.520}, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(std::tuple{}, std::tuple{}, 0));
    QVERIFY(!vrd::areTuplesEqual(std::tuple{}, std::tuple{2.55}, 0));

    const std::tuple c_Tuple1{vrd::createTupleWithAlternatingSigns(2, 3, 5, 4, 9)};
    const std::tuple c_Tuple1Ref{2, -3, 5, -4, 9};

    const std::tuple c_Tuple2{vrd::createTupleWithAlternatingSigns(2.5, -1.2, -2.3, 4.01, -5.12, -3.05, 2.44, 0.0)};
    const std::tuple c_Tuple2Ref{2.5, -1.2, 2.3, -4.01, 5.12, -3.05, 2.44, 0.0};

    const std::tuple c_Tuple3{vrd::createTupleWithAlternatingSigns(2.8, -2, -3, 4.5, 5)};
    const std::tuple c_Tuple3Ref{2.8, -2, 3, -4.5, 5};

    const std::tuple c_Tuple4{vrd::createTupleWithAlternatingSigns(-5.2, 2.1)};
    const std::tuple c_Tuple4Ref{5.2, -2.1};

    const std::tuple c_Tuple5{vrd::createTupleWithAlternatingSigns(4)};
    const std::tuple c_Tuple5Ref{4};

    const std::tuple c_Tuple6{vrd::createTupleWithAlternatingSigns()};
    const std::tuple c_Tuple6Ref{};

    QVERIFY(vrd::areTuplesEqual(c_Tuple1Ref, c_Tuple1, 0));
    QVERIFY(vrd::areTuplesEqual(c_Tuple2Ref, c_Tuple2, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(c_Tuple3Ref, c_Tuple3, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(c_Tuple4Ref, c_Tuple4, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(c_Tuple5Ref, c_Tuple5, 0));
    QVERIFY(vrd::areTuplesEqual(c_Tuple6Ref, c_Tuple6, 0));

    const std::tuple c_Tuple7{vrd::createTupleWithCumulatedValues(3, -1, 2, 5, 8, 10)};
    const std::tuple c_Tuple7Ref{2, 4, 9, 17, 27};

    const std::tuple c_Tuple8{vrd::createTupleWithCumulatedValues(-5, 8)};
    const std::tuple c_Tuple8Ref{3};

    const std::tuple c_Tuple9{vrd::createTupleWithCumulatedValues(5)};
    const std::tuple c_Tuple9Ref{5};

    const std::tuple c_Tuple10{vrd::createTupleWithCumulatedValues(-1.2, 3.5, 2.0, 4.8)};
    const std::tuple c_Tuple10Ref{2.3, 4.3, 9.1};

    const std::tuple c_Tuple11{vrd::createTupleWithCumulatedValues(2.3, -1.2)};
    const std::tuple c_Tuple11Ref{1.1};

    const std::tuple c_Tuple12{vrd::createTupleWithCumulatedValues(-5.2)};
    const std::tuple c_Tuple12Ref{-5.2};

    QVERIFY(vrd::areTuplesEqual(c_Tuple7Ref, c_Tuple7, 0));
    QVERIFY(vrd::areTuplesEqual(c_Tuple8Ref, c_Tuple8, 0));
    QVERIFY(vrd::areTuplesEqual(c_Tuple9Ref, c_Tuple9, 0));
    QVERIFY(vrd::areTuplesEqual(c_Tuple10Ref, c_Tuple10, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(c_Tuple11Ref, c_Tuple11, 1.0e-9));
    QVERIFY(vrd::areTuplesEqual(c_Tuple12Ref, c_Tuple12, 1.0e-9));
}

QTEST_APPLESS_MAIN(CPP20ConceptsTests)

#include "tst_cpp20conceptstests.moc"
