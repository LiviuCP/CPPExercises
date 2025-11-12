// clang-format off
#include <QTest>

#include "datautils.h"

enum class TrimOperation
{
    TRIM_LEFT = 0,
    TRIM_RIGHT,
    TRIM_BOTH_SIDES
};

class DataUtilsTests : public QObject
{
    Q_OBJECT

private slots:
    void testWhiteSpaceTrimming();
    void testConvertBitStringToDataWord();
    void testInvertDataWord();

    void testWhiteSpaceTrimming_data();
    void testConvertBitStringToDataWord_data();
    void testInvertDataWord_data();
};

void DataUtilsTests::testWhiteSpaceTrimming()
{
    QFETCH(std::string, stringToTrim);
    QFETCH(TrimOperation, trimOperation);
    QFETCH(std::string, requiredValueAfterTrimming);

    switch(trimOperation)
    {
    case TrimOperation::TRIM_LEFT:
        Utilities::leftTrimWhiteSpace(stringToTrim);
        break;
    case TrimOperation::TRIM_RIGHT:
        Utilities::rightTrimWhiteSpace(stringToTrim);
        break;
    case TrimOperation::TRIM_BOTH_SIDES:
        Utilities::trimWhiteSpace(stringToTrim);
        break;
    default:
        QFAIL("Invalid trimming operation!");
        break;
    }

    QVERIFY(stringToTrim == requiredValueAfterTrimming);
}

void DataUtilsTests::testConvertBitStringToDataWord()
{
    QFETCH(std::string, bitString);
    QFETCH(bool, expectedResult);
    QFETCH(DataWord, expectedDataWord);

    DataWord dataWord;
    const bool c_Result{Utilities::convertBitStringToDataWord(bitString, dataWord)};

    QVERIFY(c_Result == expectedResult);

    if (c_Result)
    {
        QVERIFY(dataWord == expectedDataWord);
    }
}

void DataUtilsTests::testInvertDataWord()
{
    QFETCH(DataWord, dataWord);
    QFETCH(DataWord, expectedDataWord);

    const DataWord c_InvertedDataWord{Utilities::invertDataWord(dataWord)};
    QVERIFY(c_InvertedDataWord == expectedDataWord);
}

