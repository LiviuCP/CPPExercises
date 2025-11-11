#pragma once

#include <string>
#include <vector>
#include <map>

using Line = std::string;
using Word = std::string;
using Occurrences = size_t;
using FileContent = std::vector<std::string>;
using WordOccurrencesMap = std::map<Word, Occurrences>;

class WordsCounter
{
public:
    WordsCounter() = default;
    WordsCounter(FileContent&& content);

    void countWords(FileContent&& content);
    const WordOccurrencesMap& getOccurrencesMap() const;

private:
    void _replaceNonSpaceSeparatorsWithSpace();
    void _buildOccurrencesMap();

    static void _trimPrefixAndSuffix(Word& word);
    static void _convertToLowerCase(Word& word);

    FileContent m_Content;
    WordOccurrencesMap m_OccurrencesMap;
};
