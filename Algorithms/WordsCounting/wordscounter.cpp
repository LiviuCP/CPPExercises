#include <cassert>
#include <cctype>
#include <set>
#include <sstream>

#include "wordscounter.h"

WordsCounter::WordsCounter(FileContent&& content)
{
    countWords(std::move(content));
}

void WordsCounter::countWords(FileContent&& content)
{
    m_Content = std::move(content);

    _replaceNonSpaceSeparatorsWithSpace();
    _buildOccurrencesMap();
}

const WordOccurrencesMap& WordsCounter::getOccurrencesMap() const
{
    return m_OccurrencesMap;
}

void WordsCounter::_replaceNonSpaceSeparatorsWithSpace()
{
    for (Line& line : m_Content)
    {
        // all characters that are not alphabetic, numeric or '-' / '\'' to be replaced by space (i.e. to be considered
        // word separators); space chars obviously should not be replaced
        std::replace_if(
            line.begin(), line.end(),
            [](char ch) {
                return !isalnum(ch) && !std::set{'-', '\'', ' '}.contains(ch);
            },
            ' ');
    }
}

void WordsCounter::_buildOccurrencesMap()
{
    for (const Line& line : m_Content)
    {
        std::istringstream istr{line};

        while (!istr.eof())
        {
            Word currentWord;
            istr >> currentWord;

            _trimPrefixAndSuffix(currentWord);

            if (currentWord.empty())
            {
                continue;
            }

            _convertToLowerCase(currentWord);

            if (m_OccurrencesMap.contains(currentWord))
            {
                ++m_OccurrencesMap[currentWord];
            }
            else
            {
                m_OccurrencesMap[currentWord] = 1;
            }
        }
    }
}

// removes all trailing and ending characters from a word that are neither alphabetic nor numeric (e.g. "_-myword\'"
// becomes "myword")
void WordsCounter::_trimPrefixAndSuffix(Word& word)
{
    const auto substrStartIt{std::find_if(word.cbegin(), word.cend(), [](char ch) { return isalnum(ch); })};
    const auto substrEndIt{std::find_if(word.crbegin(), word.crend(), [](char ch) { return isalnum(ch); })};

    if (substrStartIt != word.cend() && substrEndIt != word.crend())
    {
        const auto startIndex{std::distance(word.cbegin(), substrStartIt)};
        const auto endIndex{word.size() - std::distance(word.crbegin(), substrEndIt)};

        assert(startIndex < endIndex);

        // note: if the word contains '-' or '\'' within its content it is not being checked if these chars have valid
        // count and/or placement (e.g. "alpha--\'beta")
        word = word.substr(startIndex, endIndex - startIndex);
    }
    else
    {
        word.clear();
    }
}

void WordsCounter::_convertToLowerCase(Word& word)
{
    std::transform(word.cbegin(), word.cend(), word.begin(), [](char ch) { return tolower(ch); });
}
