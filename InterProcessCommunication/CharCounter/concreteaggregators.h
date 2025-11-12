#pragma once

#include "iaggregator.h"
#include "utilities.h"

class Aggregator : public IAggregator
{
public:
    Aggregator();
    void aggregate(const CharOccurrencesArray& charOccurrences) override;
    const CharOccurrencesArray& getCharOccurrences() const override;

private:
    CharOccurrencesArray m_TotalCharOccurrences;
};

class MaxAggregator : public IAggregator
{
public:
    MaxAggregator();
    void aggregate(const CharOccurrencesArray& charOccurrences) override;
    const CharOccurrencesArray& getCharOccurrences() const override;

private:
    CharOccurrencesArray m_MaxCharOccurrences;
};

class MinAggregator : public IAggregator
{
public:
    MinAggregator();
    void aggregate(const CharOccurrencesArray& charOccurrences) override;
    const CharOccurrencesArray& getCharOccurrences() const override;

private:
    CharOccurrencesArray m_MinCharOccurrences;
};

class AverageAggregator : public Aggregator
{
public:
    AverageAggregator();
    void aggregate(const CharOccurrencesArray& charOccurrences) override;
    const CharOccurrencesArray& getCharOccurrences() const override;

private:
    CharOccurrencesArray m_AverageCharOccurrences;
    size_t m_AggregationsCount;
};
