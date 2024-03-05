#include <QtTest>

#include <regex>

class RegexTests : public QObject
{
    Q_OBJECT

public:
    enum class IpClass : uint8_t
    {
        A = 0,
        B,
        C,
        D,
        Invalid
    };

private slots:
    void testIpAddressClasses();
    void testIpAddressClasses_data();

private:
    IpClass _parseIpAddress(const std::string& ipAddress);

    bool _isClassAIpAddress(const std::string& ipAddress);
    bool _isClassBIpAddress(const std::string& ipAddress);
    bool _isClassCIpAddress(const std::string& ipAddress);
    bool _isClassDIpAddress(const std::string& ipAddress);
};

void RegexTests::testIpAddressClasses()
{
    QFETCH(std::string, ipAddress);
    QFETCH(IpClass, ipClass);

    QVERIFY(ipClass == _parseIpAddress(ipAddress));
}

void RegexTests::testIpAddressClasses_data()
{
    QTest::addColumn<std::string>("ipAddress");
    QTest::addColumn<IpClass>("ipClass");

    QTest::newRow("1_C") << std::string{"192.168.1.4"} << IpClass::C;
    QTest::newRow("2_B") << std::string{"190.122.2.0"} << IpClass::B;
    QTest::newRow("3_A") << std::string{"9.0.1.0"} << IpClass::A;
    QTest::newRow("4_A") << std::string{"9.1.0.0"} << IpClass::A;
    QTest::newRow("5_A") << std::string{"9.1.1.0"} << IpClass::A;
    QTest::newRow("6_A") << std::string{"10.0.0.1"} << IpClass::A;
    QTest::newRow("7_C") << std::string{"198.0.2.3"} << IpClass::C;
    QTest::newRow("8_C") << std::string{"223.192.168.1"} << IpClass::C;
    QTest::newRow("9_C") << std::string{"194.168.255.2"} << IpClass::C;
    QTest::newRow("10_B") << std::string{"186.168.255.2"} << IpClass::B;
    QTest::newRow("11_A") << std::string{"12.1.1.255"} << IpClass::A;
    QTest::newRow("12_A") << std::string{"1.255.0.0"} << IpClass::A;
    QTest::newRow("13_C") << std::string{"195.24.35.254"} << IpClass::C;
    QTest::newRow("14_B") << std::string{"184.0.0.3"} << IpClass::B;
    QTest::newRow("15_C") << std::string{"202.4.5.6"} << IpClass::C;
    QTest::newRow("16_A") << std::string{"11.12.14.13"} << IpClass::A;
    QTest::newRow("17_B") << std::string{"129.1.2.3"} << IpClass::B;
    QTest::newRow("18_A") << std::string{"14.0.255.255"} << IpClass::A;
    QTest::newRow("19_B") << std::string{"191.168.2.255"} << IpClass::B;
    QTest::newRow("20_D") << std::string{"225.4.4.5"} << IpClass::D;
    QTest::newRow("21_D") << std::string{"239.0.1.0"} << IpClass::D;
    QTest::newRow("22_D") << std::string{"239.255.255.255"} << IpClass::D;
    QTest::newRow("23_Invalid") << std::string{"244.1.2.3"} << IpClass::Invalid;
    QTest::newRow("24_Invalid") << std::string{"191.168.255.255"} << IpClass::Invalid;
    QTest::newRow("25_Invalid") << std::string{"195.122.1.255"} << IpClass::Invalid;
    QTest::newRow("26_Invalid") << std::string{"0.0.0.1"} << IpClass::Invalid;
    QTest::newRow("27_Invalid") << std::string{"0192.170.4.4"} << IpClass::Invalid;
    QTest::newRow("28_Invalid") << std::string{"192.168.1O.50"} << IpClass::Invalid;
    QTest::newRow("29_Invalid") << std::string{"192.0170.4.4"} << IpClass::Invalid;
    QTest::newRow("30_Invalid") << std::string{"0.0.1.1"} << IpClass::Invalid;
    QTest::newRow("31_Invalid") << std::string{"188.+1.7.7"} << IpClass::Invalid;
    QTest::newRow("32_Invalid") << std::string{"240.1.2.3"} << IpClass::Invalid;
    QTest::newRow("33_Invalid") << std::string{"192.168.1."} << IpClass::Invalid;
    QTest::newRow("34_Invalid") << std::string{"12.1.2.1.2"} << IpClass::Invalid;
    QTest::newRow("35_Invalid") << std::string{"12.255.255.255"} << IpClass::Invalid;
    QTest::newRow("36_Invalid") << std::string{"0.1.1.1"} << IpClass::Invalid;
    QTest::newRow("37_Invalid") << std::string{"225.4 0.1.0"} << IpClass::Invalid;
    QTest::newRow("38_Invalid") << std::string{"189.1.0.0"} << IpClass::Invalid;
    QTest::newRow("39_Invalid") << std::string{"188..2.3"} << IpClass::Invalid;
    QTest::newRow("40_Invalid") << std::string{"127.0.0.1"} << IpClass::Invalid;
    QTest::newRow("41_Invalid") << std::string{"192.170.04.4"} << IpClass::Invalid;
    QTest::newRow("42_Invalid") << std::string{"9.0.0.0"} << IpClass::Invalid;
    QTest::newRow("43_Invalid") << std::string{"192.170.4.04"} << IpClass::Invalid;
    QTest::newRow("44_Invalid") << std::string{"0.0.0.0"} << IpClass::Invalid;
    QTest::newRow("45_Invalid") << std::string{"192.168.256.1"} << IpClass::Invalid;
    QTest::newRow("46_Invalid") << std::string{"15.-1.4.2"} << IpClass::Invalid;
    QTest::newRow("47_Invalid") << std::string{"255.255.255.255"} << IpClass::Invalid;
    QTest::newRow("48_Invalid") << std::string{"0.1.0.0"} << IpClass::Invalid;
    QTest::newRow("49_Invalid") << std::string{"10.256.10.0"} << IpClass::Invalid;
    QTest::newRow("50_Invalid") << std::string{"127.44.1.2"} << IpClass::Invalid;
}

