/* The class performs ascending lexicographical sorting (ordering of rows) of an integer matrix:
   - sorting it performed with/without prior sorting of row elements
   - sorting is performed ascending (both row sorting and lexical sorting)
   - a column matrix with the original row numbers is provided
*/

#pragma once

#include <algorithm>
#include <vector>

#include "matrix.h"

template <std::integral T> class LexicographicalSorter
{
public:
    static std::vector<matrix_size_t> sort(Matrix<T>& data, bool sortingPerRowRequired);

private:
    LexicographicalSorter() = delete; // class is fully static, no need to instantiate objects

    static void _doLexicographicalSort();

    static Matrix<T> s_Data;
    static std::vector<matrix_size_t> s_OriginalRowNumbers;
};

template <std::integral T>
std::vector<matrix_size_t> LexicographicalSorter<T>::sort(Matrix<T>& data, bool sortingPerRowRequired)
{
    s_OriginalRowNumbers.clear();

    s_Data = data;
    const matrix_size_t c_NrOfRows{s_Data.getNrOfRows()};

    if (c_NrOfRows > 0)
    {
        s_OriginalRowNumbers.reserve(c_NrOfRows);

        for (typename Matrix<T>::ConstNIterator it{s_Data.constNColumnBegin(0)}; it != s_Data.constNColumnEnd(0); ++it)
        {
            s_OriginalRowNumbers.push_back(*it.getRowNr());
        }

        if (sortingPerRowRequired)
        {
            for (auto rowNr{0}; rowNr < c_NrOfRows; ++rowNr)
            {
                std::sort(s_Data.zRowBegin(rowNr), s_Data.zRowEnd(rowNr));
            }
        }

        _doLexicographicalSort();

        data = std::move(s_Data);
    }

    return s_OriginalRowNumbers;
}

template <std::integral T> void LexicographicalSorter<T>::_doLexicographicalSort()
{
    const matrix_size_t c_NrOfRows{s_Data.getNrOfRows()};

    if (c_NrOfRows > 0 && c_NrOfRows == s_OriginalRowNumbers.size())
    {
        // for the moment bubble sort is used for simplicity reasons (to be replaced with a more efficient algorithm for
        // larger amounts of data)
        bool sortingRequired{true};

        while (sortingRequired)
        {
            sortingRequired = false;

            for (auto rowNr{0}; rowNr < c_NrOfRows - 1; ++rowNr)
            {
                if (!std::lexicographical_compare(s_Data.constZRowBegin(rowNr), s_Data.constZRowEnd(rowNr),
                                                  s_Data.constZRowBegin(rowNr + 1), s_Data.constZRowEnd(rowNr + 1)))
                {
                    const auto beginIt{s_OriginalRowNumbers.begin()};

                    std::iter_swap(beginIt + rowNr, beginIt + rowNr + 1);
                    s_Data.swapRows(rowNr, rowNr + 1);
                    sortingRequired = true;
                }
            }
        }
    }
    else
    {
        assert(false);
    }
}

template <std::integral T> Matrix<T> LexicographicalSorter<T>::s_Data{};

template <std::integral T> std::vector<matrix_size_t> LexicographicalSorter<T>::s_OriginalRowNumbers{};
