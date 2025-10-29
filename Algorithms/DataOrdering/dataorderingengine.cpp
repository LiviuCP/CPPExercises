#include <cassert>
#include <algorithm>

#include "dataorderingengine.h"

DataOrderingEngine::DataOrderingEngine(const DataSet& dataSet)
{
    setDataSet(dataSet);
}

void DataOrderingEngine::performGreedyMinSimplified()
{
    std::vector<bool> wordAlreadyAddedStatuses;
    std::optional<matrix_size_t> currentWordIndex{_initGreedyMinSimplified(false, wordAlreadyAddedStatuses)};

    // continue with the remaining words, add them at one end of the ordered set (first the distance between second added word and another one is considered and so on...)
    size_t addedWordsCount{static_cast<size_t>(std::count_if(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(), [](bool value){return value;}))};

    auto findMinDistanceSuccessor = [&wordAlreadyAddedStatuses, &currentWordIndex](matrix_size_t columnNr, HammingDistance currentHammingDistance, HammingDistance& currentMinHammingDistance)
    {
        if (!wordAlreadyAddedStatuses.at(columnNr) && currentHammingDistance < currentMinHammingDistance)
        {
            currentMinHammingDistance = currentHammingDistance;
            currentWordIndex = columnNr;
        }
    };

    while (addedWordsCount < mDataSet.size())
    {
        // start with maximum distance (add 1 to ensure one of the remaining words is added to ordered set)
        HammingDistance currentMinHammingDistance{static_cast<HammingDistance>(mDataSet.at(0).size()) + 1};

        if (!currentWordIndex.has_value())
        {
            assert(false);
            break;
        }

        AdjacencyMatrix::ConstZIterator currentWordIt{mAdjacencyMatrix.getConstZIterator(*currentWordIndex, *currentWordIndex)};

        const std::optional<matrix_size_t> c_CurrentWordItRowNr{currentWordIt.getRowNr()};

        if (!c_CurrentWordItRowNr.has_value())
        {
            assert(false);
            break;
        }

        // no need to check optional validity for it.getColumnNr() before getting its value (*) because the matrix is presumed non-empty (see above check) and the iterator is not reverse

        for (AdjacencyMatrix::ConstZIterator it{mAdjacencyMatrix.constZRowBegin(*c_CurrentWordItRowNr)}; it != currentWordIt; ++it)
        {
            findMinDistanceSuccessor(*it.getColumnNr(), *it, currentMinHammingDistance);
        }

        for (AdjacencyMatrix::ConstZIterator it{++currentWordIt}; it != mAdjacencyMatrix.constZRowEnd(*c_CurrentWordItRowNr); ++it)
        {
            findMinDistanceSuccessor(*it.getColumnNr(), *it, currentMinHammingDistance);
        }

        mOrderingIndexes.at(addedWordsCount) = *currentWordIndex;
        mInversionFlags.at(addedWordsCount) = false;
        wordAlreadyAddedStatuses.at(*currentWordIndex) = true;

        ++addedWordsCount;
    }

    _updateOrderedDataSet();
}

