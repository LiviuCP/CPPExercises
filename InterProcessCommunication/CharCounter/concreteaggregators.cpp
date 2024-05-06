#include "concreteaggregators.h"

Aggregator::Aggregator()
{
    std::fill(m_CharOccurrences.begin(), m_CharOccurrences.end(), 0);
}

void Aggregator::aggregate(const CharOccurrencesArray& charOccurrences)
{
    std::lock_guard<std::mutex> lock{m_AggregationMutex};

    for (size_t index{0}; index < charOccurrences.size(); ++index)
    {
        m_CharOccurrences[index] += charOccurrences[index];
    }
}

const CharOccurrencesArray& Aggregator::getCharOccurrences() const
{
    return m_CharOccurrences;
}
