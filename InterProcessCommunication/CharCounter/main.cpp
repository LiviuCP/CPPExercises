#include <map>
#include <vector>
#include <iostream>

#include "parsingengine.h"

static constexpr size_t c_MinRequiredArgs{4};

static const std::map<std::string, std::string> c_ParsingOptionsLabels{
    {"-d", "digits"},
    {"-l", "lower case"},
    {"-u", "upper case"},
    {"-lu", "lower and upper case"},
    {"-ad", "alphanumeric and digits"}
};

static const std::map<std::string, std::string> c_AggregatingOptionsLabels{
    {"-t", "total occurrences"},
    {"-m", "minimum occurrences"},
    {"-a", "average occurrences"},
    {"-M", "maximum occurrences"}
};

bool checkArguments(int argc, char* argv[])
{
    bool result{false};

    if (static_cast<size_t>(argc) < c_MinRequiredArgs)
    {
        std::cerr << "Insufficient parameters provided. There should be at least 3:\n"
                     "- parsing option\n"
                     "- aggregating option\n"
                     "- one file path\n";
    }
    else if (c_ParsingOptionsLabels.find(argv[1]) == c_ParsingOptionsLabels.cend())
    {
        std::cerr << "Invalid parsing option!\n";
    }
    else if (c_AggregatingOptionsLabels.find(argv[2]) == c_AggregatingOptionsLabels.cend())
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
            std::cout << "Parsed characters: " << c_TotalParsedDigitsCount << "\n";
            std::cout << "Matching characters (" << c_ParsingOptionsLabels.at(argv[1]) << "): " << c_TotalMatchingDigitsCount << "\n";
        }
        else
        {
            std::cout << "No characters have been parsed!\nPlease check if the provided files exist and the right permissions are in place.\n";
        }

        if (c_TotalMatchingDigitsCount > 0)
        {
            std::cout << "\nFound chars distribution (" << c_AggregatingOptionsLabels.at(argv[2]) << "):\n";
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
