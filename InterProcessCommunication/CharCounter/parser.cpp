#include <algorithm>

#include "iaggregator.h"
#include "parser.h"

static constexpr size_t c_CharCountThreshold{16 * 1024};

Parser::Parser(char* pFilePath, IAggregator* pIAggregator)
    : m_pFile{nullptr}
    , m_pFilePath{pFilePath}
    , m_pIAggregator{pIAggregator}
    , m_TotalFoundCharsCount{0}
    , m_TotalParsedCharsCount{0}
    , m_MaxCharsCountExceeded{false}
{
    std::fill(m_CharOccurrences.begin(), m_CharOccurrences.end(), 0);

    if (m_pFilePath)
    {
        m_pFile = fopen(m_pFilePath, "r");
    }
}

Parser::~Parser()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
}

void Parser::parse()
{
    if (m_pFile)
    {
        while(!feof(m_pFile)) {
            if (c_CharCountThreshold == m_TotalParsedCharsCount)
            {
                m_MaxCharsCountExceeded = true;
                break;
            }

            const char c{static_cast<char>(fgetc(m_pFile))};

            if (c >= 0 && isValidChar(c)) {

                m_TotalFoundCharsCount = m_TotalFoundCharsCount + 1;
                ++m_CharOccurrences[c];
            }

            ++m_TotalParsedCharsCount;
        }

        if (m_pIAggregator)
        {
            m_pIAggregator->aggregate(m_CharOccurrences);
        }
    }
}

size_t Parser::getTotalFoundCharsCount()
{
    return m_TotalFoundCharsCount;
}

size_t Parser::getTotalParsedCharsCount()
{
    return m_TotalParsedCharsCount;
}

bool Parser::maxCharsExceeeded() const
{
    return m_MaxCharsCountExceeded;
}

char* Parser::getFilePath() const
{
    return m_pFilePath;
}
