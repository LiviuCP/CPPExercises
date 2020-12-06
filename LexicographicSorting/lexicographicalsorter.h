/* The class performs ascending lexicographical sorting (ordering of rows) of an integer matrix:
   - sorting it performed with/without prior sorting of row elements
   - sorting is performed ascending (both row sorting and lexical sorting)
   - a column matrix with the original row numbers is provided
*/

#ifndef LEXICOGRAPHICALSORTER_H
#define LEXICOGRAPHICALSORTER_H

#include "matrix.h"

class LexicographicalSorter
{
public:
    static bool sort(Matrix<int>& data, Matrix<int> &originalRowNumbers, bool sortingPerRowRequired);

private:
    LexicographicalSorter() = delete; // class is fully static, no need to instantiate objects

    static void _doLexicographicalSort();

    static Matrix<int> sData;
    static Matrix<int> sOriginalRowNumbers;
};

#endif // LEXICOGRAPHICALSORTER_H
