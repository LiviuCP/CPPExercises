#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cassert>
#include <map>
#include <regex>

#include "apputils.h"
#include "csvparser.h"

static constexpr size_t c_MaxRowsToParse{100};
static constexpr size_t c_MacAddressSize{12};
static constexpr size_t c_IpV4AddressFieldsCount{4};
static constexpr size_t c_IpFieldMaxCharsCount{3};
static constexpr char c_Quotes{'\"'};

CSVParser::CSVParser(const std::filesystem::path& csvInputFilePath)
    : m_CSVFilePath{csvInputFilePath}
    , m_ParsingLimitExceeded{false}
{
    assert(Utils::isCsvFilePath(m_CSVFilePath));
}

void CSVParser::parse()
{
    m_ParsingLimitExceeded = false;
    bool success{false};

    try
    {
        success = _readInput();
    }
    catch (std::ios_base::failure&)
    {
        std::cerr << "An error occurred when handling input CSV file: " << std::filesystem::canonical(m_CSVFilePath).string() << "\n";
        success = false;
    }

    if (success)
    {
        _parseInput();
    }
}

const std::vector<Data::HostNameAndInfo>& CSVParser::getOutput() const
{
    return m_Output;
}

bool CSVParser::_readInput()
{
    bool success{false};
    std::ifstream in{m_CSVFilePath};

    if (in.is_open())
    {
        success = true;
        size_t rowsCount{0};

        while (!in.eof())
        {
            if (c_MaxRowsToParse == rowsCount)
            {
                m_ParsingLimitExceeded = true;
                break;
            }

            std::string line;
            getline(in, line);
            m_Input.push_back(line);
            ++rowsCount;
        }
    }

    return success;
}

void CSVParser::_parseInput()
{
    for (auto it{m_Input.cbegin()}; it != m_Input.cend(); ++it)
    {
        const ErrorCode result{_parseRow(*it)};

        if (ErrorCode::SUCCESS != result)
        {
            const size_t c_RowNr{1 + static_cast<size_t>(std::distance(m_Input.cbegin(), it))};
            m_ParsingErrors.emplace_back(result, c_RowNr);
        }
    }

    _logParsingErrorsToFile();
}

CSVParser::ErrorCode CSVParser::_parseRow(const std::string& row)
{
    std::string ipAddress, macAddress, hostName, ipClass;
    Index_t currentPosition;
    ErrorCode result{!row.empty() ? ErrorCode::SUCCESS : ErrorCode::EMPTY_ROW};

    do
    {
        if (ErrorCode::SUCCESS != result) break;

        currentPosition = _readStringField(row, hostName, 0);
        result = !currentPosition.has_value() ? ErrorCode::LESS_INFO : !_hasQuotes(hostName) ? ErrorCode::INVALID_HOSTNAME : ErrorCode::SUCCESS;

        if (ErrorCode::SUCCESS != result) break;

        currentPosition = _readStringField(row, macAddress, currentPosition);
        result = !currentPosition.has_value() ? ErrorCode::LESS_INFO : !_hasQuotes(macAddress) ? ErrorCode::INVALID_MAC : ErrorCode::SUCCESS;

        if (ErrorCode::SUCCESS != result) break;

        currentPosition = _readStringField(row, ipAddress, currentPosition);
        result = currentPosition.has_value() ? ErrorCode::ADDITIONAL_INFO : !_hasQuotes(ipAddress) ? ErrorCode::INVALID_IP : ErrorCode::SUCCESS;

        if (ErrorCode::SUCCESS != result) break;

        _trimQuotesAndWhiteSpaceFromString(hostName);
        result = !_isValidHostName(hostName) ? ErrorCode::INVALID_HOSTNAME : ErrorCode::SUCCESS;

        if (ErrorCode::SUCCESS != result) break;

        _trimQuotesAndWhiteSpaceFromString(macAddress);
        result = !_isValidMacAddress(macAddress) ? ErrorCode::INVALID_MAC : ErrorCode::SUCCESS;

        if (ErrorCode::SUCCESS != result) break;

        _trimQuotesAndWhiteSpaceFromString(ipAddress);
        ipClass = _retrieveIpClass(ipAddress);
        result = ipClass.empty() ? ErrorCode::INVALID_IP : ErrorCode::SUCCESS;
    }
    while (false);

    if (ErrorCode::SUCCESS == result)
    {
        std::transform(hostName.cbegin(), hostName.cend(), hostName.begin(), [](unsigned char c){return std::tolower(c);});
        std::transform(macAddress.cbegin(), macAddress.cend(), macAddress.begin(), [](unsigned char c){return std::toupper(c);});

        hostName = c_Quotes + hostName + c_Quotes;
        macAddress = c_Quotes + macAddress + c_Quotes;
        ipAddress = c_Quotes + ipAddress + c_Quotes;
        ipClass = c_Quotes + ipClass + c_Quotes;

        m_Output.push_back({hostName, {macAddress, ipAddress, ipClass}});
    }

    return result;
}

