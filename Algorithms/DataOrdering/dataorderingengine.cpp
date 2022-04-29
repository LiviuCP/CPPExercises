#include <cassert>
#include <algorithm>

#include "dataorderingengine.h"

DataOrderingEngine::DataOrderingEngine(const DataSet& dataSet)
    : mDataSet{dataSet}
{
    _init();
}

void DataOrderingEngine::performGreedyMinSimplified()
{
    std::vector<bool> wordAlreadyAddedStatuses;
    AdjacencyMatrix::size_type currentWordIndex{_initGreedyMinSimplified(false, wordAlreadyAddedStatuses)};

    // continue with the remaining words, add them at one end of the ordered set (first the distance between second added word and another one is considered and so on...)
    size_t addedWordsCount{static_cast<size_t>(std::count_if(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(), [](bool value){return value;}))};

    auto findMinDistanceSuccessor = [&wordAlreadyAddedStatuses, &currentWordIndex](AdjacencyMatrix::size_type columnNr, HammingDistance currentHammingDistance, HammingDistance& currentMinHammingDistance)
    {
        if (!wordAlreadyAddedStatuses.at(columnNr) && currentHammingDistance < currentMinHammingDistance)
        {
            currentMinHammingDistance = currentHammingDistance;
            currentWordIndex = columnNr;
        }
    };

    while (addedWordsCount < mDataSet.size())
    {
        HammingDistance currentMinHammingDistance{static_cast<HammingDistance>(mDataSet.at(0).size()) + 1}; // start with maximum distance (add 1 to ensure one of the remaining words is added to ordered set)

        AdjacencyMatrix::ConstZIterator currentWordIt{mAdjacencyMatrix.getConstZIterator(currentWordIndex, currentWordIndex)};

        for (AdjacencyMatrix::ConstZIterator it{mAdjacencyMatrix.constZRowBegin(currentWordIt.getRowNr())}; it != currentWordIt; ++it)
        {
            findMinDistanceSuccessor(it.getColumnNr(), *it, currentMinHammingDistance);
        }

        for (AdjacencyMatrix::ConstZIterator it{++currentWordIt}; it != mAdjacencyMatrix.constZRowEnd(currentWordIt.getRowNr()); ++it)
        {
            findMinDistanceSuccessor(it.getColumnNr(), *it, currentMinHammingDistance);
        }

        mOrderingIndexes.at(addedWordsCount) = currentWordIndex;
        mInversionFlags.at(addedWordsCount) = false;
        wordAlreadyAddedStatuses.at(currentWordIndex) = true;

        ++addedWordsCount;
    }
}

void DataOrderingEngine::performGreedyMinSimplifiedWithInversion()
{
    std::vector<bool> wordAlreadyAddedStatuses;
    AdjacencyMatrix::size_type currentWordIndex{_initGreedyMinSimplified(true, wordAlreadyAddedStatuses)};

    const HammingDistance c_MaxHammingDistance{static_cast<HammingDistance>(mDataSet.at(0).size())};

    // continue with the remaining words, add them at one end of the ordered set (first the distance between second added word and another one is considered and so on...)
    size_t addedWordsCount{static_cast<size_t>(std::count_if(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(), [](bool value){return value;}))};

    auto findMinDistanceSuccessor = [c_MaxHammingDistance, &wordAlreadyAddedStatuses, &currentWordIndex](AdjacencyMatrix::size_type columnNr, HammingDistance currentHammingDistance, HammingDistance& currentMinHammingDistance, bool& isInvertedSuccessor)
    {
        if (!wordAlreadyAddedStatuses.at(columnNr))
        {
            if (currentHammingDistance < currentMinHammingDistance)
            {
                currentMinHammingDistance = currentHammingDistance;
                currentWordIndex = columnNr;
                isInvertedSuccessor = false;
            }

            const HammingDistance invertedHammingDistance{c_MaxHammingDistance - currentHammingDistance};

            if (invertedHammingDistance < currentMinHammingDistance)
            {
                currentMinHammingDistance = invertedHammingDistance;
                currentWordIndex = columnNr;
                isInvertedSuccessor = true;
            }
        }
    };

    while (addedWordsCount < mDataSet.size())
    {
        HammingDistance currentMinHammingDistance{c_MaxHammingDistance + 1}; // start with maximum distance (add 1 to ensure one of the remaining words is added to ordered set)
        bool isInvertedSuccessor{false};

        AdjacencyMatrix::ConstZIterator currentWordIt{mAdjacencyMatrix.getConstZIterator(currentWordIndex, currentWordIndex)};

        for (AdjacencyMatrix::ConstZIterator it{mAdjacencyMatrix.constZRowBegin(currentWordIt.getRowNr())}; it != currentWordIt; ++it)
        {
            findMinDistanceSuccessor(it.getColumnNr(), *it, currentMinHammingDistance, isInvertedSuccessor);
        }

        for (AdjacencyMatrix::ConstZIterator it{++currentWordIt}; it != mAdjacencyMatrix.constZRowEnd(currentWordIt.getRowNr()); ++it)
        {
            findMinDistanceSuccessor(it.getColumnNr(), *it, currentMinHammingDistance, isInvertedSuccessor);
        }

        mOrderingIndexes.at(addedWordsCount) = currentWordIndex;
        mInversionFlags.at(addedWordsCount) = isInvertedSuccessor ? !mInversionFlags.at(addedWordsCount - 1) : mInversionFlags.at(addedWordsCount - 1);
        wordAlreadyAddedStatuses.at(currentWordIndex) = true;

        ++addedWordsCount;
    }
}

