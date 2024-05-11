#ifndef PARSINGENGINE_H
#define PARSINGENGINE_H

#include "utilities.h"

class Parser;
class IAggregator;

class ParsingEngine
{
public:
    ParsingEngine(const std::string& parsingOption, const FilePathsArray& filePaths, const std::string& aggregationOption);
    ~ParsingEngine();

    void run();

    size_t getTotalParsedDigitsCount() const;
    size_t getTotalMatchingDigitsCount() const;
    const CharOccurrencesArray& getCharOccurrences() const;

private:
    void _buildAggregator(const std::string& aggregationOption);
    void _buildParsers(const std::string& parsingOption, const FilePathsArray& filePaths);
    void _computeStatistics();

    std::vector<Parser*> m_Parsers;
    IAggregator* m_pIAggregator;
    size_t m_TotalParsedDigitsCount;
    size_t m_TotalMatchingDigitsCount;
    CharOccurrencesArray m_CharOccurrences;
};

#endif // PARSINGENGINE_H
