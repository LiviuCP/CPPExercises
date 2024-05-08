#include <thread>
#include <map>
#include <vector>
#include <iostream>

#include "parser.h"
#include "parserfactory.h"
#include "parsingqueue.h"
#include "iaggregator.h"
#include "aggregatorfactory.h"

static const std::map<std::string, ParserFactory::ParserType> c_ParsingOptionsMap
{
    {"-d", ParserFactory::ParserType::DIGITS},
    {"-l", ParserFactory::ParserType::LOWERCASE},
    {"-u", ParserFactory::ParserType::UPPERCASE},
    {"-lu", ParserFactory::ParserType::LOWER_UPPER_CASE},
    {"-ad", ParserFactory::ParserType::ALPHA_AND_DIGITS},
};

static const std::map<std::string, AggregatorFactory::AggregatorType> c_AggregatingOptionsMap
{
    {"-t", AggregatorFactory::AggregatorType::TOTAL_COUNT},
    {"-m", AggregatorFactory::AggregatorType::MIN_COUNT},
    {"-a", AggregatorFactory::AggregatorType::AVERAGE_COUNT},
    {"-M", AggregatorFactory::AggregatorType::MAX_COUNT}
};

static constexpr size_t c_MinRequiredArgs{4};
static constexpr size_t c_MinPoolingThreshold{4};

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
        std::cerr << "Invalid parsing option!\n";
    }
    else if (c_AggregatingOptionsMap.find(argv[2]) == c_AggregatingOptionsMap.cend())
    {
        std::cerr << "Invalid aggregation option!\n";
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
    std::vector<Parser*> parsers;

    size_t filesToParse{0};
    IAggregator* pIAggregator{nullptr};
    const bool c_AreArgumentsOk{checkArguments(argc, argv)};

    if (c_AreArgumentsOk)
    {
        filesToParse = argc - 2;
        pIAggregator = AggregatorFactory::createAggregator(c_AggregatingOptionsMap.at(argv[2]));
    }

    parsers.reserve(filesToParse);

    if (pIAggregator)
    {
        for (size_t fileIndex{0}; fileIndex < filesToParse; ++fileIndex)
        {
            Parser* const pParser{ParserFactory::createParser(c_ParsingOptionsMap.at(argv[1]), argv[fileIndex + 3], pIAggregator)};

            if (pParser)
            {
                parsers.push_back(pParser);
            }
        }
    }

    if (const size_t c_ParsersCount{parsers.size()}; c_ParsersCount >= c_MinPoolingThreshold)
    {
        ParsingQueue parsingQueue{c_MinPoolingThreshold};
        const bool c_IsParsingActive{parsingQueue.addParsingTasks(parsers)};

        if (c_IsParsingActive)
        {
            parsingQueue.finishParsingAndStop();
        }
    }
    else
    {
        std::vector<std::thread> threads;
        threads.reserve(c_ParsersCount);

        for (auto pParser : parsers)
        {
            threads.emplace_back(parse, std::ref(*pParser));
        }

        for (auto& currentThread : threads)
        {
            currentThread.join();
        }
    }

    int totalNrOfMatchingDigits{0};
    int totalNrOfParsedDigits{0};

    for (auto pParser : parsers)
    {
        totalNrOfMatchingDigits += pParser->getTotalFoundCharsCount();
        totalNrOfParsedDigits += pParser->getTotalParsedCharsCount();

        if (pParser->maxCharsExceeeded())
        {
            std::clog << "File " << pParser->getFilePath() << ": the number of characters to parse exceeds the maximum allowed count!\nThe extra chars have been ignored.\n\n";
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

        const CharOccurrencesArray& consolidatedCharOccurrences{pIAggregator->getCharOccurrences()};

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
