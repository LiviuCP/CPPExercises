#include <algorithm>
#include <cassert>

#include "dataorderingengine.h"

#define NO_INVERSION false
#define INVERSION_ALLOWED true

DataOrderingEngine::DataOrderingEngine(const DataSet& dataSet)
{
    setDataSet(dataSet);
}

void DataOrderingEngine::performGreedyMinSimplified()
{
    do
    {
        if (!m_WordSize.has_value())
        {
            break;
        }

        const size_t c_DataSetSize{m_DataSet.size()};

        if (0 == c_DataSetSize)
        {
            break;
        }

        if (m_OrderingIndexes.size() != c_DataSetSize || m_InversionFlags.size() != c_DataSetSize)
        {
            assert(false);
            break;
        }

        StatusFlags wordAlreadyAddedStatuses;
        std::optional<OrderingIndex> currentWordIndex{_initGreedyMinSimplified(NO_INVERSION, wordAlreadyAddedStatuses)};

        if (wordAlreadyAddedStatuses.size() != c_DataSetSize)
        {
            assert(false);
            break;
        }

        // continue with the remaining words, add them at one end of the ordered set (first the distance between second
        // added word and another one is considered and so on...)
        size_t addedWordsCount{
            static_cast<size_t>(std::count(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(), true))};

        if (0 == addedWordsCount)
        {
            assert(false);
            break;
        }

        for (; addedWordsCount < c_DataSetSize; ++addedWordsCount)
        {
            std::optional<OrderingIndex> nextWordIndex;
            _retrieveNextOrderedWord(wordAlreadyAddedStatuses, currentWordIndex, nextWordIndex);

            if (!nextWordIndex.has_value() || nextWordIndex >= c_DataSetSize || nextWordIndex == currentWordIndex)
            {
                assert(false);
                break;
            }

            currentWordIndex = nextWordIndex;
            m_OrderingIndexes[addedWordsCount] = *currentWordIndex;
            m_InversionFlags[addedWordsCount] = false;
            wordAlreadyAddedStatuses[*currentWordIndex] = true;
        }

        if (std::any_of(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(),
                        [](bool added) { return !added; }))
        {
            assert(false);
            break;
        }

        _updateOrderedDataSet();
    } while (false);
}

void DataOrderingEngine::performGreedyMinSimplifiedUsingInversion()
{
    do
    {
        if (!m_WordSize.has_value())
        {
            break;
        }

        const size_t c_DataSetSize{m_DataSet.size()};

        if (0 == c_DataSetSize)
        {
            break;
        }

        if (m_OrderingIndexes.size() != c_DataSetSize || m_InversionFlags.size() != c_DataSetSize)
        {
            assert(false);
            break;
        }

        StatusFlags wordAlreadyAddedStatuses;
        std::optional<OrderingIndex> currentWordIndex{
            _initGreedyMinSimplified(INVERSION_ALLOWED, wordAlreadyAddedStatuses)};

        if (wordAlreadyAddedStatuses.size() != c_DataSetSize)
        {
            assert(false);
            break;
        }

        // continue with the remaining words, add them at one end of the ordered set (first the distance between second
        // added word and another one is considered and so on...)
        size_t addedWordsCount{
            static_cast<size_t>(std::count(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(), true))};

        if (0 == addedWordsCount)
        {
            assert(false);
            break;
        }

        for (; addedWordsCount < c_DataSetSize; ++addedWordsCount)
        {
            std::optional<OrderingIndex> nextWordIndex;
            const bool isInvertedSuccessor{
                _retrieveNextOrderedWordUsingInversion(wordAlreadyAddedStatuses, currentWordIndex, nextWordIndex)};

            if (!nextWordIndex.has_value() || nextWordIndex >= c_DataSetSize || nextWordIndex == currentWordIndex)
            {
                assert(false);
                break;
            }

            currentWordIndex = nextWordIndex;
            m_OrderingIndexes[addedWordsCount] = *currentWordIndex;
            m_InversionFlags[addedWordsCount] = isInvertedSuccessor ? !m_InversionFlags.at(addedWordsCount - 1)
                                                                    : m_InversionFlags.at(addedWordsCount - 1);
            wordAlreadyAddedStatuses[*currentWordIndex] = true;
        }

        if (std::any_of(wordAlreadyAddedStatuses.cbegin(), wordAlreadyAddedStatuses.cend(),
                        [](bool added) { return !added; }))
        {
            assert(false);
            break;
        }

        _updateOrderedDataSet();
    } while (false);
}

