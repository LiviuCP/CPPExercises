#pragma once

#include <mutex>

#include "utilities.h"

class IAggregator
{
public:
    virtual void aggregate(const CharOccurrencesArray& charOccurrences) = 0;
    virtual const CharOccurrencesArray& getCharOccurrences() const = 0;
    virtual ~IAggregator(){};

protected:
    std::mutex m_AggregationMutex;
};
