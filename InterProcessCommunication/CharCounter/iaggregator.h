#ifndef IAGGREGATOR_H
#define IAGGREGATOR_H

#include "utilities.h"

class IAggregator
{
public:
    virtual void aggregate(const CharOccurrencesArray& charOccurrences) = 0;
};

#endif // IAGGREGATOR_H
