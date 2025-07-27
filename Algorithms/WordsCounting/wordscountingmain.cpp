#include <iostream>
#include <fstream>

#include "wordscounter.h"
#include "utils.h"

static const std::string c_InFile{Utilities::c_InputOutputDir + "wordscountinginput.txt"};
static const std::string c_OutFile{Utilities::c_InputOutputDir + "wordscountingoutput.txt"};

FileContent readInputFileContent(std::ifstream& in)
{
    FileContent content;

    if (in.is_open())
    {
        while(!in.eof())
        {
            Line currentLine;
            getline(in, currentLine);

            if (!currentLine.empty())
            {
                content.push_back(currentLine);
            }
        }
    }

    return content;
}

void writeOutputFileContent(const WordOccurrencesMap& occurrencesMap, std::ofstream& out)
{
    if (out.is_open())
    {
        out << "File: " << c_InFile << "\n\n";
        out << "Words / occurrences found:\n\n";

        size_t totalWordsCount{0};

        for (const auto&[word, count] : occurrencesMap)
        {
            totalWordsCount += count;
            out << "\"" << word << "\" / " << count << "\n";
        }

        if (!occurrencesMap.empty())
        {
            out << "\nTotal occurrences: " << totalWordsCount << "\n";
        }
        else
        {
            out << "No words found!\n";
        }
    }
}

int main()
{
    int result{0};

    std::ifstream in{c_InFile};
    std::ofstream out{c_OutFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        FileContent content{readInputFileContent(in)};
        WordsCounter wordsCounter(std::move(content));

        const WordOccurrencesMap& c_OccurrencesMap{wordsCounter.getOccurrencesMap()};
        writeOutputFileContent(c_OccurrencesMap, out);

        std::cout << "Input file: " << c_InFile << "\n";
        std::cout << "Output file: " << c_OutFile << "\n\n";

        if (!c_OccurrencesMap.empty())
        {
            std::cout << "Found " << c_OccurrencesMap.size() << " words!\n";
        }
        else
        {
            std::cout << "No words found!\n";
        }
    }
    else
    {
        result = 2;
        std::cerr << "Error in opening input and/or output file\n\n";
    }

    return result;
}
