#ifndef IAGGREGATOR_H
#define IAGGREGATOR_H

#include <mutex>

#include "utilities.h"

class IAggregator
{
public:
    virtual void aggregate(const CharOccurrencesArray& charOccurrences) = 0;
    virtual const CharOccurrencesArray& getCharOccurrences() const = 0;

protected:
    std::mutex m_AggregationMutex;
};

#endif // IAGGREGATOR_H
