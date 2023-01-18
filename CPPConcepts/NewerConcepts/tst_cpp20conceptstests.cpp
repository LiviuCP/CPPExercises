#include <QtTest>

#include <array>
#include <string_view>
#include <algorithm>

#include "datautils.h"

using namespace std::literals;

class CPP20ConceptsTests : public QObject
{
    Q_OBJECT

private slots:
    void testAbbreviatedFunctionTemplatesWithAutoParams();
    void testRangeBasedForLoopInit();
    void testMapSetContains();
    void testStringStartsEndsWith();
    void testStdErase();

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

    TripleSizeTuple _getMinMaxAvgSize(const auto& leftContainer, const auto& middleContainer, const auto& rightContainer) const;
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

void CPP20ConceptsTests::testAbbreviatedFunctionTemplatesWithAutoParams()
{
    constexpr std::string_view c_StringView{"This is another string from my code....."sv};
    const StringIntPairVector c_StringIntPairVector{{"ab", 1}, {"cd", 2}, {"ef", 3}, {"gh", 4}, {"ij", 5}, {"kl", 6}};
    const MatrixWrapper c_StringWrapper{4, 8, std::string{"abcd"}};

    const auto[c_FirstMinSize, c_FirstAvgSize, c_FirstMaxSize]{_getMinMaxAvgSize(c_StringView, c_StringIntPairVector, c_StringWrapper)};

    QVERIFY(6 == c_FirstMinSize && 26 == c_FirstAvgSize && 40 == c_FirstMaxSize);

    const MatrixWrapper c_IntWrapper{11, 3, -5};
    const RawContainer c_RawContainer{17};
    constexpr std::array<unsigned short, 10> c_ShortArray{2, 5, 4, 1, 0, 20, 15, 8, 11, 3};

    const auto[c_SecondMinSize, c_SecondAvgSize, c_SecondMaxSize]{_getMinMaxAvgSize(c_IntWrapper, c_RawContainer, c_ShortArray)};

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

QTEST_APPLESS_MAIN(CPP20ConceptsTests)

#include "tst_cpp20conceptstests.moc"
