#pragma once

#include <cstdio>

#include "utilities.h"

class IAggregator;

class Parser
{
public:
    Parser(const std::string& filePath, IAggregator* pIAggregator);
    virtual ~Parser();

    void parse();
    size_t getTotalFoundCharsCount();
    size_t getTotalParsedCharsCount();
    bool maxCharsExceeeded() const;
    const std::string& getFilePath() const;

protected:
    virtual bool isValidChar(char c) = 0;

private:
    FILE* m_pFile;
    std::string m_FilePath;
    IAggregator* m_pIAggregator;
    CharOccurrencesArray m_CharOccurrences; // only the non-negative chars are taken into consideration
    size_t m_TotalFoundCharsCount;
    size_t m_TotalParsedCharsCount;
    bool m_MaxCharsCountExceeded;
};
