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

QTEST_APPLESS_MAIN(CPP20ConceptsTests)

#include "tst_cpp20conceptstests.moc"
