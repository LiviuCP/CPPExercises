#ifndef CONCRETEPARSERS_H
#define CONCRETEPARSERS_H

#include "parser.h"

class DigitsParser : virtual public Parser
{
public:
    DigitsParser(const std::string& filePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class LowerCaseParser : virtual public Parser
{
public:
    LowerCaseParser(const std::string& filePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class UpperCaseParser : virtual public Parser
{
public:
    UpperCaseParser(const std::string& filePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class LowerUpperCaseParser : virtual public LowerCaseParser, virtual public UpperCaseParser
{
public:
    LowerUpperCaseParser(const std::string& filePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class AlphaAndDigitsParser final : public LowerUpperCaseParser, public DigitsParser
{
public:
    AlphaAndDigitsParser(const std::string& filePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

#endif // CONCRETEPARSERS_H
