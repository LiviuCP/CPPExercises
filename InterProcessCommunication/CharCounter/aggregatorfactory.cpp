#include "aggregatorfactory.h"
#include "concreteaggregators.h"

IAggregator* AggregatorFactory::createAggregator(AggregatorType aggregatorType)
{
    IAggregator* pIAggregator{nullptr};

    switch (aggregatorType)
    {
    case AggregatorType::TOTAL_COUNT:
        pIAggregator = new Aggregator;
        break;
    case AggregatorType::MIN_COUNT:
        pIAggregator = new MinAggregator;
        break;
    case AggregatorType::AVERAGE_COUNT:
        pIAggregator = new AverageAggregator;
        break;
    case AggregatorType::MAX_COUNT:
        pIAggregator = new MaxAggregator;
        break;
    default:
        break;
    }

    return pIAggregator;
}
