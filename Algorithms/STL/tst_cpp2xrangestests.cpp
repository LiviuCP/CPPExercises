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

private:
    std::optional<int> _retrieveEuclidianDistance(const IntPairVector& pointVector);

    const IntVector mPrimaryIntVector;
    const IntPairVector mPrimaryIntPairVector;
    const IntPairVector mSecondaryIntPairVector;
    const IntPairVector mThirdIntPairVector;
    const StringIntPairVector mPrimaryStringIntPairVector;
    const StringIntPairMatrix mPrimaryStringIntPairMatrix;
};

CPP2xRangesTests::CPP2xRangesTests()
    : mPrimaryIntVector{-3, 4, 2, 2, -5, -1, 0, 2, 3, 1, -4, 8}
    , mPrimaryIntPairVector{{-1, 2}, {0, 1}, {1, 2}, {1, 1}, {2, 1}, {-3, 2}, {2, -3}, {5, 1}, {0, 0}, {6, 2}, {-2, 3}, {4, 2}, {1, 0}, {2, 2}}
    , mSecondaryIntPairVector{{-2, 5}, {9, 4}, {6, 7}, {-4, -3}, {0, -1}, {3, 6}, {2, 1}, {5, 0}}
    , mThirdIntPairVector{{7, 8}, {-2, 2}, {7, -5}, {3, 4}, {-2, 9}}
    , mPrimaryStringIntPairVector{{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                  {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}}
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

QTEST_APPLESS_MAIN(CPP2xRangesTests)

#include "tst_cpp2xrangestests.moc"