void CSVParser::_logParsingErrorsToFile()
{
    if (m_Input.empty())
    {
        m_ParsingErrors.clear();
    }

    // exclude last row if empty
    if (const size_t c_ErrorsCount{m_ParsingErrors.size()}; c_ErrorsCount > 0)
    {
        const size_t c_LastInputRowNumber{m_Input.size()}; // row numbering starts at 1

        if (ParsingError{ErrorCode::EMPTY_ROW, c_LastInputRowNumber} == m_ParsingErrors[c_ErrorsCount - 1])
        {
            m_ParsingErrors.pop_back();
        }
    }

    if (m_ParsingLimitExceeded)
    {
        m_ParsingErrors.emplace_back(ErrorCode::PARSING_LIMIT_EXCEEEDED, c_MaxRowsToParse + 1);
    }

    if (!m_ParsingErrors.empty())
    {
        const std::map<ErrorCode, std::string> c_ErrorsMap {
            {ErrorCode::EMPTY_ROW, "The row is empty!"},
            {ErrorCode::INVALID_IP, "The IP address is invalid!"},
            {ErrorCode::INVALID_MAC, "The Mac address is invalid!"},
            {ErrorCode::INVALID_HOSTNAME, "The hostname is invalid!"},
            {ErrorCode::ADDITIONAL_INFO, "More CSV columns than required have been provided!"},
            {ErrorCode::LESS_INFO, "Less CSV columns than required have been provided!"},
            {ErrorCode::PARSING_LIMIT_EXCEEEDED, "The maximum allowed number of rows to parse has been exceeded! Extra rows have been ignored."},
            {ErrorCode::SUCCESS, ""}
        };

        std::filesystem::path errorsFilePath{m_CSVFilePath.parent_path()};
        errorsFilePath /= m_CSVFilePath.stem();
        errorsFilePath += "_error.txt";

        bool success{false};

        try
        {
            std::ofstream errStream{errorsFilePath};
            success = errStream.is_open();

            if (success)
            {
                for (const auto&[errCode, rowNumber] : m_ParsingErrors)
                {
                    errStream << "Row number: " << rowNumber << " " << c_ErrorsMap.at(errCode) << "\n";
                }
            }
        }
        catch (std::ios_base::failure&)
        {
            success = false;
        }

        if (!success)
        {
            std::cerr << "An error occurred when writing to error file: " << std::filesystem::canonical(errorsFilePath).string() << "\n";
        }
    }
}

CSVParser::Index_t CSVParser::_readStringField(const std::string& source, std::string& destination, Index_t index, char separator)
{
    Index_t nextIndex;

    if(const size_t c_Length{source.size()};
        c_Length > 0u && index.has_value() && index < c_Length)
    {
        size_t currentIndex{index.value()};

        // the field separator should not be taken into account unless it's the first character in the string (resulting in an empty starting field)
        if (0 != currentIndex && separator == source[currentIndex])
        {
            ++currentIndex;
        }

        destination = source.substr(currentIndex);
        std::smatch fieldMatch;
        std::regex separatorRegex{"\\" + std::string{separator}};

        if (const bool matchFound{std::regex_search(destination, fieldMatch, separatorRegex)}; matchFound)
        {
            destination = fieldMatch.prefix();
            nextIndex = currentIndex + destination.size();
        }
    }

    return nextIndex;
}