void DataOrderingEngine::setDataSet(const DataSet& dataSet)
{
    _reset();
    mDataSet = dataSet;
    _init();
}

const DataSet& DataOrderingEngine::getDataSet() const
{
    return mDataSet;
}

const OrderingIndexes& DataOrderingEngine::getOrderingIndexes() const
{
    return mOrderingIndexes;
}

const InversionFlags& DataOrderingEngine::getInversionFlags() const
{
    return mInversionFlags;
}

HammingDistance DataOrderingEngine::getTotalTransitionsCount() const
{
    HammingDistance transitionsCount{0};

    const size_t cDataSetSize{mDataSet.size()};

    if (cDataSetSize > 1)
    {
        const HammingDistance cWordSize{static_cast<HammingDistance>(mDataSet.at(0).size())};

        for (size_t currentWordIndex{0}; currentWordIndex < cDataSetSize - 1; ++currentWordIndex)
        {
            const AdjacencyMatrix::size_type cFirstWordIndex{static_cast<AdjacencyMatrix::size_type>(mOrderingIndexes.at(currentWordIndex))};
            const AdjacencyMatrix::size_type cSecondWordIndex{static_cast<AdjacencyMatrix::size_type>(mOrderingIndexes.at(currentWordIndex + 1))};
            const bool cAreInInvertedRelation{mInversionFlags.at(currentWordIndex) != mInversionFlags.at(currentWordIndex + 1)};

            HammingDistance hammingDistance{mAdjacencyMatrix.at(cFirstWordIndex, cSecondWordIndex)};

            if (cAreInInvertedRelation)
            {
                hammingDistance = cWordSize - hammingDistance;
            }

            transitionsCount += hammingDistance;
        }
    }

    return transitionsCount;
}

HammingDistance DataOrderingEngine::_getHammingDistance(const DataWord& firstWord, const DataWord& secondWord)
{
    HammingDistance hammingDistance{-1}; // this value is for unequal size words only (error case)

    const size_t c_WordSize{firstWord.size()};

    if (c_WordSize == secondWord.size())
    {
        hammingDistance = 0;

        for (size_t currentPos{0}; currentPos < c_WordSize; ++currentPos)
        {
            if (firstWord.at(currentPos) != secondWord.at(currentPos))
            {
                ++hammingDistance;
            }
        }
    }

    return hammingDistance;
}

void DataOrderingEngine::_reset()
{
    mDataSet.clear();
    mAdjacencyMatrix.clear();
    mOrderingIndexes.clear();
    mInversionFlags.clear();
}

void DataOrderingEngine::_init()
{
    const size_t cWordsCount{mDataSet.size()};

    if (cWordsCount > 0)
    {
        _buildAdjacencyMatrix();

        if (mAdjacencyMatrix.getNrOfRows() > 0)
        {
            mOrderingIndexes.reserve(cWordsCount);
            mInversionFlags.reserve(cWordsCount);

            for (size_t currentWordIndex{0}; currentWordIndex < cWordsCount; ++currentWordIndex)
            {
                mOrderingIndexes.push_back(currentWordIndex);
                mInversionFlags.push_back(false);
            }
        }
        else
        {
            assert(false); // an error occurred in matrix calculation, probably different size words
        }
    }
}

void DataOrderingEngine::_buildAdjacencyMatrix()
{
    const AdjacencyMatrix::size_type cWordsCount{static_cast<AdjacencyMatrix::size_type>(mDataSet.size())};

    if (cWordsCount > 0)
    {
        mAdjacencyMatrix.resize(cWordsCount, cWordsCount);

        for (size_t firstWordPos{0}; firstWordPos < mDataSet.size(); ++firstWordPos)
        {
            for (size_t secondWordPos{firstWordPos + 1}; secondWordPos < mDataSet.size(); ++secondWordPos)
            {
                AdjacencyMatrix::size_type rowNumber{static_cast<AdjacencyMatrix::size_type>(firstWordPos)};
                AdjacencyMatrix::size_type columnNumber{static_cast<AdjacencyMatrix::size_type>(secondWordPos)};

                const HammingDistance cHammingDistance{_getHammingDistance(mDataSet.at(firstWordPos), mDataSet.at(secondWordPos))};

                if (cHammingDistance >= 0)
                {
                    mAdjacencyMatrix.at(rowNumber, columnNumber) = cHammingDistance;
                    mAdjacencyMatrix.at(columnNumber, rowNumber) = cHammingDistance;
                }
                else
                {
                    mAdjacencyMatrix.clear();
                    break;
                }
            }
        }

        // main diagonal contains Hamming distances from each word to itself
        for (AdjacencyMatrix::DIterator it{mAdjacencyMatrix.dBegin(0)}; it != mAdjacencyMatrix.dEnd(0); ++it)
        {
            *it = 0;
        }
    }
    else
    {
        mAdjacencyMatrix.clear();
    }
}

