#include <QtTest>

#include <algorithm>

#include "datautils.h"

static constexpr auto lexicographicalCompare{[](const IntPair& firstIntPair, const IntPair& secondIntPair) {
    auto equivalence{firstIntPair.first <=> secondIntPair.first};

    if (0 == equivalence)
    {
        equivalence = firstIntPair.second <=> secondIntPair.second;
    }

    return equivalence < 0;
}};

static constexpr auto reverseProjection{[](const IntPair& intPair) {return IntPair{intPair.second, intPair.first};}};

template<typename DataType>
class MatrixDiagonalView : public std::ranges::view_interface<MatrixDiagonalView<DataType>>
{
public:
    MatrixDiagonalView() = default;
    MatrixDiagonalView(Matrix<DataType>& matrix, Matrix<DataType>::size_type diagonalNr)
    {
        if (!matrix.isEmpty())
        {
            diagonalNr = std::clamp(diagonalNr, 1 - matrix.getNrOfRows(), matrix.getNrOfColumns() - 1);
            mDiagBegin = matrix.dBegin(diagonalNr);
            mDiagEnd = matrix.dEnd(diagonalNr);
        }
    }

    auto begin() const
    {
        return mDiagBegin;
    }

    auto end() const
    {
        return mDiagEnd;
    }

private:
    typename Matrix<DataType>::DIterator mDiagBegin;
    typename Matrix<DataType>::DIterator mDiagEnd;
};

class CPP2xRangesTests : public QObject
{
    Q_OBJECT

public:
    CPP2xRangesTests();

private slots:
    void testAllAnyNoneOf();
    void testFind();
    void testSort();
    void testReverse();
    void testViews();
    void testKeyValueViews();
    void testJoinView();
    void testViewInterface();

private:
    std::optional<int> _retrieveEuclidianDistance(const IntPairVector& pointVector);
    IntVector _retrievePrimes(size_t count, std::optional<uint8_t> lastDigit = std::optional<char>{});
    bool _isPrime(int number);

    const IntVector mPrimaryIntVector;
    const IntPairVector mPrimaryIntPairVector;
    const IntPairVector mSecondaryIntPairVector;
    const IntPairVector mThirdIntPairVector;
    const StringIntPairVector mPrimaryStringIntPairVector;
    const IntMatrix mPrimaryIntMatrix;
    const IntMatrix mSecondaryIntMatrix;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
};