void CSVParser::_trimQuotesAndWhiteSpaceFromString(std::string& str)
{
    if (!str.empty())
    {
        // left trim
        auto isNotSpaceOrQuote{[](unsigned char c){return !std::isspace(c) && '\"' != c;}};
        auto leftTrimIt{std::find_if(str.begin(), str.end(), isNotSpaceOrQuote)};
        str.erase(str.begin(), leftTrimIt);

        // right trim is a bit trickier as apparently string erase doesn't work well with reverse iterators (so last character should be checked before applying reverse so costs are minimized)
        if (!str.empty() && !isNotSpaceOrQuote(str[str.length() - 1]))
        {
            std::reverse(str.begin(), str.end());
            auto rightTrimIt{std::find_if(str.begin() + 1, str.end(), isNotSpaceOrQuote)};
            str.erase(str.begin(), rightTrimIt);
            std::reverse(str.begin(), str.end());
        }
    }
}

bool CSVParser::_hasQuotes(const std::string_view str)
{
    return str.starts_with('\"') && str.ends_with('\"');
}

bool CSVParser::_isValidMacAddress(const std::string& macAddress)
{
    std::regex nullAddressRe{"0{12}"};
    std::regex macAddressRe{"[\\da-fA-F]{12}"};

    return !std::regex_match(macAddress, nullAddressRe) &&
           std::regex_match(macAddress, macAddressRe);
}

bool CSVParser::_isValidHostName(const std::string& hostName)
{
    bool isValid{false};

    if (!hostName.empty() && hostName.size() < 25)
    {
        std::regex hostNameRe{"([a-zA-Z]+)(_[a-zA-Z]+)*(_?\\d{1,4})?"};
        isValid = std::regex_match(hostName, hostNameRe);
    }

    return isValid;
}

std::string CSVParser::_retrieveIpClass(const std::string& ipAddress)
{
    std::string ipAddressClass;

    if (_isValidClassAIpAddress(ipAddress))
    {
        ipAddressClass = "A";
    }
    else if (_isValidClassBIpAddress(ipAddress))
    {
        ipAddressClass = "B";
    }
    else if (_isValidClassCIpAddress(ipAddress))
    {
        ipAddressClass = "C";
    }
    else if (_isValidClassDIpAddress(ipAddress))
    {
        ipAddressClass = "D";
    }

    return ipAddressClass;
}

bool CSVParser::_isValidClassAIpAddress(const std::string& ipAddress)
{
    // network (e.g. 10.0.0.0) and broadcast (e.g. 12.255.255.255) addresses to be excluded
    std::regex classANetworkOrBroadcastAddressRe{"(12[0-6]|1[0-1][0-9]|[1-9][0-9]|[1-9])((\\.255){3}|(\\.0){3})"};

    std::regex ipAddressClassARe{"(12[0-6]|1[0-1]\\d|[1-9]\\d|[1-9])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){3}"};

    return !std::regex_match(ipAddress, classANetworkOrBroadcastAddressRe) &&
           std::regex_match(ipAddress, ipAddressClassARe);
}

bool CSVParser::_isValidClassBIpAddress(const std::string& ipAddress)
{
    // network (e.g. 190.2.0.0) and broadcast (e.g. 191.4.255.255) addresses to be excluded
    std::regex classBNetworkOrBroadcastAddressRe{"(12[8-9]|1[3-8]\\d|19[0-1])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d))((\\.255){2}|(\\.0){2})"};
    std::regex ipAddressClassBRe{"(12[8-9]|1[3-8]\\d|19[0-1])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){3}"};

    return !std::regex_match(ipAddress, classBNetworkOrBroadcastAddressRe) &&
           std::regex_match(ipAddress, ipAddressClassBRe);
}

bool CSVParser::_isValidClassCIpAddress(const std::string& ipAddress)
{
    // unlike class A/B (see above) the ip class regex for C excludes the network (e.g. 192.168.5.0) and broadcast (e.g. 194.178.2.255) addresses
    std::regex ipAddressClassCRe{"(19[2-9]|2[0-1]\\d|22[0-3])(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){2}(\\.(25[0-4]|2[0-4]\\d|1\\d{2}|[1-9]\\d|[1-9]))"};
    return std::regex_match(ipAddress, ipAddressClassCRe);
}

bool CSVParser::_isValidClassDIpAddress(const std::string& ipAddress)
{
    // no network/broadcast address to be excluded for class D (not applicable)
    std::regex ipAddressClassDRe{"(22[4-9]|23\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d{2}|[1-9]\\d|\\d)){3}"};
    return std::regex_match(ipAddress, ipAddressClassDRe);
}
