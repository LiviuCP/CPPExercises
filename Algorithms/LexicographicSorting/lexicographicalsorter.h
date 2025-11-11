/* The class performs ascending lexicographical sorting (ordering of rows) of an integer matrix:
   - sorting it performed with/without prior sorting of row elements
   - sorting is performed ascending (both row sorting and lexical sorting)
   - a column matrix with the original row numbers is provided
*/

#pragma once

#include <algorithm>

#include "matrix.h"

template<std::integral T>
class LexicographicalSorter
{
public:
    static bool sort(Matrix<T>& data, Matrix<matrix_size_t>& originalRowNumbers, bool sortingPerRowRequired);

private:
    LexicographicalSorter() = delete; // class is fully static, no need to instantiate objects

    static void _doLexicographicalSort();

    static Matrix<T> sData;
    static Matrix<matrix_size_t> sOriginalRowNumbers;
};

template<std::integral T>
bool LexicographicalSorter<T>::sort(Matrix<T>& data, Matrix<matrix_size_t>& originalRowNumbers, bool sortingPerRowRequired)
{
    bool success{false};
    sData = data;
    const matrix_size_t c_NrOfRows{sData.getNrOfRows()};

    if (c_NrOfRows > 0)
    {
        sOriginalRowNumbers.resize(c_NrOfRows, 1);

        for (typename Matrix<matrix_size_t>::NIterator it{sOriginalRowNumbers.nBegin()}; it != sOriginalRowNumbers.nEnd(); ++it)
        {
            *it = *it.getRowNr();
        }

        if (sortingPerRowRequired)
        {
            for (auto row{0}; row < c_NrOfRows; ++row)
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

template<std::integral T>
void LexicographicalSorter<T>::_doLexicographicalSort()
{
    const matrix_size_t c_NrOfRows{sData.getNrOfRows()};
    assert(c_NrOfRows > 0);

    if (c_NrOfRows > 0)
    {
        // for the moment bubble sort is used for simplicity reasons (to be replaced with a more efficient algorithm for larger amounts of data)
        bool sortingRequired{true};

        while (sortingRequired)
        {
            sortingRequired = false;

            for (auto row{0}; row < c_NrOfRows - 1; ++row)
            {
                if (!std::lexicographical_compare(sData.constZRowBegin(row), sData.constZRowEnd(row), sData.constZRowBegin(row + 1), sData.constZRowEnd(row + 1)))
                {
                    sData.swapRows(row, row+1);
                    sOriginalRowNumbers.swapRows(row, row+1);
                    sortingRequired = true;
                }
            }
        }
    }
}

template<std::integral T>
Matrix<T> LexicographicalSorter<T>::sData{};

template<std::integral T>
Matrix<matrix_size_t> LexicographicalSorter<T>::sOriginalRowNumbers{};