CPP2xRangesTests::CPP2xRangesTests()
    : mPrimaryIntVector{-3, 4, 2, 2, -5, -1, 0, 2, 3, 1, -4, 8}
    , mPrimaryIntPairVector{{-1, 2}, {0, 1}, {1, 2}, {1, 1}, {2, 1}, {-3, 2}, {2, -3}, {5, 1}, {0, 0}, {6, 2}, {-2, 3}, {4, 2}, {1, 0}, {2, 2}}
    , mSecondaryIntPairVector{{-2, 5}, {9, 4}, {6, 7}, {-4, -3}, {0, -1}, {3, 6}, {2, 1}, {5, 0}}
    , mThirdIntPairVector{{7, 8}, {-2, 2}, {7, -5}, {3, 4}, {-2, 9}}
    , mPrimaryStringIntPairVector{{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                  {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}}
    , mPrimaryIntMatrix{2, 3, {-1, 2, -3,
                                4, -5, 6
                       }}
    , mSecondaryIntMatrix{3, 3, {-7, 8, -9,
                                 10, -11, 12,
                                 -13, 14, -15
                         }}
    , mPrimaryStringIntPairMatrix{2, 5, {{"Anna", 18}, {"Kelly", 12}, {"Annabel", 11}, {"Juan", 10}, {"Jack", 8},
                                         {"Barbara", 10}, {"Barney", 20}, {"Joseph", 11}, {"Johnny", 9}, {"Jeff", 15}
                                 }}
{
}

void CPP2xRangesTests::testAllAnyNoneOf()
{
    QVERIFY(std::ranges::all_of(mPrimaryStringIntPairVector, [](int age) {return age > 8 && age < 18;}, [](const auto& person) {return person.second;}));
    QVERIFY(!std::ranges::all_of(mPrimaryStringIntPairVector, [](int age) {return age > 8 && age < 14;}, [](const auto& person) {return person.second;}));
    QVERIFY(std::ranges::any_of(mPrimaryStringIntPairVector, [](const std::string& name) {return name.size() < 5;}, [](const auto& person){return person.first;}));
    QVERIFY(!std::ranges::any_of(mPrimaryStringIntPairVector, [](const std::string& name) {return name.size() > 8;}, [](const auto& person){return person.first;}));
    QVERIFY(std::ranges::none_of(mPrimaryStringIntPairVector, [](const std::string& name) {return name.starts_with("reg");}, [](const auto& person){return person.first;}));
    QVERIFY(!std::ranges::none_of(mPrimaryStringIntPairVector, [](const std::string& name) {return name.starts_with("Reg");}, [](const auto& person){return person.first;}));
}

void CPP2xRangesTests::testFind()
{
    auto firstIt{std::ranges::find(mPrimaryStringIntPairVector, 14, [](const auto& person){return person.second;})};
    auto secondIt{std::ranges::adjacent_find(firstIt,
        std::end(mPrimaryStringIntPairVector),
        [](int firstPersonAge, int secondPersonAge){return std::abs(firstPersonAge - secondPersonAge) <= 1;},
        [](const auto& person){return person.second;})};

    QVERIFY("Andrew" == secondIt->first && 11 == secondIt->second);
}

void CPP2xRangesTests::testSort()
{
    StringIntPairVector firstStringIntPairVector{mPrimaryStringIntPairVector};
    const StringIntPairVector c_FirstStringIntPairVectorRef{{"Mark", 9}, {"Alex", 10}, {"Alistair", 10}, {"Kevin", 11}, {"Andrew", 11},
                                                            {"George", 14}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}};

    std::ranges::stable_sort(firstStringIntPairVector.begin(), firstStringIntPairVector.begin() + 6, std::less<int>(), [](const auto& person){return person.second;});

    QVERIFY(c_FirstStringIntPairVectorRef == firstStringIntPairVector);

    StringIntPairVector secondStringIntPairVector{mPrimaryStringIntPairVector};
    const StringIntPairVector c_SecondStringIntPairVectorRef{{"Alistair", 10}, {"Cameron", 10}, {"Patrick", 14}, {"Andrew", 11}, {"George", 14},
                                                             {"Reggie", 12}, {"Kevin", 11}, {"Alex", 10}, {"John", 11}, {"Mark", 9}};
    std::ranges::sort(secondStringIntPairVector,
        [](const std::string& firstPersonName, const std::string& secondPersonName) {return firstPersonName.size() > secondPersonName.size() || firstPersonName < secondPersonName;},
        [](const auto& person) {return person.first;});

    QVERIFY(c_SecondStringIntPairVectorRef == secondStringIntPairVector);

    IntPairVector firstIntPairVector{mPrimaryIntPairVector};
    const IntPairVector c_FirstIntPairVectorRef{{6, 2}, {5, 1}, {4, 2}, {2, 2}, {2, 1}, {2, -3}, {1, 2}, {1, 1}, {1, 0}, {0, 1}, {0, 0}, {-1, 2}, {-2, 3}, {-3, 2}};

    std::ranges::sort(firstIntPairVector.rbegin(), firstIntPairVector.rend(), lexicographicalCompare);

    QVERIFY(c_FirstIntPairVectorRef == firstIntPairVector);

    IntPairVector secondIntPairVector{mPrimaryIntPairVector};
    const IntPairVector c_SecondIntPairVectorRef{{-2, 3}, {6, 2}, {4, 2}, {2, 2}, {1, 2}, {-1, 2}, {-3, 2}, {5, 1}, {2, 1}, {1, 1}, {0, 1}, {1, 0}, {0, 0}, {2, -3}};

    std::ranges::sort(secondIntPairVector.rbegin(), secondIntPairVector.rend(), lexicographicalCompare, reverseProjection);

    QVERIFY(c_SecondIntPairVectorRef == secondIntPairVector);

    StringIntPairMatrix stringIntPairMatrix{mPrimaryStringIntPairMatrix};
    const StringIntPairMatrix c_StringIntPairMatrixRef{2, 5, {{"Annabel", 11}, {"Barbara", 10}, {"Barney", 20}, {"Joseph", 11}, {"Johnny", 9},
                                                              {"Kelly", 12}, {"Anna", 18}, {"Juan", 10}, {"Jack", 8}, {"Jeff", 15}
                                                      }};

    std::ranges::stable_sort(stringIntPairMatrix, std::greater<std::size_t>(), [](const auto& person) {return std::size(person.first);});

    QVERIFY(c_StringIntPairMatrixRef == stringIntPairMatrix);
}

