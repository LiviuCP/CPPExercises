#include "concreteparsers.h"

DigitsParser::DigitsParser(const std::string& filePath, IAggregator* pIAggregator)
    : Parser{filePath, pIAggregator}
{
}

bool DigitsParser::isValidChar(char c)
{
    return c >= 48 && c <= 57;
}

LowerCaseParser::LowerCaseParser(const std::string& filePath, IAggregator* pIAggregator)
    : Parser{filePath, pIAggregator}
{
}

bool LowerCaseParser::isValidChar(char c)
{
    return c >= 97 && c <= 122;
}

UpperCaseParser::UpperCaseParser(const std::string& filePath, IAggregator* pIAggregator)
    : Parser{filePath, pIAggregator}
{
}

bool UpperCaseParser::isValidChar(char c)
{
    return c >= 65 && c <= 90;
}

LowerUpperCaseParser::LowerUpperCaseParser(const std::string& filePath, IAggregator* pIAggregator)
    : LowerCaseParser{filePath, pIAggregator}
    , UpperCaseParser{filePath, pIAggregator}
    , Parser{filePath, pIAggregator}
{
}

bool LowerUpperCaseParser::isValidChar(char c)
{
    return LowerCaseParser::isValidChar(c) || UpperCaseParser::isValidChar(c);
}

AlphaAndDigitsParser::AlphaAndDigitsParser(const std::string& filePath, IAggregator* pIAggregator)
    : LowerUpperCaseParser{filePath, pIAggregator}
    , LowerCaseParser{filePath, pIAggregator}
    , UpperCaseParser{filePath, pIAggregator}
    , DigitsParser{filePath, pIAggregator}
    , Parser{filePath, pIAggregator}
{
}

bool AlphaAndDigitsParser::isValidChar(char c)
{
    return LowerUpperCaseParser::isValidChar(c) || DigitsParser::isValidChar(c);
}