void DataOrderingEngine::setDataSet(const DataSet& dataSet)
{
    _reset();
    m_DataSet = dataSet;
    m_OrderedDataSet = dataSet;
    _init();
}

const DataSet& DataOrderingEngine::getOrderedDataSet() const
{
    return m_OrderedDataSet;
}

const OrderingIndexes& DataOrderingEngine::getOrderingIndexes() const
{
    return m_OrderingIndexes;
}

const InversionFlags& DataOrderingEngine::getInversionFlags() const
{
    return m_InversionFlags;
}

HammingDistance DataOrderingEngine::getTotalTransitionsCount() const
{
    HammingDistance transitionsCount{0};

    const size_t c_DataSetSize{m_DataSet.size()};
    const size_t c_OrderingIndexesSize{m_OrderingIndexes.size()};
    const size_t c_InversionFlagsSize{m_InversionFlags.size()};
    const size_t c_NrOfRows{m_AdjacencyMatrix.getNrOfRows()};
    const size_t c_NrOfColumns{m_AdjacencyMatrix.getNrOfColumns()};

    assert(c_OrderingIndexesSize == c_DataSetSize);
    assert(c_InversionFlagsSize == c_DataSetSize);
    assert(c_NrOfRows == c_DataSetSize);
    assert(c_NrOfColumns == c_DataSetSize);

    if (m_WordSize.has_value() && m_WordSize > 0 && c_DataSetSize > 1 && c_OrderingIndexesSize == c_DataSetSize &&
        c_InversionFlagsSize == c_DataSetSize && c_NrOfRows == c_DataSetSize && c_NrOfColumns == c_DataSetSize)
    {
        for (size_t currentWordIndex{0}; currentWordIndex < c_DataSetSize - 1; ++currentWordIndex)
        {
            const matrix_size_t c_FirstWordIndex{m_OrderingIndexes[currentWordIndex]};
            const matrix_size_t c_SecondWordIndex{m_OrderingIndexes[currentWordIndex + 1]};

            if (c_FirstWordIndex >= c_DataSetSize || c_SecondWordIndex >= c_DataSetSize ||
                c_FirstWordIndex == c_SecondWordIndex)
            {
                assert(false);
                break;
            }

            HammingDistance hammingDistance{m_AdjacencyMatrix.at(c_FirstWordIndex, c_SecondWordIndex)};

            if (!hammingDistance.has_value() || hammingDistance > m_WordSize)
            {
                assert(false);
                break;
            }

            if (m_InversionFlags[currentWordIndex] != m_InversionFlags[currentWordIndex + 1])
            {
                hammingDistance =
                    *m_WordSize - *hammingDistance; // normalize Hamming distance if one of the words is inverted
            }

            transitionsCount = *transitionsCount + *hammingDistance;
        }
    }

    return transitionsCount;
}

// std::nullopt value is for unequal size words only (error case)
HammingDistance DataOrderingEngine::_getHammingDistance(const DataWord& firstWord, const DataWord& secondWord)
{
    HammingDistance hammingDistance;

    const size_t c_WordSize{firstWord.size()};

    if (c_WordSize == secondWord.size())
    {
        hammingDistance = 0;

        for (size_t currentPos{0}; currentPos < c_WordSize; ++currentPos)
        {
            hammingDistance = *hammingDistance + static_cast<size_t>(firstWord[currentPos] != secondWord[currentPos]);
        }
    }

    return hammingDistance;
}

