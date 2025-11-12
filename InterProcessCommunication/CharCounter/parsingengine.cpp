#include <iostream>
#include <map>
#include <thread>

#include "aggregatorfactory.h"
#include "parser.h"
#include "parserfactory.h"
#include "parsingengine.h"
#include "parsingqueue.h"

static constexpr size_t c_MinPoolingThreshold{4};

ParsingEngine::ParsingEngine(const std::string& parsingOption, const FilePathsArray& filePaths,
                             const std::string& aggregationOption)
    : m_pIAggregator{nullptr}
    , m_TotalParsedDigitsCount{0}
    , m_TotalMatchingDigitsCount{0}
{
    _buildAggregator(aggregationOption); // required for creating parsers, so should be built first
    _buildParsers(parsingOption, filePaths);
}

ParsingEngine::~ParsingEngine()
{
    for (auto pParser : m_Parsers)
    {
        if (pParser)
        {
            delete pParser;
            pParser = nullptr;
        }
    }

    if (m_pIAggregator)
    {
        delete m_pIAggregator;
        m_pIAggregator = nullptr;
    }
}

void ParsingEngine::run()
{
    // use thread pool only if number of files to be parsed reaches the threshold (otherwise use dedicated thread per
    // parser)
    if (const size_t c_ParsersCount{m_Parsers.size()}; c_ParsersCount >= c_MinPoolingThreshold)
    {
        ParsingQueue parsingQueue{c_MinPoolingThreshold};
        const bool c_IsParsingActive{parsingQueue.addParsingTasks(m_Parsers)};

        if (c_IsParsingActive)
        {
            parsingQueue.finishParsingAndStop();
        }
    }
    else
    {
        std::vector<std::thread> threads;
        threads.reserve(c_ParsersCount);

        for (auto pParser : m_Parsers)
        {
            threads.emplace_back(&Parser::parse, pParser);
        }

        for (auto& currentThread : threads)
        {
            currentThread.join();
        }
    }

    _computeStatistics();
}

size_t ParsingEngine::getTotalParsedDigitsCount() const
{
    return m_TotalParsedDigitsCount;
}

size_t ParsingEngine::getTotalMatchingDigitsCount() const
{
    return m_TotalMatchingDigitsCount;
}

const CharOccurrencesArray& ParsingEngine::getCharOccurrences() const
{
    return m_CharOccurrences;
}

void ParsingEngine::_buildAggregator(const std::string& aggregationOption)
{
    const std::map<std::string, AggregatorFactory::AggregatorType> c_AggregatingOptionsMap{
        {"-t", AggregatorFactory::AggregatorType::TOTAL_COUNT},
        {"-m", AggregatorFactory::AggregatorType::MIN_COUNT},
        {"-a", AggregatorFactory::AggregatorType::AVERAGE_COUNT},
        {"-M", AggregatorFactory::AggregatorType::MAX_COUNT}};

    auto aggregatorTypeIt{c_AggregatingOptionsMap.find(aggregationOption)};

    if (c_AggregatingOptionsMap.cend() != aggregatorTypeIt)
    {
        m_pIAggregator = AggregatorFactory::createAggregator(aggregatorTypeIt->second);
    }
}

void ParsingEngine::_buildParsers(const std::string& parsingOption, const FilePathsArray& filePaths)
{
    if (m_pIAggregator)
    {
        const std::map<std::string, ParserFactory::ParserType> c_ParsingOptionsMap{
            {"-d", ParserFactory::ParserType::DIGITS},
            {"-l", ParserFactory::ParserType::LOWERCASE},
            {"-u", ParserFactory::ParserType::UPPERCASE},
            {"-lu", ParserFactory::ParserType::LOWER_UPPER_CASE},
            {"-ad", ParserFactory::ParserType::ALPHA_AND_DIGITS},
        };

        auto parserTypeIt{c_ParsingOptionsMap.find(parsingOption)};

        if (c_ParsingOptionsMap.cend() != parserTypeIt)
        {
            m_Parsers.clear();
            m_Parsers.reserve(filePaths.size());

            for (const auto& path : filePaths)
            {
                Parser* const pParser{ParserFactory::createParser(parserTypeIt->second, path, m_pIAggregator)};

                if (pParser)
                {
                    m_Parsers.push_back(pParser);
                }
            }
        }
    }
}

void ParsingEngine::_computeStatistics()
{
    for (auto pParser : m_Parsers)
    {
        if (pParser)
        {
            m_TotalMatchingDigitsCount += pParser->getTotalFoundCharsCount();
            m_TotalParsedDigitsCount += pParser->getTotalParsedCharsCount();

            if (pParser->maxCharsExceeeded())
            {
                std::clog << "File " << pParser->getFilePath()
                          << ": the number of characters to parse exceeds the maximum allowed count!\nThe extra chars "
                             "have been ignored.\n\n";
            }
        }
    }

    if (m_pIAggregator)
    {
        m_CharOccurrences = m_pIAggregator->getCharOccurrences();
    }
}
