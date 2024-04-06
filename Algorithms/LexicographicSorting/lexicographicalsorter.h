/* The class performs ascending lexicographical sorting (ordering of rows) of an integer matrix:
   - sorting it performed with/without prior sorting of row elements
   - sorting is performed ascending (both row sorting and lexical sorting)
   - a column matrix with the original row numbers is provided
*/

#ifndef LEXICOGRAPHICALSORTER_H
#define LEXICOGRAPHICALSORTER_H

#include <algorithm>

#include "matrix.h"

template<typename T>
class LexicographicalSorter
{
public:
    static bool sort(Matrix<T>& data, Matrix<int>& originalRowNumbers, bool sortingPerRowRequired);

private:
    LexicographicalSorter() = delete; // class is fully static, no need to instantiate objects

    static void _doLexicographicalSort();

    static Matrix<T> sData;
    static Matrix<int> sOriginalRowNumbers;
};

template<typename T>
bool LexicographicalSorter<T>::sort(Matrix<T>& data, Matrix<int>& originalRowNumbers, bool sortingPerRowRequired)
{
    bool success{false};
    sData = data;
    const int c_NrOfRows{sData.getNrOfRows()};

    if (c_NrOfRows > 0)
    {
        sOriginalRowNumbers.resize(c_NrOfRows, 1);

        for (Matrix<int>::NIterator it{sOriginalRowNumbers.nBegin()}; it != sOriginalRowNumbers.nEnd(); ++it)
        {
            *it = it.getRowNr();
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

template<typename T>
void LexicographicalSorter<T>::_doLexicographicalSort()
{
    const int c_NrOfRows{sData.getNrOfRows()};

    // for the moment bubble sort is used for simplicity reasons (to be replaced with a more efficient algorithm for larger amounts of data)
    bool sortingRequired{true};

    while (sortingRequired)
    {
        sortingRequired = false;

        for (int row{0}; row < c_NrOfRows - 1; ++row)
        {
            if (!std::lexicographical_compare(sData.constZRowBegin(row), sData.constZRowEnd(row), sData.constZRowBegin(row+1), sData.constZRowEnd(row+1)))
            {
                sData.swapRows(row, row+1);
                sOriginalRowNumbers.swapRows(row, row+1);
                sortingRequired = true;
            }
        }
    }
}

template<typename T>
Matrix<T> LexicographicalSorter<T>::sData{};

template<typename T>
Matrix<int> LexicographicalSorter<T>::sOriginalRowNumbers{};

#endif // LEXICOGRAPHICALSORTER_H