void DataUtilsTests::testWhiteSpaceTrimming_data()
{
    QTest::addColumn<std::string>("stringToTrim");
    QTest::addColumn<TrimOperation>("trimOperation");
    QTest::addColumn<std::string>("requiredValueAfterTrimming");

    QTest::newRow("trim left: 1") << std::string{"  abcdefghij"} << TrimOperation::TRIM_LEFT << std::string{"abcdefghij"};
    QTest::newRow("trim left: 2") << std::string{"abcdefghij  "} << TrimOperation::TRIM_LEFT << std::string{"abcdefghij  "};
    QTest::newRow("trim left: 3") << std::string{"  abcdefghij  "} << TrimOperation::TRIM_LEFT << std::string{"abcdefghij  "};
    QTest::newRow("trim left: 4") << std::string{"abcdefghij"} << TrimOperation::TRIM_LEFT << std::string{"abcdefghij"};
    QTest::newRow("trim left: 5") << std::string{"  abcde fghij"} << TrimOperation::TRIM_LEFT << std::string{"abcde fghij"};
    QTest::newRow("trim left: 6") << std::string{"abcde fghij  "} << TrimOperation::TRIM_LEFT << std::string{"abcde fghij  "};
    QTest::newRow("trim left: 7") << std::string{"  abcde fghij  "} << TrimOperation::TRIM_LEFT << std::string{"abcde fghij  "};
    QTest::newRow("trim left: 8") << std::string{"abcde fghij"} << TrimOperation::TRIM_LEFT << std::string{"abcde fghij"};
    QTest::newRow("trim left: 9") << std::string{" a b"} << TrimOperation::TRIM_LEFT << std::string{"a b"};
    QTest::newRow("trim left: 10") << std::string{"a b "} << TrimOperation::TRIM_LEFT << std::string{"a b "};
    QTest::newRow("trim left: 11") << std::string{" a b "} << TrimOperation::TRIM_LEFT << std::string{"a b "};
    QTest::newRow("trim left: 12") << std::string{"a b"} << TrimOperation::TRIM_LEFT << std::string{"a b"};
    QTest::newRow("trim left: 13") << std::string{" a"} << TrimOperation::TRIM_LEFT << std::string{"a"};
    QTest::newRow("trim left: 14") << std::string{"a "} << TrimOperation::TRIM_LEFT << std::string{"a "};
    QTest::newRow("trim left: 15") << std::string{" a "} << TrimOperation::TRIM_LEFT << std::string{"a "};
    QTest::newRow("trim left: 16") << std::string{"a"} << TrimOperation::TRIM_LEFT << std::string{"a"};
    QTest::newRow("trim left: 17") << std::string{" "} << TrimOperation::TRIM_LEFT << std::string{""};
    QTest::newRow("trim left: 18") << std::string{""} << TrimOperation::TRIM_LEFT << std::string{""};
    QTest::newRow("trim right: 1") << std::string{"  abcdefghij"} << TrimOperation::TRIM_RIGHT << std::string{"  abcdefghij"};
    QTest::newRow("trim right: 2") << std::string{"abcdefghij  "} << TrimOperation::TRIM_RIGHT << std::string{"abcdefghij"};
    QTest::newRow("trim right: 3") << std::string{"  abcdefghij  "} << TrimOperation::TRIM_RIGHT << std::string{"  abcdefghij"};
    QTest::newRow("trim right: 4") << std::string{"abcdefghij"} << TrimOperation::TRIM_RIGHT << std::string{"abcdefghij"};
    QTest::newRow("trim right: 5") << std::string{"  abcde fghij"} << TrimOperation::TRIM_RIGHT << std::string{"  abcde fghij"};
    QTest::newRow("trim right: 6") << std::string{"abcde fghij  "} << TrimOperation::TRIM_RIGHT << std::string{"abcde fghij"};
    QTest::newRow("trim right: 7") << std::string{"  abcde fghij  "} << TrimOperation::TRIM_RIGHT << std::string{"  abcde fghij"};
    QTest::newRow("trim right: 8") << std::string{"abcde fghij"} << TrimOperation::TRIM_RIGHT << std::string{"abcde fghij"};
    QTest::newRow("trim right: 9") << std::string{" a b"} << TrimOperation::TRIM_RIGHT << std::string{" a b"};
    QTest::newRow("trim right: 10") << std::string{"a b "} << TrimOperation::TRIM_RIGHT << std::string{"a b"};
    QTest::newRow("trim right: 11") << std::string{" a b "} << TrimOperation::TRIM_RIGHT << std::string{" a b"};
    QTest::newRow("trim right: 12") << std::string{"a b"} << TrimOperation::TRIM_RIGHT << std::string{"a b"};
    QTest::newRow("trim right: 13") << std::string{" a"} << TrimOperation::TRIM_RIGHT << std::string{" a"};
    QTest::newRow("trim right: 14") << std::string{"a "} << TrimOperation::TRIM_RIGHT << std::string{"a"};
    QTest::newRow("trim right: 15") << std::string{" a "} << TrimOperation::TRIM_RIGHT << std::string{" a"};
    QTest::newRow("trim right: 16") << std::string{"a"} << TrimOperation::TRIM_RIGHT << std::string{"a"};
    QTest::newRow("trim right: 17") << std::string{" "} << TrimOperation::TRIM_RIGHT << std::string{""};
    QTest::newRow("trim right: 18") << std::string{""} << TrimOperation::TRIM_RIGHT << std::string{""};
    QTest::newRow("trim both sides: 1") << std::string{"  abcdefghij"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcdefghij"};
    QTest::newRow("trim both sides: 2") << std::string{"abcdefghij  "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcdefghij"};
    QTest::newRow("trim both sides: 3") << std::string{"  abcdefghij  "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcdefghij"};
    QTest::newRow("trim both sides: 4") << std::string{"abcdefghij"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcdefghij"};
    QTest::newRow("trim both sides: 5") << std::string{"  abcde fghij"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcde fghij"};
    QTest::newRow("trim both sides: 6") << std::string{"abcde fghij  "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcde fghij"};
    QTest::newRow("trim both sides: 7") << std::string{"  abcde fghij  "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcde fghij"};
    QTest::newRow("trim both sides: 8") << std::string{"abcde fghij"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"abcde fghij"};
    QTest::newRow("trim both sides: 9") << std::string{" a b"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a b"};
    QTest::newRow("trim both sides: 10") << std::string{"a b "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a b"};
    QTest::newRow("trim both sides: 11") << std::string{" a b "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a b"};
    QTest::newRow("trim both sides: 12") << std::string{"a b"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a b"};
    QTest::newRow("trim both sides: 13") << std::string{" a"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a"};
    QTest::newRow("trim both sides: 14") << std::string{"a "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a"};
    QTest::newRow("trim both sides: 15") << std::string{" a "} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a"};
    QTest::newRow("trim both sides: 16") << std::string{"a"} << TrimOperation::TRIM_BOTH_SIDES << std::string{"a"};
    QTest::newRow("trim both sides: 17") << std::string{" "} << TrimOperation::TRIM_BOTH_SIDES << std::string{""};
    QTest::newRow("trim both sides: 18") << std::string{""} << TrimOperation::TRIM_BOTH_SIDES << std::string{""};
}

void DataUtilsTests::testConvertBitStringToDataWord_data()
{
    QTest::addColumn<std::string>("bitString");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<DataWord>("expectedDataWord");

    QTest::newRow("valid bitstring: 1") << std::string{"0010110101011010"} << true << DataWord{0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0};
    QTest::newRow("valid bitstring: 2") << std::string{"1101001010100101"} << true << DataWord{1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1};
    QTest::newRow("valid bitstring: 3") << std::string{"11111111"} << true << DataWord{1, 1, 1, 1, 1, 1, 1, 1};
    QTest::newRow("valid bitstring: 4") << std::string{"0000"} << true << DataWord{0, 0, 0, 0};
    QTest::newRow("valid bitstring: 5") << std::string{"00"} << true << DataWord{0, 0};
    QTest::newRow("valid bitstring: 6") << std::string{"01"} << true << DataWord{0, 1};
    QTest::newRow("valid bitstring: 7") << std::string{"10"} << true << DataWord{1, 0};
    QTest::newRow("valid bitstring: 8") << std::string{"11"} << true << DataWord{1, 1};
    QTest::newRow("valid bitstring: 9") << std::string{"0"} << true << DataWord{0};
    QTest::newRow("valid bitstring: 10") << std::string{"1"} << true << DataWord{1};
    QTest::newRow("invalid bitstring: 1") << std::string{"10102010"} << false << DataWord{};
    QTest::newRow("invalid bitstring: 2") << std::string{"1010_010"} << false << DataWord{};
    QTest::newRow("invalid bitstring: 3") << std::string{"1010a010"} << false << DataWord{};
    QTest::newRow("invalid bitstring: 4") << std::string{"1010 010"} << false << DataWord{};
    QTest::newRow("invalid bitstring: 5") << std::string{" 0101010"} << false << DataWord{};
    QTest::newRow("invalid bitstring: 6") << std::string{"1010101 "} << false << DataWord{};
    QTest::newRow("invalid bitstring: 7") << std::string{"+01\n101a"} << false << DataWord{};
    QTest::newRow("invalid bitstring: 8") << std::string{2} << false << DataWord{};
    QTest::newRow("invalid bitstring: 9") << std::string{ } << false << DataWord{};
    QTest::newRow("invalid bitstring: 10") << std::string{} << false << DataWord{};
}

void DataUtilsTests::testInvertDataWord_data()
{
    QTest::addColumn<DataWord>("dataWord");
    QTest::addColumn<DataWord>("expectedDataWord");

    QTest::newRow("invert data word: 1") << DataWord{0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0} << DataWord{1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1};
    QTest::newRow("invert data word: 2") << DataWord{1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1} << DataWord{0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0};
    QTest::newRow("invert data word: 3") << DataWord{0, 0, 0, 0, 0, 0, 0, 0} << DataWord{1, 1, 1, 1, 1, 1, 1, 1};
    QTest::newRow("invert data word: 4") << DataWord{1, 1, 1, 1, 1, 1, 1, 1} << DataWord{0, 0, 0, 0, 0, 0, 0, 0};
    QTest::newRow("invert data word: 5") << DataWord{0, 1, 0, 1} << DataWord{1, 0, 1, 0};
    QTest::newRow("invert data word: 6") << DataWord{1, 0, 1, 0} << DataWord{0, 1, 0, 1};
    QTest::newRow("invert data word: 7") << DataWord{0, 0, 1} << DataWord{1, 1, 0};
    QTest::newRow("invert data word: 8") << DataWord{1, 1, 0} << DataWord{0, 0, 1};
    QTest::newRow("invert data word: 9") << DataWord{0, 0} << DataWord{1, 1};
    QTest::newRow("invert data word: 10") << DataWord{0, 1} << DataWord{1, 0};
    QTest::newRow("invert data word: 11") << DataWord{1, 0} << DataWord{0, 1};
    QTest::newRow("invert data word: 12") << DataWord{1, 1} << DataWord{0, 0};
    QTest::newRow("invert data word: 13") << DataWord{0} << DataWord{1};
    QTest::newRow("invert data word: 14") << DataWord{1} << DataWord{0};
    QTest::newRow("invert data word: 15") << DataWord{} << DataWord{};
}

QTEST_APPLESS_MAIN(DataUtilsTests)

#include "tst_datautilstests.moc"
// clang-format on