bool DataOrderingEngine::_retrieveMinimumDistancePair(bool inversionAllowed, DataOrderingEngine::WordsPair& minDistancePair) const
{
    bool areInverted{false};

    if (mAdjacencyMatrix.getNrOfRows() > 1)
    {
        AdjacencyMatrix::size_type currentFirstWordIndex{0};
        AdjacencyMatrix::size_type currentSecondWordIndex{1};

        const AdjacencyMatrix::size_type c_PositiveDiagonalsCount{mAdjacencyMatrix.getNrOfColumns() - 1};

        HammingDistance currentDistance{mAdjacencyMatrix.at(currentFirstWordIndex, currentSecondWordIndex)};

        if (inversionAllowed)
        {
            const HammingDistance c_WordSize{static_cast<HammingDistance>(mDataSet.at(0).size())};
            const HammingDistance c_FirstPairInvertedDistance{c_WordSize - currentDistance};

            if (c_FirstPairInvertedDistance < currentDistance)
            {
                currentDistance = c_FirstPairInvertedDistance;
                areInverted = true;
            }

            for (AdjacencyMatrix::size_type currentDiagNr{1}; currentDiagNr < c_PositiveDiagonalsCount; ++currentDiagNr)
            {
                for (AdjacencyMatrix::ConstDIterator it{mAdjacencyMatrix.constDBegin(currentDiagNr)}; it != mAdjacencyMatrix.constDEnd(currentDiagNr); ++it)
                {
                    if (*it < currentDistance)
                    {
                        currentDistance = *it;
                        currentFirstWordIndex = it.getRowNr();
                        currentSecondWordIndex = it.getColumnNr();
                        areInverted = false;
                    }

                    const HammingDistance c_CurrentPairInvertedDistance{c_WordSize - *it};

                    if (c_CurrentPairInvertedDistance < currentDistance)
                    {
                        currentDistance = c_CurrentPairInvertedDistance;
                        currentFirstWordIndex = it.getRowNr();
                        currentSecondWordIndex = it.getColumnNr();
                        areInverted = true;
                    }
                }
            }
        }
        else
        {
            for (AdjacencyMatrix::size_type currentDiagNr{1}; currentDiagNr < c_PositiveDiagonalsCount; ++currentDiagNr)
            {
                for (AdjacencyMatrix::ConstDIterator it{mAdjacencyMatrix.constDBegin(currentDiagNr)}; it != mAdjacencyMatrix.constDEnd(currentDiagNr); ++it)
                {
                    if (*it < currentDistance)
                    {
                        currentDistance = *it;
                        currentFirstWordIndex = it.getRowNr();
                        currentSecondWordIndex = it.getColumnNr();
                    }
                }
            }
        }

        minDistancePair = WordsPair{currentFirstWordIndex, currentSecondWordIndex};
    }

    return areInverted;
}

DataOrderingEngine::AdjacencyMatrix::size_type DataOrderingEngine::_initGreedyMinSimplified(bool inversionAllowed, std::vector<bool>& wordAlreadyAddedStatuses)
{
    AdjacencyMatrix::size_type currentWordIndex{-1};

    const size_t c_DataSetSize{mDataSet.size()};

    if (c_DataSetSize > 0)
    {
        wordAlreadyAddedStatuses.resize(c_DataSetSize, false);

        if (c_DataSetSize > 1)
        {
            // get the first two words (that have the minimum Hamming distance) and add them to ordered set
            WordsPair minDistancePair{};
            const bool c_IsInvertedMinPair{_retrieveMinimumDistancePair(inversionAllowed, minDistancePair)};

            mOrderingIndexes.at(0) = static_cast<size_t>(minDistancePair.first);
            mOrderingIndexes.at(1) = static_cast<size_t>(minDistancePair.second);
            mInversionFlags.at(0) = false;
            mInversionFlags.at(1) = c_IsInvertedMinPair;

            wordAlreadyAddedStatuses.at(mOrderingIndexes.at(0)) = true;
            wordAlreadyAddedStatuses.at(mOrderingIndexes.at(1)) = true;

            currentWordIndex = minDistancePair.second;
        }
        else
        {
            mOrderingIndexes.at(0) = 0;
            mInversionFlags.at(0) = false;
            wordAlreadyAddedStatuses.at(0) = true;
            currentWordIndex = 0;
        }
    }
    else
    {
        wordAlreadyAddedStatuses.clear();
    }

    return currentWordIndex;
}
