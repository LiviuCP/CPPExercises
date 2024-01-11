#include <QtTest>

#include <algorithm>

#include "datautils.h"

class CPP2xRangesTests : public QObject
{
    Q_OBJECT

public:
    CPP2xRangesTests();

private slots:
    void testAllAnyNoneOf();
    void testFind();
    void testSort();

private:
    const StringIntPairVector mPrimaryStringIntPairVector;
};

CPP2xRangesTests::CPP2xRangesTests()
    : mPrimaryStringIntPairVector{{"Alex", 10}, {"Kevin", 11}, {"Alistair", 10}, {"George", 14}, {"Mark", 9},
                                  {"Andrew", 11}, {"Cameron", 10}, {"Reggie", 12}, {"Patrick", 14}, {"John", 11}}
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
}

QTEST_APPLESS_MAIN(CPP2xRangesTests)

#include "tst_cpp2xrangestests.moc"