void DataOrderingEngine::_init()
{
    if (!m_DataSet.empty())
    {
        _computeWordSize();
    }

    if (m_WordSize.has_value() && m_WordSize > 0 && m_AdjacencyMatrix.isEmpty() && m_OrderingIndexes.empty() &&
        m_InversionFlags.empty())
    {
        _buildAdjacencyMatrix();
    }

    if (!m_AdjacencyMatrix.isEmpty())
    {
        const size_t c_WordsCount{m_DataSet.size()};
        assert(c_WordsCount == m_AdjacencyMatrix.getNrOfRows() && c_WordsCount == m_AdjacencyMatrix.getNrOfColumns());

        m_OrderingIndexes.reserve(c_WordsCount);
        m_InversionFlags.reserve(c_WordsCount);

        for (size_t currentWordIndex{0}; currentWordIndex < c_WordsCount; ++currentWordIndex)
        {
            m_OrderingIndexes.push_back(currentWordIndex);
        }

        std::fill_n(std::back_inserter(m_InversionFlags), c_WordsCount, false);
    }
}

void DataOrderingEngine::_computeWordSize()
{
    // null word size means empty or invalid dataset
    m_WordSize.reset();

    if (!m_DataSet.empty())
    {
        const HammingDistance c_WordSize = m_DataSet.at(0).size();

        // the dataset is considered invalid if the words don't have the same size
        if (std::all_of(m_DataSet.begin(), m_DataSet.end(),
                        [c_WordSize](const auto& word) { return c_WordSize == word.size(); }))
        {
            m_WordSize = c_WordSize;
            assert(c_WordSize > 0);
        }
    }
}

void DataOrderingEngine::_buildAdjacencyMatrix()
{
    AdjacencyMatrix newAdjacencyMatrix;

    if (!m_DataSet.empty() && m_WordSize.has_value() && m_WordSize > 0)
    {
        bool areHammingDistancesValid{true};

        const matrix_size_t c_WordsCount{static_cast<matrix_size_t>(m_DataSet.size())};
        newAdjacencyMatrix.resize(c_WordsCount, c_WordsCount);

        for (matrix_size_t firstWordPos{0}; firstWordPos < c_WordsCount; ++firstWordPos)
        {
            for (matrix_size_t secondWordPos{firstWordPos + 1}; secondWordPos < c_WordsCount; ++secondWordPos)
            {
                const HammingDistance c_HammingDistance{
                    _getHammingDistance(m_DataSet.at(firstWordPos), m_DataSet.at(secondWordPos))};

                if (!c_HammingDistance.has_value() || c_HammingDistance > m_WordSize)
                {
                    assert(false);
                    areHammingDistancesValid = false;
                    firstWordPos = c_WordsCount; // stop the outer loop too
                    break;
                }

                newAdjacencyMatrix.at(firstWordPos, secondWordPos) = c_HammingDistance;
                newAdjacencyMatrix.at(secondWordPos, firstWordPos) = c_HammingDistance;
            }

            // main diagonal contains Hamming distances from each word to itself
            newAdjacencyMatrix.at(firstWordPos, firstWordPos) = 0;
        }

        if (!areHammingDistancesValid)
        {
            newAdjacencyMatrix.clear();
        }
    }

    m_AdjacencyMatrix = std::move(newAdjacencyMatrix);
}

void DataOrderingEngine::_reset()
{
    m_DataSet.clear();
    m_OrderedDataSet.clear();
    m_AdjacencyMatrix.clear();
    m_OrderingIndexes.clear();
    m_InversionFlags.clear();
    m_WordSize.reset();
}

