#pragma once

#include <utility>

#include "matrix.h"
#include "datautils.h"

using HammingDistance = std::optional<size_t>;
using OrderingIndex = matrix_size_t;
using OrderingIndexes = std::vector<OrderingIndex>;
using InversionFlags = DataWord;
using StatusFlags = DataWord;

class DataOrderingEngine
{
public:
    DataOrderingEngine(const DataSet& dataSet = {});

    // to be implemented if required in practice
    DataOrderingEngine(const DataOrderingEngine&) = delete;
    DataOrderingEngine(DataOrderingEngine&&) = delete;
    DataOrderingEngine& operator=(const DataOrderingEngine&) = delete;
    DataOrderingEngine& operator=(DataOrderingEngine&&) = delete;

    void performGreedyMinSimplified();
    void performGreedyMinSimplifiedUsingInversion();

    void setDataSet(const DataSet& dataSet);

    const DataSet& getOrderedDataSet() const;
    const OrderingIndexes& getOrderingIndexes() const;
    const InversionFlags& getInversionFlags() const;
    HammingDistance getTotalTransitionsCount() const;

private:
    using AdjacencyMatrix = Matrix<HammingDistance>;

    // indexes of a pair of data words (as contained in the data set but converted to adjacency matrix index types)
    using OrderingIndexesPair = std::pair<OrderingIndex, OrderingIndex>;

    static HammingDistance _getHammingDistance(const DataWord& firstWord, const DataWord& secondWord);

    void _init();
    void _computeWordSize();
    void _buildAdjacencyMatrix();
    void _reset();

    std::optional<OrderingIndex> _initGreedyMinSimplified(bool inversionAllowed, StatusFlags& wordAlreadyAddedStatuses);
    void _retrieveFirstTwoOrderedWords(std::optional<OrderingIndexesPair>& minDistancePair) const;
    bool _retrieveFirstTwoOrderedWordsUsingInversion(std::optional<OrderingIndexesPair>& minDistancePair) const;
    void _retrieveNextOrderedWord(const StatusFlags& wordAlreadyAddedStatuses, const std::optional<OrderingIndex>& currentWordIndex, std::optional<OrderingIndex>& nextWordIndex) const;
    bool _retrieveNextOrderedWordUsingInversion(const StatusFlags& wordAlreadyAddedStatuses, const std::optional<OrderingIndex>& currentWordIndex, std::optional<OrderingIndex>& nextWordIndex) const;
    HammingDistance _retrieveDistanceBetweenFirstTwoUnorderedWords() const;
    void _updateOrderedDataSet();

    DataSet m_DataSet;
    DataSet m_OrderedDataSet;
    AdjacencyMatrix m_AdjacencyMatrix;
    OrderingIndexes m_OrderingIndexes; // original index of each word (permutation occurs by indexes, original dataset is not modified)
    InversionFlags m_InversionFlags; // each word has a flag mentioning if inverted or not (flags are also "permutated")
    HammingDistance m_WordSize;
};
