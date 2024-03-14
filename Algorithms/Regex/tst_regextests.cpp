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
    void testHostName();

    void testIpAddressClasses_data();
    void testHostName_data();

private:
    IpClass _parseIpAddress(const std::string& ipAddress);

    bool _isClassAIpAddress(const std::string& ipAddress);
    bool _isClassBIpAddress(const std::string& ipAddress);
    bool _isClassCIpAddress(const std::string& ipAddress);
    bool _isClassDIpAddress(const std::string& ipAddress);

    bool _isValidHostName(const std::string& hostName);
};

void RegexTests::testIpAddressClasses()
{
    QFETCH(std::string, ipAddress);
    QFETCH(IpClass, ipClass);

    QVERIFY(ipClass == _parseIpAddress(ipAddress));
}

void RegexTests::testHostName()
{
    QFETCH(std::string, hostName);
    QFETCH(bool, shouldBeValidHostName);

    QVERIFY(shouldBeValidHostName == _isValidHostName(hostName));
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

void RegexTests::testHostName_data()
{
    QTest::addColumn<std::string>("hostName");
    QTest::addColumn<bool>("shouldBeValidHostName");

    QTest::newRow("1_Valid") << std::string{"my_host14"} << true;
    QTest::newRow("2_Valid") << std::string{"my_multicast_a"} << true;
    QTest::newRow("3_Valid") << std::string{"my_host_2024"} << true;
    QTest::newRow("4_Valid") << std::string{"my_host_2"} << true;
    QTest::newRow("5_Valid") << std::string{"My_Wonder_Host_is_here_1"} << true;
    QTest::newRow("6_Valid") << std::string{"Another_Host"} << true;
    QTest::newRow("7_Valid") << std::string{"Ahostnameisahostname8765"} << true;
    QTest::newRow("8_Valid") << std::string{"another_Host"} << true;
    QTest::newRow("9_Invalid") << std::string{" "} << false;
    QTest::newRow("10_Invalid") << std::string{"_my_host41"} << false;
    QTest::newRow("11_Invalid") << std::string{"my_host41_"} << false;
    QTest::newRow("12_Invalid") << std::string{"my__host41"} << false;
    QTest::newRow("13_Invalid") << std::string{"my host41"} << false;
    QTest::newRow("14_Invalid") << std::string{"my+host41"} << false;
    QTest::newRow("15_Invalid") << std::string{"my2host41"} << false;
    QTest::newRow("16_Invalid") << std::string{"my_host41234"} << false;
    QTest::newRow("17_Invalid") << std::string{"my_host41a"} << false;
    QTest::newRow("18_Invalid") << std::string{"41my_host"} << false;
    QTest::newRow("19_Invalid") << std::string{"my_hostabcdefghijklmnop41"} << false;
    QTest::newRow("20_Invalid") << std::string{"My_Beautiful_HostIsHere1"} << false;
    QTest::newRow("21_Valid") << std::string{"myhost412"} << true;
    QTest::newRow("22_Invalid") << std::string{"myHost412"} << false;
    QTest::newRow("23_Valid") << std::string{"my_Host412"} << true;
    QTest::newRow("24_Invalid") << std::string{"my_HoSt412"} << false;
    QTest::newRow("25_Valid") << std::string{"host_2240"} << true;
    QTest::newRow("26_Invalid") << std::string{"host_22405"} << false;
    QTest::newRow("27_Valid") << std::string{"h_O_s_T_n_A_m_E_1"} << true;
    QTest::newRow("28_Invalid") << std::string{"h_O_sT_n_A_m_E_1"} << false;
    QTest::newRow("29_Invalid") << std::string{""} << false;
    QTest::newRow("30_Invalid") << std::string{"AhostnameisAhostname8765"} << false;
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

/* The host name should contain only letters, digits and underscore (max. 24 characters in total):
    - digits are optional but should be at the end and should not be more than 4
    - maximum 2 underscore characters (both optional)
*/
bool RegexTests::_isValidHostName(const std::string& hostName)
{
    bool isValid{false};

    if (const size_t c_MaxCharsCount{24}; !hostName.empty() && hostName.size() <= c_MaxCharsCount)
    {
        std::regex hostNameRe{"([a-zA-Z]{1}[a-z]*)(_[a-zA-Z]{1}[a-z]*)*(_?\\d{1,4})?"};
        isValid = std::regex_match(hostName, hostNameRe);
    }

    return isValid;
}

QTEST_APPLESS_MAIN(RegexTests)

#include "tst_regextests.moc"
