#include <algorithm>

#include "concreteaggregators.h"

Aggregator::Aggregator()
{
}

void Aggregator::aggregate(const CharOccurrencesArray& charOccurrences)
{
    std::lock_guard<std::mutex> lock{m_AggregationMutex};

    for (size_t index{0}; index < charOccurrences.size(); ++index)
    {
        m_TotalCharOccurrences[index] += charOccurrences[index];
    }
}

const CharOccurrencesArray& Aggregator::getCharOccurrences() const
{
    return m_TotalCharOccurrences;
}

MaxAggregator::MaxAggregator()
{
}

void MaxAggregator::aggregate(const CharOccurrencesArray& charOccurrences)
{
    std::lock_guard<std::mutex> lock{m_AggregationMutex};

    for (size_t index{0}; index < charOccurrences.size(); ++index)
    {
        m_MaxCharOccurrences[index] = std::max(m_MaxCharOccurrences[index], charOccurrences[index]);
    }
}

const CharOccurrencesArray& MaxAggregator::getCharOccurrences() const
{
    return m_MaxCharOccurrences;
}

MinAggregator::MinAggregator()
{
}

void MinAggregator::aggregate(const CharOccurrencesArray& charOccurrences)
{
    std::lock_guard<std::mutex> lock{m_AggregationMutex};

    // exclude 0 occurrences from minimum calculation (unless the character really doesn't exist in any of the files)
    for (size_t index{0}; index < charOccurrences.size(); ++index)
    {
        m_MinCharOccurrences[index] = (0 == m_MinCharOccurrences[index] || 0 == charOccurrences[index]) ? std::max(m_MinCharOccurrences[index], charOccurrences[index])
                                                                                                        : std::min(m_MinCharOccurrences[index], charOccurrences[index]);
    }
}

const CharOccurrencesArray& MinAggregator::getCharOccurrences() const
{
    return m_MinCharOccurrences;
}

AverageAggregator::AverageAggregator()
    : m_AggregationsCount{0}
{
}

void AverageAggregator::aggregate(const CharOccurrencesArray& charOccurrences)
{
    Aggregator::aggregate(charOccurrences);
    ++m_AggregationsCount;

    const CharOccurrencesArray& c_TotalCharOccurrences{Aggregator::getCharOccurrences()};
    for (size_t index{0}; index < charOccurrences.size(); ++index)
    {
        m_AverageCharOccurrences[index] = c_TotalCharOccurrences[index] / m_AggregationsCount;

        // do up-rounding (ceiling)
        if (c_TotalCharOccurrences[index] % m_AggregationsCount > 0)
        {
            ++m_AverageCharOccurrences[index];
        }
    }
}

const CharOccurrencesArray& AverageAggregator::getCharOccurrences() const
{
    return m_AverageCharOccurrences;
}
