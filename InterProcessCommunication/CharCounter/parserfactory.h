#ifndef PARSERFACTORY_H
#define PARSERFACTORY_H

#include <string>

class Parser;
class IAggregator;

class ParserFactory
{
public:
    enum class ParserType : unsigned char
    {
        DIGITS = 0,
        LOWERCASE,
        UPPERCASE,
        LOWER_UPPER_CASE,
        ALPHA_AND_DIGITS
    };

    ParserFactory() = delete;
    static Parser* createParser(ParserType parserType, const std::string& filePath, IAggregator* pIAggregator);
};

#endif
