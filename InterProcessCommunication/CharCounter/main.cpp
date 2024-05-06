#include <thread>
#include <map>
#include <vector>
#include <iostream>

#include "parser.h"
#include "parserfactory.h"
#include "concreteaggregators.h"

static const std::map<std::string, ParserFactory::ParserType> c_ParsingOptionsMap
{
    {"-d", ParserFactory::ParserType::DIGITS},
    {"-l", ParserFactory::ParserType::LOWERCASE},
    {"-u", ParserFactory::ParserType::UPPERCASE},
    {"-lu", ParserFactory::ParserType::LOWER_UPPER_CASE},
    {"-ad", ParserFactory::ParserType::ALPHA_AND_DIGITS},
};

static constexpr size_t c_MinRequiredArgs{3};
static constexpr size_t c_MaxFilesToParseCount{4};

bool checkArguments(int argc, char* argv[])
{
    bool result{false};

    if (argc < c_MinRequiredArgs)
    {
        std::cerr << "Insufficient parameters provided. There should be at least 2:\n"
                     "- parsing option\n"
                     "- one file path\n";
    }
    else if (c_ParsingOptionsMap.find(argv[1]) == c_ParsingOptionsMap.cend())
    {
        std::cerr << "Invalid parsing option\n";
    }
    else
    {
        result = true;
    }

    return result;
}

void parse(Parser& parser)
{
    parser.parse();
}

int main(int argc, char* argv[]) {
    std::vector<std::thread> threads;
    std::vector<Parser*> parsers;

    size_t filesToParse{0};
    const bool c_AreArgumentsOk{checkArguments(argc, argv)};

    if (c_AreArgumentsOk)
    {
        // TODO: implement thread pool in order to be able to parse more files
        filesToParse = argc - 2;
        if (filesToParse > c_MaxFilesToParseCount)
        {
            filesToParse = c_MaxFilesToParseCount;
        }
    }

    parsers.reserve(filesToParse);
    threads.reserve(filesToParse);

    Aggregator aggregator;

    for (size_t fileIndex{0}; fileIndex < filesToParse; ++fileIndex)
    {
        Parser* const pParser{ParserFactory::createParser(c_ParsingOptionsMap.at(argv[1]), argv[fileIndex + 2], &aggregator)};

        if (pParser)
        {
            parsers.push_back(pParser);
            threads.emplace_back(parse, std::ref(*parsers[fileIndex]));
        }
    }

    for (auto& currentThread : threads)
    {
        currentThread.join();
    }

    int totalNrOfMatchingDigits{0};
    int totalNrOfParsedDigits{0};

    for (size_t fileIndex{0}; fileIndex < filesToParse; ++fileIndex)
    {
        totalNrOfMatchingDigits += parsers[fileIndex]->getTotalFoundCharsCount();
        totalNrOfParsedDigits += parsers[fileIndex]->getTotalParsedCharsCount();

        if (parsers[fileIndex]->maxCharsExceeeded())
        {
            std::clog << "File " << argv[fileIndex + 2] << ": the number of matching characters exceeds the maximum allowed count!\n The extra chars have been ignored.\n";
        }
    }

    if (c_AreArgumentsOk)
    {
        if (totalNrOfParsedDigits > 0)
        {
            std::cout << "Matching characters: " << totalNrOfMatchingDigits << "\n";
            std::cout << "Parsed characters: " << totalNrOfParsedDigits << "\n";
        }
        else
        {
            std::cout << "No characters have been parsed!\nPlease check if the provided files exist and the right permissions are in place.\n";
        }

        if (totalNrOfMatchingDigits > 0)
        {
            std::cout << "\nFound chars distribution: \n";
        }

        const CharOccurrencesArray& consolidatedCharOccurrences{aggregator.getCharOccurrences()};

        for (char ch{0}; ch < consolidatedCharOccurrences.size(); ++ch)
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

    for (auto pParser : parsers)
    {
        delete pParser;
        pParser = nullptr;
    }

    return 0;
}
