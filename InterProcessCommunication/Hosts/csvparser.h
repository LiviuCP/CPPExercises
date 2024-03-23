#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <utility>

#include "datautils.h"

class CSVParser
{
public:
    CSVParser() = delete;
    explicit CSVParser(const std::string& csvInputFilePath);

    void parse();
    const std::vector<Data::HostNameAndInfo>& getOutput() const;

private:
    enum class ErrorCode : int8_t
    {
        EMPTY_ROW = -1,
        SUCCESS,
        INVALID_IP,
        INVALID_MAC,
        INVALID_HOSTNAME,
        ADDITIONAL_INFO,
        LESS_INFO,
        PARSING_LIMIT_EXCEEEDED
    };

    using Index_t = std::optional<size_t>;
    using ParsingError = std::pair<ErrorCode, size_t>;

    bool _readInput();
    void _parseInput();
    ErrorCode _parseRow(const std::string& row);
    void _logParsingErrorsToFile();

    static Index_t _readStringField(const std::string& source, std::string& destination, Index_t index, char separator = ',');
    static void _trimQuotesAndWhiteSpaceFromString(std::string& str);
    static bool _hasQuotes(const std::string_view str);

    static bool _isValidMacAddress(const std::string& macAddress);
    static bool _isValidHostName(const std::string& hostName);

    static std::string _retrieveIpClass(const std::string& ipAddress);
    static bool _isValidClassAIpAddress(const std::string& ipAddress);
    static bool _isValidClassBIpAddress(const std::string& ipAddress);
    static bool _isValidClassCIpAddress(const std::string& ipAddress);
    static bool _isValidClassDIpAddress(const std::string& ipAddress);

    std::string m_CSVFilePath;
    std::vector<std::string> m_Input;
    std::vector<ParsingError> m_ParsingErrors;
    std::vector<Data::HostNameAndInfo> m_Output;
    bool m_ParsingLimitExceeded;
};
