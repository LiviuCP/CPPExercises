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

    void testWhiteSpaceTrimming_data();
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

QTEST_APPLESS_MAIN(DataUtilsTests)

#include "tst_datautilstests.moc"
