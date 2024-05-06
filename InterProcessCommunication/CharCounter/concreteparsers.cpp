#include "concreteparsers.h"

DigitsParser::DigitsParser(char* pFilePath, IAggregator* pIAggregator)
    : Parser{pFilePath, pIAggregator}
{
}

bool DigitsParser::isValidChar(char c)
{
    return c >= 48 && c <= 57;
}

LowerCaseParser::LowerCaseParser(char* pFilePath, IAggregator* pIAggregator)
    : Parser{pFilePath, pIAggregator}
{
}

bool LowerCaseParser::isValidChar(char c)
{
    return c >= 97 && c <= 122;
}

UpperCaseParser::UpperCaseParser(char* pFilePath, IAggregator* pIAggregator)
    : Parser{pFilePath, pIAggregator}
{
}

bool UpperCaseParser::isValidChar(char c)
{
    return c >= 65 && c <= 90;
}

LowerUpperCaseParser::LowerUpperCaseParser(char* pFilePath, IAggregator* pIAggregator)
    : LowerCaseParser{pFilePath, pIAggregator}
    , UpperCaseParser{pFilePath, pIAggregator}
    , Parser{pFilePath, pIAggregator}
{
}

bool LowerUpperCaseParser::isValidChar(char c)
{
    return LowerCaseParser::isValidChar(c) || UpperCaseParser::isValidChar(c);
}

AlphaAndDigitsParser::AlphaAndDigitsParser(char* pFilePath, IAggregator* pIAggregator)
    : LowerUpperCaseParser{pFilePath, pIAggregator}
    , LowerCaseParser{pFilePath, pIAggregator}
    , UpperCaseParser{pFilePath, pIAggregator}
    , DigitsParser{pFilePath, pIAggregator}
    , Parser{pFilePath, pIAggregator}
{
}

bool AlphaAndDigitsParser::isValidChar(char c)
{
    return LowerUpperCaseParser::isValidChar(c) || DigitsParser::isValidChar(c);
}
