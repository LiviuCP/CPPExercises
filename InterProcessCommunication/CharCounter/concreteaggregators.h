#ifndef CONCRETEAGGREGATORS_H
#define CONCRETEAGGREGATORS_H

#include <mutex>

#include "utilities.h"
#include "iaggregator.h"

class Aggregator : public IAggregator
{
public:
    Aggregator();
    void aggregate(const CharOccurrencesArray& charOccurrences) override;
    const CharOccurrencesArray& getCharOccurrences() const;

private:
    CharOccurrencesArray m_CharOccurrences;
    std::mutex m_AggregationMutex;
};

#endif // CONCRETEAGGREGATORS_H
