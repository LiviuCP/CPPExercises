#ifndef PARSER_H
#define PARSER_H

#include <cstdio>

#include "utilities.h"

class IAggregator;

class Parser
{
public:
    Parser(char* pFilePath, IAggregator* pIAggregator);
    virtual ~Parser();

    void parse();
    size_t getTotalFoundCharsCount();
    size_t getTotalParsedCharsCount();
    bool maxCharsExceeeded() const;
    char* getFilePath() const;

protected:
    virtual bool isValidChar(char c) = 0;

private:
    FILE * m_pFile;
    char* m_pFilePath;
    IAggregator* m_pIAggregator;
    CharOccurrencesArray m_CharOccurrences; // only the non-negative chars are taken into consideration
    size_t m_TotalFoundCharsCount;
    size_t m_TotalParsedCharsCount;
    bool m_MaxCharsCountExceeded;
};

#endif
