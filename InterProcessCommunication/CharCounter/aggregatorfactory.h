#ifndef AGGREGATORFACTORY_H
#define AGGREGATORFACTORY_H

#include "iaggregator.h"

class AggregatorFactory
{
public:
    enum class AggregatorType : unsigned char
    {
        TOTAL_COUNT = 0,
        MIN_COUNT,
        AVERAGE_COUNT,
        MAX_COUNT
    };

    AggregatorFactory() = delete;
    static IAggregator* createAggregator(AggregatorType aggregatorType);
};

#endif // AGGREGATORFACTORY_H
