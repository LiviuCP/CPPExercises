#include <set>
#include <vector>
#include <iostream>

#include "parsingengine.h"

static constexpr size_t c_MinRequiredArgs{4};

static const std::set<std::string> c_ParsingOptions{"-d", "-l", "-u", "-lu", "-ad"};
static const std::set<std::string> c_AggregatingOptions{"-t", "-m", "-a", "-M"};

bool checkArguments(int argc, char* argv[])
{
    bool result{false};

    if (static_cast<size_t>(argc) < c_MinRequiredArgs)
    {
        std::cerr << "Insufficient parameters provided. There should be at least 2:\n"
                     "- parsing option\n"
                     "- one file path\n";
    }
    else if (c_ParsingOptions.find(argv[1]) == c_ParsingOptions.cend())
    {
        std::cerr << "Invalid parsing option!\n";
    }
    else if (c_AggregatingOptions.find(argv[2]) == c_AggregatingOptions.cend())
    {
        std::cerr << "Invalid aggregation option!\n";
    }
    else
    {
        result = true;
    }

    return result;
}

int main(int argc, char* argv[]) {
    std::vector<Parser*> parsers;

    size_t filesToParse{0};
    const bool c_AreArgumentsOk{checkArguments(argc, argv)};

    if (c_AreArgumentsOk)
    {
        filesToParse = argc - 3;

        std::vector<char*> filePaths;
        for (size_t fileIndex{0}; fileIndex < filesToParse; ++fileIndex)
        {
            filePaths.push_back(argv[fileIndex + 3]);
        }

        ParsingEngine parsingEngine{argv[1], filePaths, argv[2]};
        parsingEngine.run();

        const size_t c_TotalParsedDigitsCount{parsingEngine.getTotalParsedDigitsCount()};
        const size_t c_TotalMatchingDigitsCount{parsingEngine.getTotalMatchingDigitsCount()};

        if (c_TotalParsedDigitsCount > 0)
        {
            std::cout << "Matching characters: " << c_TotalMatchingDigitsCount << "\n";
            std::cout << "Parsed characters: " << c_TotalParsedDigitsCount << "\n";
        }
        else
        {
            std::cout << "No characters have been parsed!\nPlease check if the provided files exist and the right permissions are in place.\n";
        }

        if (c_TotalMatchingDigitsCount > 0)
        {
            std::cout << "\nFound chars distribution: \n";
        }

        const CharOccurrencesArray& consolidatedCharOccurrences{parsingEngine.getCharOccurrences()};

        for (unsigned char ch{0}; ch < consolidatedCharOccurrences.size(); ++ch)
        {
            if (consolidatedCharOccurrences[ch] > 0)
            {
                std::cout << ch << " : " << consolidatedCharOccurrences[ch] << "\n";
            }
        }
    }
    else
    {
        std::cout << "Please try again.\n";
    }

    return 0;
}
