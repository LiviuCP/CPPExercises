#ifndef CONCRETEPARSERS_H
#define CONCRETEPARSERS_H

#include "parser.h"

class DigitsParser : virtual public Parser
{
public:
    DigitsParser(char* pFilePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class LowerCaseParser : virtual public Parser
{
public:
    LowerCaseParser(char* pFilePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class UpperCaseParser : virtual public Parser
{
public:
    UpperCaseParser(char* pFilePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class LowerUpperCaseParser : virtual public LowerCaseParser, virtual public UpperCaseParser
{
public:
    LowerUpperCaseParser(char* pFilePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

class AlphaAndDigitsParser final : public LowerUpperCaseParser, public DigitsParser
{
public:
    AlphaAndDigitsParser(char* pFilePath, IAggregator* pIAggregator);

protected:
    bool isValidChar(char c) override;
};

#endif // CONCRETEPARSERS_H