void DataOrderingEngine::performGreedyMinSimplifiedWithInversion()
{
    std::vector<bool> wordAlreadyAddedStatuses;
    std::optional<matrix_size_t> currentWordIndex{_initGreedyMinSimplified(true, wordAlreadyAddedStatuses)};

    const HammingDistance c_MaxHammingDistance{static_cast<HammingDistance>(mDataSet.at(0).size())};

    // continue with the remaining words, add them at one end of the ordered set (first the distance between second added word and another one is considered and so on...)
    size_t addedWordsCount{static_cast<size_t>(std::count_if(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(), [](bool value){return value;}))};

    auto findMinDistanceSuccessor = [c_MaxHammingDistance, &wordAlreadyAddedStatuses, &currentWordIndex](matrix_size_t columnNr, HammingDistance currentHammingDistance, HammingDistance& currentMinHammingDistance, bool& isInvertedSuccessor)
    {
        if (!wordAlreadyAddedStatuses.at(columnNr))
        {
            if (currentHammingDistance < currentMinHammingDistance)
            {
                currentMinHammingDistance = currentHammingDistance;
                currentWordIndex = columnNr;
                isInvertedSuccessor = false;
            }

            const HammingDistance c_InvertedHammingDistance{c_MaxHammingDistance - currentHammingDistance};

            if (c_InvertedHammingDistance < currentMinHammingDistance)
            {
                currentMinHammingDistance = c_InvertedHammingDistance;
                currentWordIndex = columnNr;
                isInvertedSuccessor = true;
            }
        }
    };

    while (addedWordsCount < mDataSet.size())
    {
        HammingDistance currentMinHammingDistance{c_MaxHammingDistance + 1}; // start with maximum distance (add 1 to ensure one of the remaining words is added to ordered set)
        bool isInvertedSuccessor{false};

        if (!currentWordIndex.has_value())
        {
            assert(false);
            break;
        }

        AdjacencyMatrix::ConstZIterator currentWordIt{mAdjacencyMatrix.getConstZIterator(*currentWordIndex, *currentWordIndex)};

        const std::optional<matrix_size_t> c_CurrentWordItRowNr{currentWordIt.getRowNr()};

        if (!c_CurrentWordItRowNr.has_value())
        {
            assert(false);
            break;
        }

        // no need to check optional validity for it.getColumnNr() before getting its value (*) because the matrix is presumed non-empty (see above check) and the iterator is not reverse

        for (AdjacencyMatrix::ConstZIterator it{mAdjacencyMatrix.constZRowBegin(*c_CurrentWordItRowNr)}; it != currentWordIt; ++it)
        {
            findMinDistanceSuccessor(*it.getColumnNr(), *it, currentMinHammingDistance, isInvertedSuccessor);
        }

        for (AdjacencyMatrix::ConstZIterator it{++currentWordIt}; it != mAdjacencyMatrix.constZRowEnd(*c_CurrentWordItRowNr); ++it)
        {
            findMinDistanceSuccessor(*it.getColumnNr(), *it, currentMinHammingDistance, isInvertedSuccessor);
        }

        mOrderingIndexes.at(addedWordsCount) = *currentWordIndex;
        mInversionFlags.at(addedWordsCount) = isInvertedSuccessor ? !mInversionFlags.at(addedWordsCount - 1) : mInversionFlags.at(addedWordsCount - 1);
        wordAlreadyAddedStatuses.at(*currentWordIndex) = true;

        ++addedWordsCount;
    }

    _updateOrderedDataSet();
}

void DataOrderingEngine::setDataSet(const DataSet& dataSet)
{
    _reset();
    mDataSet = dataSet;
    mOrderedDataSet = dataSet;
    _init();
}