/* The (host) address should:
   - have the right format
   - belong to a valid class (class A: 1 - 126; class B: 128 - 191; class C: 192 - 223; class D: 224 - 239)
   - not be a network-only address (no host part) - if it belongs to classes A/B/C
   - not be a broadcast address - if it belongs to classes A/B/C
*/
RegexTests::IpClass RegexTests::_parseIpAddress(const std::string& ipAddress)
{
    return _isClassAIpAddress(ipAddress) ? IpClass::A
         : _isClassBIpAddress(ipAddress) ? IpClass::B
         : _isClassCIpAddress(ipAddress) ? IpClass::C
         : _isClassDIpAddress(ipAddress) ? IpClass::D
                                         : IpClass::Invalid;
}

bool RegexTests::_isClassAIpAddress(const std::string& ipAddress)
{
    // network (e.g. 10.0.0.0) and broadcast (e.g. 12.255.255.255) addresses to be excluded
    std::regex classANetworkOrBroadcastAddressRe{"(12[0-6]|1[0-1][0-9]|[1-9][0-9]|[1-9])((\\.255){3}|(\\.0){3})"};

    std::regex ipAddressClassARe{"(12[0-6]|1[0-1]\\d|[1-9]\\d|[1-9])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){3}"};

    return !std::regex_match(ipAddress, classANetworkOrBroadcastAddressRe) &&
           std::regex_match(ipAddress, ipAddressClassARe);
}

bool RegexTests::_isClassBIpAddress(const std::string& ipAddress)
{
    // network (e.g. 190.2.0.0) and broadcast (e.g. 191.4.255.255) addresses to be excluded
    std::regex classBNetworkOrBroadcastAddressRe{"(12[8-9]|1[3-8]\\d|19[0-1])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d))((\\.255){2}|(\\.0){2})"};
    std::regex ipAddressClassBRe{"(12[8-9]|1[3-8]\\d|19[0-1])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){3}"};

    return !std::regex_match(ipAddress, classBNetworkOrBroadcastAddressRe) &&
           std::regex_match(ipAddress, ipAddressClassBRe);
}

bool RegexTests::_isClassCIpAddress(const std::string& ipAddress)
{
    // unlike class A/B (see above) the ip class regex for C excludes the network (e.g. 192.168.5.0) and broadcast (e.g. 194.178.2.255) addresses
    std::regex ipAddressClassCRe{"(19[2-9]|2[0-1]\\d|22[0-3])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){2}(\\.(25[0-4]|2[0-4]\\d|1\\d{2}|[1-9]\\d|[1-9]))"};

    return std::regex_match(ipAddress, ipAddressClassCRe);
}

bool RegexTests::_isClassDIpAddress(const std::string& ipAddress)
{
    // no network/broadcast address to be excluded for class D (not applicable)
    std::regex ipAddressClassDRe{"(22[4-9]|23\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){3}"};

    return std::regex_match(ipAddress, ipAddressClassDRe);
}

QTEST_APPLESS_MAIN(RegexTests)

#include "tst_regextests.moc"
