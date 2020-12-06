#include <cassert>
#include <algorithm>

#include "lexicographicalsorter.h"

bool LexicographicalSorter::sort(Matrix<int>& data, Matrix<int>& originalRowNumbers, bool sortingPerRowRequired)
{
    bool success{false};
    sData = data;
    const int c_NrOfRows{sData.getNrOfRows()};

    if (c_NrOfRows > 0)
    {
        sOriginalRowNumbers.resize(c_NrOfRows, 1);

        for (Matrix<int>::NIterator it{sOriginalRowNumbers.nBegin()}; it != sOriginalRowNumbers.nEnd(); ++it)
        {
            *it = it.getCurrentRowNr();
        }

        if (sortingPerRowRequired)
        {
            for (int row{0}; row < c_NrOfRows; ++row)
            {
                std::sort(sData.zRowBegin(row), sData.zRowEnd(row));
            }
        }

        _doLexicographicalSort();

        data = std::move(sData);
        originalRowNumbers = std::move(sOriginalRowNumbers);
        success = true;
    }

    return success;
}

void LexicographicalSorter::_doLexicographicalSort()
{
    const int c_NrOfRows{sData.getNrOfRows()};

    // for the moment bubble sort is used for simplicity reasons
    bool sortingRequired{true};

    while (sortingRequired)
    {
        sortingRequired = false;

        for (int row{0}; row < c_NrOfRows - 1; ++row)
        {
            if (!std::lexicographical_compare(sData.constZRowBegin(row), sData.constZRowEnd(row), sData.constZRowBegin(row+1), sData.constZRowEnd(row+1)))
            {
                sData.swapRows(row, sData, row+1);
                sOriginalRowNumbers.swapRows(row, sOriginalRowNumbers, row+1);
                sortingRequired = true;
            }
        }
    }
}

Matrix<int> LexicographicalSorter::sData{};
Matrix<int> LexicographicalSorter::sOriginalRowNumbers{};