const DataSet& DataOrderingEngine::getOrderedDataSet() const
{
    return mOrderedDataSet;
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

    const size_t c_DataSetSize{mDataSet.size()};

    if (c_DataSetSize > 1)
    {
        const HammingDistance cWordSize{static_cast<HammingDistance>(mDataSet.at(0).size())};

        for (size_t currentWordIndex{0}; currentWordIndex < c_DataSetSize - 1; ++currentWordIndex)
        {
            const matrix_size_t c_FirstWordIndex{static_cast<matrix_size_t>(mOrderingIndexes.at(currentWordIndex))};
            const matrix_size_t c_SecondWordIndex{static_cast<matrix_size_t>(mOrderingIndexes.at(currentWordIndex + 1))};
            const bool c_AreInInvertedRelation{mInversionFlags.at(currentWordIndex) != mInversionFlags.at(currentWordIndex + 1)};

            HammingDistance hammingDistance{mAdjacencyMatrix.at(c_FirstWordIndex, c_SecondWordIndex)};

            if (c_AreInInvertedRelation)
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
    mOrderedDataSet.clear();
    mAdjacencyMatrix.clear();
    mOrderingIndexes.clear();
    mInversionFlags.clear();
}

void DataOrderingEngine::_init()
{
    const size_t c_WordsCount{mDataSet.size()};

    if (c_WordsCount > 0)
    {
        _buildAdjacencyMatrix();

        if (mAdjacencyMatrix.getNrOfRows() > 0)
        {
            mOrderingIndexes.reserve(c_WordsCount);
            mInversionFlags.reserve(c_WordsCount);

            for (size_t currentWordIndex{0}; currentWordIndex < c_WordsCount; ++currentWordIndex)
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
    const matrix_size_t c_WordsCount{static_cast<matrix_size_t>(mDataSet.size())};

    if (c_WordsCount > 0)
    {
        mAdjacencyMatrix.resize(c_WordsCount, c_WordsCount);

        for (size_t firstWordPos{0}; firstWordPos < mDataSet.size(); ++firstWordPos)
        {
            for (size_t secondWordPos{firstWordPos + 1}; secondWordPos < mDataSet.size(); ++secondWordPos)
            {
                const matrix_size_t c_RowNumber{static_cast<matrix_size_t>(firstWordPos)};
                const matrix_size_t c_ColumnNumber{static_cast<matrix_size_t>(secondWordPos)};

                const HammingDistance c_HammingDistance{_getHammingDistance(mDataSet.at(firstWordPos), mDataSet.at(secondWordPos))};

                if (c_HammingDistance >= 0)
                {
                    mAdjacencyMatrix.at(c_RowNumber, c_ColumnNumber) = c_HammingDistance;
                    mAdjacencyMatrix.at(c_ColumnNumber, c_RowNumber) = c_HammingDistance;
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
        matrix_size_t currentFirstWordIndex{0};
        matrix_size_t currentSecondWordIndex{1};

        const matrix_size_t c_PositiveDiagonalsCount{mAdjacencyMatrix.getNrOfColumns() - 1};

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

            for (matrix_size_t currentDiagNr{1}; currentDiagNr < c_PositiveDiagonalsCount; ++currentDiagNr)
            {
                for (AdjacencyMatrix::ConstDIterator it{mAdjacencyMatrix.constDBegin(currentDiagNr)}; it != mAdjacencyMatrix.constDEnd(currentDiagNr); ++it)
                {
                    // no need to check the validity of getRowNr() and getColumnNr() as the matrix is not empty and the iterators are not reverse
                    const matrix_size_t c_RowNr{*it.getRowNr()};
                    const matrix_size_t c_ColumnNr{*it.getColumnNr()};

                    if (*it < currentDistance)
                    {
                        currentDistance = *it;
                        currentFirstWordIndex = c_RowNr;
                        currentSecondWordIndex = c_ColumnNr;
                        areInverted = false;
                    }

                    const HammingDistance c_CurrentPairInvertedDistance{c_WordSize - *it};

                    if (c_CurrentPairInvertedDistance < currentDistance)
                    {
                        currentDistance = c_CurrentPairInvertedDistance;
                        currentFirstWordIndex = c_RowNr;
                        currentSecondWordIndex = c_ColumnNr;
                        areInverted = true;
                    }
                }
            }
        }
        else
        {
            for (matrix_size_t currentDiagNr{1}; currentDiagNr < c_PositiveDiagonalsCount; ++currentDiagNr)
            {
                for (AdjacencyMatrix::ConstDIterator it{mAdjacencyMatrix.constDBegin(currentDiagNr)}; it != mAdjacencyMatrix.constDEnd(currentDiagNr); ++it)
                {
                    if (*it < currentDistance)
                    {
                        currentDistance = *it;

                        // no need to check the validity of getRowNr() and getColumnNr() as the matrix is not empty and the iterators are not reverse
                        currentFirstWordIndex = *it.getRowNr();
                        currentSecondWordIndex = *it.getColumnNr();
                    }
                }
            }
        }

        minDistancePair = WordsPair{currentFirstWordIndex, currentSecondWordIndex};
    }

    return areInverted;
}

void DataOrderingEngine::_updateOrderedDataSet()
{
    DataSet orderedDataSet;
    const size_t c_DataSetSize{mDataSet.size()};

    if (c_DataSetSize == mOrderingIndexes.size() && c_DataSetSize == mInversionFlags.size())
    {
        orderedDataSet.reserve(c_DataSetSize);

        for (size_t currentWordIndex{0}; currentWordIndex < c_DataSetSize; ++currentWordIndex)
        {
            if (const bool c_ShouldInvert{mInversionFlags.at(currentWordIndex)}; c_ShouldInvert)
            {
                orderedDataSet.push_back(Utilities::invertDataWord(mDataSet.at(mOrderingIndexes.at(currentWordIndex))));
            }
            else
            {
                orderedDataSet.push_back(mDataSet.at(mOrderingIndexes.at(currentWordIndex)));
            }
        }

        mOrderedDataSet = std::move(orderedDataSet);
    }
    else
    {
        assert(false);
    }
}

std::optional<matrix_size_t> DataOrderingEngine::_initGreedyMinSimplified(bool inversionAllowed, std::vector<bool>& wordAlreadyAddedStatuses)
{
    std::optional<matrix_size_t> currentWordIndex;

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
