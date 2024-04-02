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
    void testEmail();

    void testIpAddressClasses_data();
    void testHostName_data();
    void testEmail_data();

private:
    IpClass _parseIpAddress(const std::string& ipAddress);

    bool _isClassAIpAddress(const std::string& ipAddress);
    bool _isClassBIpAddress(const std::string& ipAddress);
    bool _isClassCIpAddress(const std::string& ipAddress);
    bool _isClassDIpAddress(const std::string& ipAddress);

    bool _isValidHostName(const std::string& hostName);
    bool _isValidEmail(const std::string& email);
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

void RegexTests::testEmail()
{
    QFETCH(std::string, email);
    QFETCH(bool, shouldBeValidEmail);

    QVERIFY(shouldBeValidEmail == _isValidEmail(email));
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

void RegexTests::testEmail_data()
{
    QTest::addColumn<std::string>("email");
    QTest::addColumn<bool>("shouldBeValidEmail");

    QTest::newRow("1a_Valid") << std::string{"johnsmith@gmail.com"} << true;
    QTest::newRow("1b_Valid") << std::string{"john.smith@gmail.com"} << true;
    QTest::newRow("1c_Valid") << std::string{"john_smith@gmail.com"} << true;
    QTest::newRow("1d_Valid") << std::string{"john-smith@gmail.com"} << true;
    QTest::newRow("1e_Valid") << std::string{"john-jack.smith@gmail.com"} << true;
    QTest::newRow("1f_Valid") << std::string{"john_jack.smith@gmail.com"} << true;
    QTest::newRow("2a_Valid") << std::string{"JohnSmith@gmail.com"} << true;
    QTest::newRow("2b_Valid") << std::string{"John.Smith@gmail.com"} << true;
    QTest::newRow("2c_Valid") << std::string{"John_smIth@gmail.com"} << true;
    QTest::newRow("2d_Valid") << std::string{"jOhn-Smith@gmail.com"} << true;
    QTest::newRow("2e_Valid") << std::string{"John-jack.smith@gmail.com"} << true;
    QTest::newRow("2f_Valid") << std::string{"john_Jack.smith@gmail.com"} << true;
    QTest::newRow("3a_Valid") << std::string{"johnsmith@gmail.co.uk"} << true;
    QTest::newRow("3b_Valid") << std::string{"john.smith@gmail.co.uk"} << true;
    QTest::newRow("3c_Valid") << std::string{"john_smith@gmail.co.uk"} << true;
    QTest::newRow("3d_Valid") << std::string{"john-smith@gmail.co.uk"} << true;
    QTest::newRow("3e_Valid") << std::string{"john-jack.smith@gmail.co.uk"} << true;
    QTest::newRow("3f_Valid") << std::string{"john_jack.smith@gmail.co.uk"} << true;
    QTest::newRow("4a_Valid") << std::string{"johnsmith1982@gmail.com"} << true;
    QTest::newRow("4b_Valid") << std::string{"john.smith1982@gmail.com"} << true;
    QTest::newRow("4c_Valid") << std::string{"john_smith1982@gmail.com"} << true;
    QTest::newRow("4d_Valid") << std::string{"john-smith1982@gmail.com"} << true;
    QTest::newRow("4e_Valid") << std::string{"john-jack.smith1982@gmail.com"} << true;
    QTest::newRow("4f_Valid") << std::string{"john_jack.smith1982@gmail.com"} << true;
    QTest::newRow("5a_Valid") << std::string{"johnsmith19820101@gmail.com"} << true;
    QTest::newRow("5b_Valid") << std::string{"john.smith19820101@gmail.com"} << true;
    QTest::newRow("5c_Valid") << std::string{"john_smith19820101@gmail.com"} << true;
    QTest::newRow("5d_Valid") << std::string{"john-smith19820101@gmail.com"} << true;
    QTest::newRow("5e_Valid") << std::string{"john-jack.smith19820101@gmail.com"} << true;
    QTest::newRow("5f_Valid") << std::string{"john_jack.smith19820101@gmail.com"} << true;
    QTest::newRow("6a_Valid") << std::string{"j@g.com"} << true;
    QTest::newRow("6b_Valid") << std::string{"j.s@g.com"} << true;
    QTest::newRow("6c_Valid") << std::string{"j-j@g.com"} << true;
    QTest::newRow("6d_Valid") << std::string{"j_j@g.com"} << true;
    QTest::newRow("6e_Valid") << std::string{"j-j.s@g.com"} << true;
    QTest::newRow("6f_Valid") << std::string{"j_j.s@g.com"} << true;
    QTest::newRow("1a_Invalid") << std::string{"john.smith_@gmail.com"} << false;
    QTest::newRow("1b_Invalid") << std::string{"john.smith-@gmail.com"} << false;
    QTest::newRow("1c_Invalid") << std::string{"john.jack_smith@gmail.com"} << false;
    QTest::newRow("1d_Invalid") << std::string{"john.jack-smith@gmail.com"} << false;
    QTest::newRow("1e_Invalid") << std::string{"john..smith@gmail.com"} << false;
    QTest::newRow("1f_Invalid") << std::string{"john-.smith@gmail.com"} << false;
    QTest::newRow("1g_Invalid") << std::string{"john--smith@gmail.com"} << false;
    QTest::newRow("1h_Invalid") << std::string{"john__smith@gmail.com"} << false;
    QTest::newRow("1i_Invalid") << std::string{"john_jack_smith@gmail.com"} << false;
    QTest::newRow("1j_Invalid") << std::string{"john-jack_smith@gmail.com"} << false;
    QTest::newRow("1k_Invalid") << std::string{"john_jack-smith@gmail.com"} << false;
    QTest::newRow("1l_Invalid") << std::string{"john-jack-smith@gmail.com"} << false;
    QTest::newRow("1m_Invalid") << std::string{"johnsmith.@gmail.com"} << false;
    QTest::newRow("1n_Invalid") << std::string{"john/smith@gmail.com"} << false;
    QTest::newRow("1o_Invalid") << std::string{"_john.smith@gmail.com"} << false;
    QTest::newRow("1p_Invalid") << std::string{"jo1hn_smith@gmail.com"} << false;
    QTest::newRow("1q_Invalid") << std::string{"john+smith@gmail.com"} << false;
    QTest::newRow("1r_Invalid") << std::string{"john-jack_son.smith@gmail.com"} << false;
    QTest::newRow("1s_Invalid") << std::string{"john1_jack.smith@gmail.com"} << false;
    QTest::newRow("1t_Invalid") << std::string{"john_jack1.smith@gmail.com"} << false;
    QTest::newRow("2a_Invalid") << std::string{"johnsmith123456789@gmail.com"} << false;
    QTest::newRow("2b_Invalid") << std::string{"john.smith123456789@gmail.com"} << false;
    QTest::newRow("2c_Invalid") << std::string{"john-jack.smith123456789@gmail.com"} << false;
    QTest::newRow("2d_Invalid") << std::string{"john_jack.smith123456789@gmail.com"} << false;
    QTest::newRow("2e_Invalid") << std::string{"john_jack123456789@gmail.com"} << false;
    QTest::newRow("2f_Invalid") << std::string{"john-jack123456789@gmail.com"} << false;
    QTest::newRow("3a_Invalid") << std::string{"johnsmith@gmail.comm.uk"} << false;
    QTest::newRow("3b_Invalid") << std::string{"john.smith@gmail.co.ukmm"} << false;
    QTest::newRow("3c_Invalid") << std::string{"john_smith@gmail.co.uk.com"} << false;
    QTest::newRow("3d_Invalid") << std::string{"john-smith@gmail.co.uk."} << false;
    QTest::newRow("3e_Invalid") << std::string{"john-jack.smith@gmail.co.uk_"} << false;
    QTest::newRow("3f_Invalid") << std::string{"john_jack.smith@gmail.co..uk"} << false;
    QTest::newRow("3g_Invalid") << std::string{"johnsmith@gmail.co.uk-"} << false;
    QTest::newRow("3h_Invalid") << std::string{"john.smith@gmail.co.-uk"} << false;
    QTest::newRow("3i_Invalid") << std::string{"john_smith@gmail.comm"} << false;
    QTest::newRow("3j_Invalid") << std::string{"john-smith@gmail.co_uk"} << false;
    QTest::newRow("3k_Invalid") << std::string{"john-jack.smith@gmail.co.uk1"} << false;
    QTest::newRow("3l_Invalid") << std::string{"john_jack.smith@gmail..com"} << false;
    QTest::newRow("3m_Invalid") << std::string{"johnsmith@gmail.12"} << false;
    QTest::newRow("3n_Invalid") << std::string{"john.smith@gmail.co1.uk"} << false;
    QTest::newRow("3o_Invalid") << std::string{"john_smith@gmail2.com"} << false;
    QTest::newRow("3p_Invalid") << std::string{"john-smith@gma_il.com"} << false;
    QTest::newRow("3q_Invalid") << std::string{"john-jack.smith@1gmail.com"} << false;
    QTest::newRow("3r_Invalid") << std::string{"john_jack.smith@gmail"} << false;
    QTest::newRow("3s_Invalid") << std::string{"johnsmith@gmail."} << false;
    QTest::newRow("3t_Invalid") << std::string{"john.smith@gmail.c"} << false;
    QTest::newRow("3u_Invalid") << std::string{"john_smith@gmail.co.u"} << false;
    QTest::newRow("3v_Invalid") << std::string{"john-smith@gmail.c.uk"} << false;
    QTest::newRow("3x_Invalid") << std::string{"john-jack.smith@gmail.co."} << false;
    QTest::newRow("3y_Invalid") << std::string{"john_jack.smith@gmail.cou.k"} << false;
    QTest::newRow("4a_Invalid") << std::string{"JohnSmithgmail.com"} << false;
    QTest::newRow("4b_Invalid") << std::string{"John.Smith@"} << false;
    QTest::newRow("4c_Invalid") << std::string{"John_smIth@.com"} << false;
    QTest::newRow("4d_Invalid") << std::string{"jOhn@Smith@gmail.com"} << false;
    QTest::newRow("4e_Invalid") << std::string{"John-jack.smith@@gmail.com"} << false;
    QTest::newRow("4f_Invalid") << std::string{"@gmail.com"} << false;
    QTest::newRow("4g_Invalid") << std::string{"john.smith"} << false;
    QTest::newRow("4h_Invalid") << std::string{"john"} << false;
    QTest::newRow("4i_Invalid") << std::string{"john-jack"} << false;
    QTest::newRow("4j_Invalid") << std::string{"john_jack"} << false;
    QTest::newRow("4k_Invalid") << std::string{"john_jack.smith"} << false;
    QTest::newRow("4l_Invalid") << std::string{"john-jack.smith"} << false;
    QTest::newRow("5a_Invalid") << std::string{""} << false;
    QTest::newRow("5b_Invalid") << std::string{" "} << false;
    QTest::newRow("5c_Invalid") << std::string{"@"} << false;
    QTest::newRow("5d_Invalid") << std::string{"_"} << false;
    QTest::newRow("5e_Invalid") << std::string{"-"} << false;
    QTest::newRow("5f_Invalid") << std::string{"."} << false;
    QTest::newRow("5g_Invalid") << std::string{"+"} << false;
    QTest::newRow("5g_Invalid") << std::string{"1"} << false;
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

bool RegexTests::_isValidEmail(const std::string& email)
{
    std::regex emailRe{"[a-z]+([_-][a-z]+)?(\\.[a-z]+)?\\d{0,8}@[a-z]+(\\.[a-z]{2,3}){1,2}", std::regex_constants::ECMAScript | std::regex_constants::icase};
    return std::regex_match(email, emailRe);
}

QTEST_APPLESS_MAIN(RegexTests)

#include "tst_regextests.moc"
