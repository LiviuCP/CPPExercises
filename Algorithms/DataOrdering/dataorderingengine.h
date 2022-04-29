#ifndef DATAORDERINGENGINE_H
#define DATAORDERINGENGINE_H

#include <utility>

#include "matrix.h"
#include "datautils.h"

using HammingDistance = int;
using OrderingIndexes = std::vector<size_t>;
using InversionFlags = std::vector<bool>;

class DataOrderingEngine
{
public:
    DataOrderingEngine() = delete;
    DataOrderingEngine(const DataSet& dataSet);

    // to be implemented if required in practice
    DataOrderingEngine(const DataOrderingEngine&) = delete;
    DataOrderingEngine(DataOrderingEngine&&) = delete;
    DataOrderingEngine& operator=(const DataOrderingEngine&) = delete;
    DataOrderingEngine& operator=(DataOrderingEngine&&) = delete;

    void performGreedyMinSimplified();
    void performGreedyMinSimplifiedWithInversion();

    void setDataSet(const DataSet& dataSet);

    const DataSet& getDataSet() const;
    const OrderingIndexes& getOrderingIndexes() const;
    const InversionFlags& getInversionFlags() const;
    HammingDistance getTotalTransitionsCount() const;

private:
    using AdjacencyMatrix = Matrix<HammingDistance>;

    // indexes of a pair of data words (as contained in the data set but converted to adjacency matrix index types)
    using WordsPair = std::pair<AdjacencyMatrix::size_type, AdjacencyMatrix::size_type>;

    static HammingDistance _getHammingDistance(const DataWord& firstWord, const DataWord& secondWord);

    void _reset();
    void _init();
    void _buildAdjacencyMatrix();
    bool _retrieveMinimumDistancePair(bool inversionAllowed, WordsPair& minDistancePair) const;

    AdjacencyMatrix::size_type _initGreedyMinSimplified(bool inversionAllowed, std::vector<bool>& wordAlreadyAddedStatuses);

    DataSet mDataSet;
    AdjacencyMatrix mAdjacencyMatrix;
    OrderingIndexes mOrderingIndexes; // original index of each word (permutation occurs by indexes, original dataset is not modified)
    InversionFlags mInversionFlags; // each word has a flag mentioning if inverted or not (flags are also "permutated")
};

#endif // DATAORDERINGENGINE_H