void CPP2xRangesTests::testReverse()
{
    StringIntPairMatrix firstStringIntPairMatrix{mPrimaryStringIntPairMatrix};
    const StringIntPairMatrix c_FirstStringIntPairMatrixRef{2, 5, {{"Jeff", 15}, {"Johnny", 9}, {"Joseph", 11}, {"Barney", 20}, {"Barbara", 10},
                                                                   {"Jack", 8}, {"Juan", 10}, {"Annabel", 11}, {"Kelly", 12}, {"Anna", 18}
                                                           }};
    std::ranges::reverse(firstStringIntPairMatrix);

    StringIntPairMatrix secondStringIntPairMatrix{mPrimaryStringIntPairMatrix};
    const StringIntPairMatrix c_SecondStringIntPairMatrixRef{2, 5, {{"Anna", 18}, {"Annabel", 11}, {"Jack", 8}, {"Barney", 20}, {"Johnny", 9},
                                                                    {"Kelly", 12}, {"Juan", 10}, {"Barbara", 10}, {"Joseph", 11}, {"Jeff", 15}
                                                            }};

    std::ranges::reverse_copy(firstStringIntPairMatrix, secondStringIntPairMatrix.nBegin());

    StringIntPairVector stringIntPairVector{mPrimaryStringIntPairVector};
    const StringIntPairVector c_StringIntPairVectorRef{{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                                       {"Jeff", 15}, {"Joseph", 11}, {"Barbara", 10}, {"Patrick", 14}, {"John", 11}};

    std::ranges::reverse_copy(secondStringIntPairMatrix.constReverseZBegin(), secondStringIntPairMatrix.getConstReverseZIterator(1, 1), stringIntPairVector.rbegin() + 2);

    QVERIFY(c_FirstStringIntPairMatrixRef == firstStringIntPairMatrix &&
            c_SecondStringIntPairMatrixRef == secondStringIntPairMatrix &&
            c_StringIntPairVectorRef == stringIntPairVector);
}

void CPP2xRangesTests::testViews()
{
    IntVector intVector{mPrimaryIntVector};
    const IntVector c_IntVectorRef{12, 4, 2, 2, 10, -1, 15, 2, 18, 1, -4, 8};

    auto negativeValuesReversed{intVector | std::views::filter([](int element){return element < 0;}) | std::views::reverse};
    const IntVector c_NegativeValuesReversedRef{-4, -1, -5, -3};

    QVERIFY(std::ranges::equal(c_NegativeValuesReversedRef, negativeValuesReversed));

    auto valuesDivisibleByThreeOrFive{intVector | std::views::filter([](int element) {return 0 == element % 3 || 0 == element % 5;})};
    std::ranges::for_each(valuesDivisibleByThreeOrFive, [](int& element) {element += 15;});

    QVERIFY(std::ranges::equal(c_IntVectorRef, intVector));

    auto fourCharactersNamesItemsReversed{mPrimaryStringIntPairMatrix | std::views::filter([](const auto& person) {return 4 == std::size(person.first);}) | std::views::reverse};
    StringIntPairMatrix stringIntPairMatrix;
    stringIntPairMatrix.resize(2, 2);
    const StringIntPairMatrix c_StringIntPairMatrixRef{2, 2, {{"Jeff", 15}, {"Juan", 10},
                                                              {"Jack", 8}, {"Anna", 18}
                                                      }};

    std::ranges::copy(fourCharactersNamesItemsReversed, stringIntPairMatrix.nBegin());

    QVERIFY(c_StringIntPairMatrixRef == stringIntPairMatrix);

    QVERIFY(1 == _retrieveEuclidianDistance(mPrimaryIntPairVector));
    QVERIFY(8 == _retrieveEuclidianDistance(mSecondaryIntPairVector));
    QVERIFY(29 == _retrieveEuclidianDistance(mThirdIntPairVector));
    QVERIFY(41 == _retrieveEuclidianDistance({mPrimaryIntPairVector.at(6), mPrimaryIntPairVector.at(9)}));
    QVERIFY(40 == _retrieveEuclidianDistance({mSecondaryIntPairVector.at(1), mSecondaryIntPairVector.at(3), mSecondaryIntPairVector.at(5)}));
    QVERIFY(!_retrieveEuclidianDistance({mPrimaryIntPairVector.at(0)}).has_value());
    QVERIFY(!_retrieveEuclidianDistance({}).has_value());

    IntPairVector intPairVector{mSecondaryIntPairVector};
    intPairVector.push_back({-4, -3});

    QVERIFY(0 == _retrieveEuclidianDistance(intPairVector));

    const IntVector c_IntVector1{1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 53, 67, 79};
    const IntVector c_IntVector2{3, 29, 4, 71};
    const IntVector c_IntVector3{0, 9, 21, 33, 87};
    const IntVector c_IntVector4{-6, -4, 0, 4, 14};
    const IntVector c_IntVector5{-5, -9, -3, -2, -1};
    const IntVector c_IntVector6{-17, -13, -11, -7, 5, 19, 113, 97, 43};

    QVERIFY(std::ranges::all_of(c_IntVector1, [this](int number){return _isPrime(number);}));
    QVERIFY(std::ranges::any_of(c_IntVector2, [this](int number){return !_isPrime(number);}));
    QVERIFY(std::ranges::none_of(c_IntVector3, [this](int number){return _isPrime(number);}));
    QVERIFY(std::ranges::all_of(c_IntVector4, [this](int number){return !_isPrime(number);}));
    QVERIFY(std::ranges::any_of(c_IntVector5, [this](int number){return _isPrime(number);}));
    QVERIFY(std::ranges::none_of(c_IntVector6, [this](int number){return !_isPrime(number);}));

    const IntMatrix c_IntMatrix{3, 4, {1, -3,   2,  4,
                                       19, 4, -13,  6,
                                       7, -2,  15, -17
                               }};

    QVERIFY(std::ranges::any_of(c_IntMatrix.constMBegin(1), c_IntMatrix.constMEnd(1), [this](int number) {return _isPrime(number) && 0 == number % 2;}));
    QVERIFY(-28 == std::accumulate(c_IntMatrix.constNColumnBegin(2), c_IntMatrix.constNEnd(), 0, [this](int first, int second){return first + (_isPrime(second) ? second : 0);}));

    const IntVector c_PrimesVectorRef1{1, 2, 3, 5, 7, 11, 13, 17};
    QVERIFY(c_PrimesVectorRef1 == _retrievePrimes(8));

    const IntVector c_PrimesVectorRef2{1, 11, 31, 41, 61};
    QVERIFY(c_PrimesVectorRef2 == _retrievePrimes(5, 1) && c_PrimesVectorRef2 == _retrievePrimes(5, 11));

    const IntVector c_PrimesVectorRef3{3, 13, 23, 43, 53, 73, 83, 103, 113};
    QVERIFY(c_PrimesVectorRef3 == _retrievePrimes(9, 3) && c_PrimesVectorRef3 == _retrievePrimes(9, 23));

    const IntVector c_PrimesVectorRef4{7, 17, 37, 47, 67, 97};
    QVERIFY(c_PrimesVectorRef4 == _retrievePrimes(6, 7) && c_PrimesVectorRef4 == _retrievePrimes(6, 67));

    const IntVector c_PrimesVectorRef5{19};
    QVERIFY(c_PrimesVectorRef5 == _retrievePrimes(1, 9) && c_PrimesVectorRef5 == _retrievePrimes(1, 109));

    const IntVector c_PrimesVectorRef6{2};
    QVERIFY(c_PrimesVectorRef6 == _retrievePrimes(2, 2) && c_PrimesVectorRef6 == _retrievePrimes(1, 2) && c_PrimesVectorRef6 == _retrievePrimes(3, 32));

    const IntVector c_PrimesVectorRef7{5};
    QVERIFY(c_PrimesVectorRef7 == _retrievePrimes(2, 5) && c_PrimesVectorRef7 == _retrievePrimes(1, 5) && c_PrimesVectorRef7 == _retrievePrimes(4, 45));

    QVERIFY(_retrievePrimes(1, 0).empty() && _retrievePrimes(1, 4).empty() && _retrievePrimes(1, 6).empty() && _retrievePrimes(1, 8).empty());
    QVERIFY(_retrievePrimes(1, 10).empty() && _retrievePrimes(1, 24).empty() && _retrievePrimes(1, 36).empty() && _retrievePrimes(1, 48).empty());
    QVERIFY(_retrievePrimes(0, 2).empty() && _retrievePrimes(0, 5).empty() && _retrievePrimes(0, 7).empty() && _retrievePrimes(0, 8).empty());
    QVERIFY(_retrievePrimes(0, 12).empty() && _retrievePrimes(0, 25).empty() && _retrievePrimes(0, 47).empty() && _retrievePrimes(0, 78).empty());
    QVERIFY(_retrievePrimes(0).empty());
}

void CPP2xRangesTests::testKeyValueViews()
{
    auto keysView{mPrimaryStringIntPairMatrix | std::ranges::views::keys};
    auto reverseValuesView{mPrimaryStringIntPairMatrix | std::ranges::views::values | std::ranges::views::reverse};

    StringMatrix keysMatrix{5, 2, ""};

    const StringMatrix c_KeysMatrixRef{5, 2, {"Jeff",    "Jack",
                                              "Johnny",  "Juan",
                                              "Joseph",  "Annabel",
                                              "Barney",  "Kelly",
                                              "Barbara", "Anna"
                                      }};

    std::ranges::copy(keysView, keysMatrix.reverseNBegin());

    QVERIFY(c_KeysMatrixRef == keysMatrix);

    IntMatrix valuesMatrix{5, 2, -1};

    const IntMatrix c_ValuesMatrixRef{5, 2, {15,  8,
                                              9, 10,
                                             11, 11,
                                             20, 12,
                                             10, 18
                                     }};

    std::ranges::copy(reverseValuesView, valuesMatrix.nBegin());

    QVERIFY(c_ValuesMatrixRef == valuesMatrix);
}

void CPP2xRangesTests::testJoinView()
{
    /* Case 1: vertical concatenation of two matrixes */

    IntMatrix firstIntMatrix{mPrimaryIntMatrix};
    IntMatrix secondIntMatrix{mSecondaryIntMatrix};

    std::vector<IntMatrix> matrixesToJoin{firstIntMatrix, secondIntMatrix};
    auto joinMatrixesView{matrixesToJoin | std::ranges::views::join};

    IntMatrix thirdIntMatrix{5, 3, 0};

    std::ranges::copy(joinMatrixesView, thirdIntMatrix.begin());

    QVERIFY(-1 == thirdIntMatrix.at(0, 0) &&
            -15 == thirdIntMatrix.at(4, 2) &&
            -7 == thirdIntMatrix.at(2, 0) &&
            -8 == std::accumulate(thirdIntMatrix.constZBegin(), thirdIntMatrix.constZEnd(), 0));

    firstIntMatrix.catByRow(firstIntMatrix, secondIntMatrix);

    QVERIFY(firstIntMatrix == thirdIntMatrix);

    /* Case 2: horizontal concatenation of two matrixes */

    firstIntMatrix.resize(3, 2);
    std::ranges::copy(mPrimaryIntMatrix, firstIntMatrix.zBegin());
    secondIntMatrix = mSecondaryIntMatrix;
    thirdIntMatrix.resize(3, 5, 0);

    IntMatrix firstIntMatrixTransposed, secondIntMatrixTransposed;

    firstIntMatrix.transpose(firstIntMatrixTransposed);
    secondIntMatrix.transpose(secondIntMatrixTransposed);

    matrixesToJoin = {firstIntMatrixTransposed, secondIntMatrixTransposed};
    joinMatrixesView = matrixesToJoin | std::ranges::views::join;

    std::ranges::copy(joinMatrixesView, thirdIntMatrix.nBegin());

    QVERIFY(-1 == thirdIntMatrix.at(0, 0) &&
            -15 == thirdIntMatrix.at(2, 4) &&
            -7 == thirdIntMatrix.at(0, 2) &&
            -8 == std::accumulate(thirdIntMatrix.constNBegin(), thirdIntMatrix.constNEnd(), 0));

    secondIntMatrix.catByColumn(firstIntMatrix, secondIntMatrix);

    QVERIFY(secondIntMatrix == thirdIntMatrix);
}

void CPP2xRangesTests::testViewInterface()
{
    IntMatrix firstIntMatrix{mSecondaryIntMatrix};
    MatrixDiagonalView firstDiagView{firstIntMatrix, 0};

    QVERIFY(-33 == std::accumulate(firstDiagView.begin(), firstDiagView.end(), 0));
    QVERIFY(3 == std::distance(firstDiagView.begin(), firstDiagView.end()));

    IntMatrix secondIntMatrix;
    MatrixDiagonalView secondDiagView{secondIntMatrix, 0};

    QVERIFY(0 == std::accumulate(secondDiagView.begin(), secondDiagView.end(), 0));
    QVERIFY(secondDiagView.begin() == secondDiagView.end());

    IntMatrix thirdIntMatrix{mPrimaryIntMatrix};
    MatrixDiagonalView thirdDiagView{thirdIntMatrix, 1};
    auto reversedThirdDiagView{thirdDiagView | std::ranges::views::reverse};

    const IntMatrix c_ThirdIntMatrixRef{2, 3, {6, 2, -3,
                                               4, 2,  6
                                       }};

    std::ranges::copy(reversedThirdDiagView, thirdIntMatrix.dBegin(0));

    QVERIFY(c_ThirdIntMatrixRef == thirdIntMatrix);

    IntMatrix fourthIntMatrix{mPrimaryIntMatrix};
    MatrixDiagonalView fourthDiagView{fourthIntMatrix, -2};
    MatrixDiagonalView fifthDiagView{fourthIntMatrix, 3};

    const IntMatrix c_FourthIntMatrixRef{2, 3, {-1,  2, 4,
                                                -3, -5, 6
                                        }};

    std::ranges::iter_swap(fourthDiagView.begin(), fifthDiagView.begin());

    QVERIFY(c_FourthIntMatrixRef == fourthIntMatrix);
}

std::optional<int> CPP2xRangesTests::_retrieveEuclidianDistance(const IntPairVector& pointVector)
{
    auto getMinDistance{[](const IntPairVector& sortedPointVector){
        auto pointVectorLeftView{sortedPointVector | std::views::drop(1)};
        auto pointVectorRightView{sortedPointVector | std::views::reverse | std::views::drop(1) | std::views::reverse};

        IntVector resultingDistances;
        resultingDistances.resize(sortedPointVector.size() - 1);
        std::ranges::transform(pointVectorLeftView,
                               pointVectorRightView,
                               resultingDistances.begin(),
                               [](const IntPair& firstPoint, const IntPair& secondPoint) {
                                   const auto&[firstPointX, firstPointY]{firstPoint};
                                   const auto&[secondPointX, secondPointY]{secondPoint};

                                   return (firstPointX - secondPointX) * (firstPointX - secondPointX) +
                                          (firstPointY - secondPointY) * (firstPointY - secondPointY);
                               });

        const auto minElementIt{std::ranges::min_element(resultingDistances)};

        return *minElementIt;
    }};

    std::optional<int> distance;

    if (pointVector.size() > 1)
    {
        // two copies, each one to be sorted in a different way -> resulting minimum distances to be aggregated in a "global" minimum distance (Euclidian distance)
        IntPairVector pointsSortedByXY{pointVector};
        IntPairVector pointsSortedByYX{pointVector};

        std::ranges::sort(pointsSortedByXY, lexicographicalCompare);
        std::ranges::sort(pointsSortedByYX, lexicographicalCompare, reverseProjection);

        distance = std::min(getMinDistance(pointsSortedByXY), getMinDistance(pointsSortedByYX));
    }

    return distance;
}

IntVector CPP2xRangesTests::_retrievePrimes(size_t count, std::optional<uint8_t> lastDigit)
{
    IntVector result;

    if (lastDigit.has_value())
    {
        lastDigit = lastDigit.value() % 10;
    }

    /* Generate all positive primes: 1, 2, 3, 5, 7, ...
       - if no filtering by last digit is required, then the required count is taken from the (infinite) "raw" prime numbers sequence
       - if filtering by last digit is required, then the required count is taken from the prime numbers sequence filtered by digit (which is also infinite)
       - the (filtered) corner cases (2, 5) are handled separately (see below); the view is not used for retrieving them
       - filtering by an even digits (other than 2) is also handled separately (see below) - obviously empty result (they are not prime)
    */
    auto primeNumbersView{std::views::iota(0)
                          | std::views::transform([](int number){return number > 1 ? 2 * number - 1 : number + 1;})
                          | std::views::filter([this](int number){return _isPrime(number);})
                          | std::views::filter([lastDigit](int number) {return !lastDigit.has_value() || static_cast<uint8_t>(number % 10) == lastDigit;})
                          | std::views::take(count)};

    bool shouldUseView{!lastDigit.has_value()};

    if (!shouldUseView)
    {
        if (2 == lastDigit || 5 == lastDigit)
        {
            if (count > 0)
            {
                result.push_back(lastDigit.value());
            }
        }
        else if (lastDigit.value() % 2 != 0)
        {
            shouldUseView = true;
        }
    }

    if (shouldUseView)
    {
        result.resize(count);
        std::ranges::copy(primeNumbersView, result.begin());
    }

    return result;
}

bool CPP2xRangesTests::_isPrime(int number)
{
    number = std::abs(number);
    bool isPrime{2 == number || 5 == number}; // rule out corner cases (these are the only prime numbers ending with these digits)

    if (!isPrime)
    {
        constexpr std::array<uint8_t, 4> c_LastDigitsOfPrimes{1, 3, 7, 9}; // all prime numbers except the two above end with these digits

        if (std::ranges::any_of(c_LastDigitsOfPrimes, [number](uint8_t digit) {return digit == static_cast<uint8_t>(number % 10);}))
        {
            isPrime = true;

            /* generate all potential divisors (stop at upper bound), then check if they divide the number
               - possible divisors should end with "prime number digits", so as many non-prime numbers as possible are excluded (to avoid unnecessary calculations)
               - the resulting potential divisors are not necessarily prime (wanted to avoid checking them for prime-ness as well thereby preventing a recursion)
               - however by ending with the required digits they include all possible prime divisors
            */
            const int c_DivisorsBound{static_cast<int>(std::sqrt(number)) + 1};
            constexpr std::array<int, 4> c_StartingDivisors{3, 7, 9, 11};

            for (const auto& startingDivisor : c_StartingDivisors)
            {
                auto divisorsView{std::views::iota(0) |
                                  std::views::transform([startingDivisor](int index) {return startingDivisor + 10 * index;}) |
                                  std::views::take_while([c_DivisorsBound](int divisor) {return divisor < c_DivisorsBound;})};

                if (std::ranges::any_of(divisorsView, [number](int divisor){return 0 == number % divisor;}))
                {
                    isPrime = false;
                    break;
                }
            }
        }
    }

    return isPrime;
}

QTEST_APPLESS_MAIN(CPP2xRangesTests)

#include "tst_cpp2xrangestests.moc"