std::optional<OrderingIndex> DataOrderingEngine::_initGreedyMinSimplified(bool inversionAllowed,
                                                                          StatusFlags& wordAlreadyAddedStatuses)
{
    std::optional<OrderingIndex> currentWordIndex;

    const size_t c_DataSetSize{m_DataSet.size()};
    const size_t c_OrderingIndexesSize{m_OrderingIndexes.size()};
    const size_t c_InversionFlagsSize{m_InversionFlags.size()};

    assert(c_OrderingIndexesSize == c_DataSetSize);
    assert(c_InversionFlagsSize == c_DataSetSize);

    if (c_DataSetSize > 0 && c_OrderingIndexesSize == c_DataSetSize && c_InversionFlagsSize == c_DataSetSize)
    {
        wordAlreadyAddedStatuses.resize(c_DataSetSize, false);

        if (c_DataSetSize > 1)
        {
            // get the first two words (that have the minimum Hamming distance) and add them to ordered set
            std::optional<OrderingIndexesPair> minDistancePair;
            bool isInversionRequired{false};

            if (inversionAllowed)
            {
                isInversionRequired = _retrieveFirstTwoOrderedWordsUsingInversion(minDistancePair);
            }
            else
            {
                _retrieveFirstTwoOrderedWords(minDistancePair);
            }

            if (minDistancePair.has_value() && minDistancePair->first != minDistancePair->second &&
                minDistancePair->first < c_DataSetSize && minDistancePair->second < c_DataSetSize)
            {
                m_OrderingIndexes[0] = minDistancePair->first;
                m_OrderingIndexes[1] = minDistancePair->second;
                m_InversionFlags[0] = false;
                m_InversionFlags[1] = isInversionRequired;
                wordAlreadyAddedStatuses.at(m_OrderingIndexes[0]) = true;
                wordAlreadyAddedStatuses.at(m_OrderingIndexes[1]) = true;
                currentWordIndex = m_OrderingIndexes[1];
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            m_OrderingIndexes[0] = 0;
            m_InversionFlags[0] = false;
            wordAlreadyAddedStatuses[0] = true;
            currentWordIndex = 0;
        }
    }
    else
    {
        wordAlreadyAddedStatuses.clear();
    }

    return currentWordIndex;
}

void DataOrderingEngine::_retrieveFirstTwoOrderedWords(std::optional<OrderingIndexesPair>& minDistancePair) const
{
    do
    {
        if (!m_WordSize.has_value())
        {
            break;
        }

        const size_t c_DataSetSize{m_DataSet.size()};

        if (c_DataSetSize <= 1)
        {
            break;
        }

        if (m_AdjacencyMatrix.getNrOfRows() != c_DataSetSize || m_AdjacencyMatrix.getNrOfColumns() != c_DataSetSize)
        {
            assert(false);
            break;
        }

        HammingDistance currentDistance{_retrieveDistanceBetweenFirstTwoUnorderedWords()};

        if (!currentDistance.has_value() || m_WordSize < currentDistance)
        {
            assert(false);
            break;
        }

        matrix_size_t currentFirstWordIndex{0};
        matrix_size_t currentSecondWordIndex{1};
        const matrix_size_t c_PositiveDiagonalsCount{static_cast<matrix_size_t>(c_DataSetSize - 1)};

        for (matrix_size_t currentDiagNr{1}; currentDiagNr < c_PositiveDiagonalsCount; ++currentDiagNr)
        {
            for (AdjacencyMatrix::ConstDIterator it{m_AdjacencyMatrix.constDBegin(currentDiagNr)};
                 it != m_AdjacencyMatrix.constDEnd(currentDiagNr); ++it)
            {
                if (!it->has_value() || *it > m_WordSize)
                {
                    assert(false);
                    currentDiagNr = c_PositiveDiagonalsCount; // stop condition for the outer loop (fail fast)
                    break;
                }

                if (*it < currentDistance)
                {
                    currentDistance = *it;

                    // no need to check the validity of getRowNr() and getColumnNr() as the matrix is not empty and the
                    // iterators are not reverse
                    currentFirstWordIndex = *it.getRowNr();
                    currentSecondWordIndex = *it.getColumnNr();
                }
            }
        }

        minDistancePair = {currentFirstWordIndex, currentSecondWordIndex};
    } while (false);
}

bool DataOrderingEngine::_retrieveFirstTwoOrderedWordsUsingInversion(
    std::optional<OrderingIndexesPair>& minDistancePair) const
{
    bool areInverted{false};

    do
    {
        if (!m_WordSize.has_value())
        {
            break;
        }

        const size_t c_DataSetSize{m_DataSet.size()};

        if (c_DataSetSize <= 1)
        {
            break;
        }

        if (m_AdjacencyMatrix.getNrOfRows() != c_DataSetSize || m_AdjacencyMatrix.getNrOfColumns() != c_DataSetSize)
        {
            assert(false);
            break;
        }

        HammingDistance currentDistance{_retrieveDistanceBetweenFirstTwoUnorderedWords()};

        if (!currentDistance.has_value() || m_WordSize < currentDistance)
        {
            assert(false);
            break;
        }

        const HammingDistance c_FirstPairInvertedDistance{*m_WordSize - *currentDistance};

        if (c_FirstPairInvertedDistance < currentDistance)
        {
            currentDistance = c_FirstPairInvertedDistance;
            areInverted = true;
        }

        matrix_size_t currentFirstWordIndex{0};
        matrix_size_t currentSecondWordIndex{1};
        const matrix_size_t c_PositiveDiagonalsCount{static_cast<matrix_size_t>(c_DataSetSize - 1)};

        for (matrix_size_t currentDiagNr{1}; currentDiagNr < c_PositiveDiagonalsCount; ++currentDiagNr)
        {
            for (AdjacencyMatrix::ConstDIterator it{m_AdjacencyMatrix.constDBegin(currentDiagNr)};
                 it != m_AdjacencyMatrix.constDEnd(currentDiagNr); ++it)
            {
                if (!it->has_value() || *it > m_WordSize)
                {
                    assert(false);
                    currentDiagNr = c_PositiveDiagonalsCount; // stop condition for the outer loop (fail fast)
                    break;
                }

                // no need to check the validity of getRowNr() and getColumnNr() as the matrix is not empty and the
                // iterators are not reverse
                const matrix_size_t c_RowNr{*it.getRowNr()};
                const matrix_size_t c_ColumnNr{*it.getColumnNr()};

                if (*it < currentDistance)
                {
                    currentDistance = *it;
                    currentFirstWordIndex = c_RowNr;
                    currentSecondWordIndex = c_ColumnNr;
                    areInverted = false;
                }

                const HammingDistance c_CurrentPairInvertedDistance{*m_WordSize - **it};

                if (c_CurrentPairInvertedDistance < currentDistance)
                {
                    currentDistance = c_CurrentPairInvertedDistance;
                    currentFirstWordIndex = c_RowNr;
                    currentSecondWordIndex = c_ColumnNr;
                    areInverted = true;
                }
            }
        }

        minDistancePair = {currentFirstWordIndex, currentSecondWordIndex};
    } while (false);

    return areInverted;
}

void DataOrderingEngine::_retrieveNextOrderedWord(const StatusFlags& wordAlreadyAddedStatuses,
                                                  const std::optional<OrderingIndex>& currentWordIndex,
                                                  std::optional<OrderingIndex>& nextWordIndex) const
{
    do
    {
        const size_t c_DataSetSize{m_DataSet.size()};

        if (!m_WordSize.has_value() || m_AdjacencyMatrix.getNrOfRows() != c_DataSetSize ||
            m_AdjacencyMatrix.getNrOfColumns() != c_DataSetSize || wordAlreadyAddedStatuses.size() != c_DataSetSize ||
            !currentWordIndex.has_value() || currentWordIndex >= c_DataSetSize)
        {
            assert(false);
            break;
        }

        // start with current word, detemine next word
        nextWordIndex = currentWordIndex;

        // start by adding 1 to maximum distance (word size) to ensure one of the remaining words is added to ordered
        // set
        HammingDistance minHammingDistance{*m_WordSize + 1};

        AdjacencyMatrix::ConstZIterator currentWordIt{
            m_AdjacencyMatrix.getConstZIterator(*nextWordIndex, *nextWordIndex)};
        const std::optional<matrix_size_t> c_CurrentWordItRowNr{currentWordIt.getRowNr()};

        for (AdjacencyMatrix::ConstZIterator it{m_AdjacencyMatrix.constZRowBegin(*c_CurrentWordItRowNr)};
             it != m_AdjacencyMatrix.constZRowEnd(*c_CurrentWordItRowNr); ++it)
        {
            if (!it->has_value() || *it > m_WordSize)
            {
                assert(false);
                break;
            }

            // no need to check optional validity for it.getColumnNr() before getting its value (*) because the iterator
            // should be valid
            const matrix_size_t c_CheckedIndex{*it.getColumnNr()};

            if (it != currentWordIt && !wordAlreadyAddedStatuses[c_CheckedIndex] && *it < minHammingDistance)
            {
                minHammingDistance = *it;
                nextWordIndex = c_CheckedIndex;
            }
        }
    } while (false);
}

bool DataOrderingEngine::_retrieveNextOrderedWordUsingInversion(const StatusFlags& wordAlreadyAddedStatuses,
                                                                const std::optional<OrderingIndex>& currentWordIndex,
                                                                std::optional<OrderingIndex>& nextWordIndex) const
{
    bool isInvertedSuccessor{false};

    do
    {
        const size_t c_DataSetSize{m_DataSet.size()};

        if (!m_WordSize.has_value() || m_AdjacencyMatrix.getNrOfRows() != c_DataSetSize ||
            m_AdjacencyMatrix.getNrOfColumns() != c_DataSetSize || wordAlreadyAddedStatuses.size() != c_DataSetSize ||
            !currentWordIndex.has_value() || currentWordIndex >= c_DataSetSize)
        {
            assert(false);
            break;
        }

        // start with current word, detemine next word
        nextWordIndex = currentWordIndex;

        // start by adding 1 to maximum distance (word size) to ensure one of the remaining words is added to ordered
        // set
        HammingDistance minHammingDistance{*m_WordSize + 1};

        AdjacencyMatrix::ConstZIterator currentWordIt{
            m_AdjacencyMatrix.getConstZIterator(*nextWordIndex, *nextWordIndex)};
        const std::optional<matrix_size_t> c_CurrentWordItRowNr{currentWordIt.getRowNr()};

        // no need to check optional validity for it.getColumnNr() before getting its value (*) because the matrix is
        // presumed non-empty (see above check) and the iterator is not reverse
        for (AdjacencyMatrix::ConstZIterator it{m_AdjacencyMatrix.constZRowBegin(*c_CurrentWordItRowNr)};
             it != m_AdjacencyMatrix.constZRowEnd(*c_CurrentWordItRowNr); ++it)
        {
            if (!it->has_value() || *it > m_WordSize)
            {
                assert(false);
                break;
            }

            // no need to check optional validity for it.getColumnNr() before getting its value (*) because the iterator
            // should be valid
            const matrix_size_t c_CheckedIndex{*it.getColumnNr()};

            if (it != currentWordIt && !wordAlreadyAddedStatuses[c_CheckedIndex])
            {
                if (*it < minHammingDistance)
                {
                    minHammingDistance = *it;
                    nextWordIndex = c_CheckedIndex;
                    isInvertedSuccessor = false;
                }

                const HammingDistance c_InvertedHammingDistance{*m_WordSize - **it};

                if (c_InvertedHammingDistance < minHammingDistance)
                {
                    minHammingDistance = c_InvertedHammingDistance;
                    nextWordIndex = c_CheckedIndex;
                    isInvertedSuccessor = true;
                }
            }
        }
    } while (false);

    return isInvertedSuccessor;
}

HammingDistance DataOrderingEngine::_retrieveDistanceBetweenFirstTwoUnorderedWords() const
{
    HammingDistance startingDistance;

    const matrix_size_t currentFirstWordIndex{0};
    const matrix_size_t currentSecondWordIndex{1};
    const size_t c_DataSetSize{m_DataSet.size()};

    if (c_DataSetSize > 1 && m_AdjacencyMatrix.getNrOfRows() == c_DataSetSize &&
        m_AdjacencyMatrix.getNrOfColumns() == c_DataSetSize)
    {
        startingDistance = m_AdjacencyMatrix.at(currentFirstWordIndex, currentSecondWordIndex);
        assert(startingDistance.has_value());
    }
    else
    {
        assert(false);
    }

    return startingDistance;
}

void DataOrderingEngine::_updateOrderedDataSet()
{
    const size_t c_DataSetSize{m_DataSet.size()};

    if (c_DataSetSize == m_OrderingIndexes.size() && c_DataSetSize == m_InversionFlags.size())
    {
        m_OrderedDataSet.clear();
        m_OrderedDataSet.reserve(c_DataSetSize);

        for (size_t currentWordIndex{0}; currentWordIndex < c_DataSetSize; ++currentWordIndex)
        {
            const bool& c_ShouldInvert{m_InversionFlags.at(currentWordIndex)};
            const OrderingIndex& c_OrderingIndex{m_OrderingIndexes.at(currentWordIndex)};

            if (c_OrderingIndex >= c_DataSetSize)
            {
                assert(false);
                break;
            }

            const DataWord& c_DataWord{m_DataSet.at(c_OrderingIndex)};

            m_OrderedDataSet.push_back(c_ShouldInvert ? Utilities::invertDataWord(c_DataWord) : c_DataWord);
        }
    }
    else
    {
        assert(false);
    }
}
