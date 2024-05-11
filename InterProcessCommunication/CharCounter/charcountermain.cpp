#include <map>
#include <iostream>

#include "parsingengine.h"

static constexpr size_t c_MinRequiredDataParamsCount{3};

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

    const size_t c_DataParamsCount{static_cast<size_t>(argc - 1)}; // application executable taken out from arguments count

    if (c_DataParamsCount < c_MinRequiredDataParamsCount)
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

    const bool c_AreArgumentsOk{checkArguments(argc, argv)};

    if (c_AreArgumentsOk)
    {
        const size_t c_NonFilePathArgumentsCount{3};
        const size_t c_FilePathsCount{static_cast<size_t>(argc) - c_NonFilePathArgumentsCount}; // take out the application executable path, parsing and aggregation options

        FilePathsArray filePaths;
        for (size_t fileIndex{0}; fileIndex < c_FilePathsCount; ++fileIndex)
        {
            filePaths.push_back(argv[fileIndex + c_NonFilePathArgumentsCount]);
        }

        const std::string c_ParsingOption{argv[1]};
        const std::string c_AggregatingOption{argv[2]};

        ParsingEngine parsingEngine{c_ParsingOption, filePaths, c_AggregatingOption};
        parsingEngine.run();

        const size_t c_TotalParsedDigitsCount{parsingEngine.getTotalParsedDigitsCount()};
        const size_t c_TotalMatchingDigitsCount{parsingEngine.getTotalMatchingDigitsCount()};

        if (c_TotalParsedDigitsCount > 0)
        {
            std::cout << "Parsed characters: " << c_TotalParsedDigitsCount << "\n";
            std::cout << "Matching characters (" << c_ParsingOptionsLabels.at(c_ParsingOption) << "): " << c_TotalMatchingDigitsCount << "\n";
        }
        else
        {
            std::cout << "No characters have been parsed!\nPlease check if the provided files exist and the right permissions are in place.\n";
        }

        if (c_TotalMatchingDigitsCount > 0)
        {
            std::cout << "\nFound chars distribution (" << c_AggregatingOptionsLabels.at(c_AggregatingOption) << "):\n";
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
