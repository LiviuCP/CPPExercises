/* Each CPPExercises subproject that includes matrixutils.h should have following path setup in INCLUDEPATH (relative to the subproject dir):
   1) CPPExercises/External/Matrix/MatrixLib/Matrix
   2) CPPExercises/External/Matrix/MatrixLib/Utils
*/

#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

#include <fstream>
#include <cmath>

#include "datautils.h"

template<typename T>
std::ifstream& operator>>(std::ifstream& in, Matrix<T>& data);

template<typename T>
std::ofstream& operator<<(std::ofstream& out, const Matrix<T>& data);

template<typename T>
std::ifstream& operator>>(std::ifstream& in, Matrix<T>& data)
{
    bool success{false};

    Matrix<T> matrix{};

    int nrOfRows{0};
    int nrOfColumns{0};

    in >> nrOfRows;

    if (!in.fail() && nrOfRows > 0)
    {
        in >> nrOfColumns;

        if (!in.fail() && nrOfColumns > 0)
        {
            matrix.resize(nrOfRows, nrOfColumns);
            success = true;

            for (typename Matrix<T>::ZIterator it{matrix.zBegin()}; it != matrix.zEnd(); ++it)
            {
                in >> *it;

                if (in.fail())
                {
                    success = false;
                    break;
                }
            }

            if (success)
            {
                data = std::move(matrix);
            }
        }
    }

    // ensure an empty matrix is provided if reading has not been done successfully
    if (!success)
    {
        data.clear();
    }

    return in;
}

// this operator needs to be included (by including "matrixutils.h") otherwise the boolean value of the matrix will get printed
template<typename T>
std::ofstream& operator<<(std::ofstream& out, const Matrix<T>& data)
{
    for(int row{0}; row < data.getNrOfRows(); ++row)
    {
        for (typename Matrix<T>::ConstZIterator it{data.constZRowBegin(row)}; it != data.constZRowEnd(row); ++it)
        {
            out << *it << " ";
        }

        out << "\n";
    }

    return out;
}

/* Retrieves the row and column number of the matrix based on given diagonal index:
   - the diagonal index is included in the result as it might be clamped in case it exceeds the given bounds (0 and maximum number of elements)
   - instead of Matrix it would be sufficient to put the numberr of rows and columns as arguments, but the Matrix object is guaranteed to have valid dimensions so it's a safer option
     (besides the result of the diagonal index mapping to coordinates is supposed to be used on a real matrix)
   - However it should be noted that this mapping is actually matrix element type independent
   - null result for empty matrix, no exceptions triggered

   Diagonal indexes are counted starting with the top left corner (element (0, 0)) and ending with bottom right corner (e.g. (4, 5) for a 5 x 6 matrix) as given in the example:

    0  2  5  9 14 19
    1  4  8 13 18 23
    3  7 12 17 22 26
    6 11 16 21 25 28
   10 15 20 24 27 29

   Diagonal numbers are counted starting with the top-left corner (diagonal 0) and ending with the bottom-right corner (diagonal nr_of_rows + nr_of_columns - 2). For the above example:
    - diagonal 0: index 0
    - diagonal 1: indexes 1, 2
    ...
    - diagonal 4: indexes 10-14 (max size diagonal)
    - diagonal 5: indexes 15-19 (max size diagonal)
    ...
    - diagonal 8: indexes 27-28
    - diagonal 9: index 29

   There are three scenarios/cases that would be taken into account:
    - case 1: the diagonal index is on a diagonal that precedes the first maximum size diagonal (top-left area)
      - a second order equation is being used for retrieving the diagonal number: diag_nr^2 + diag_nr - 2 * diag_index = 0
      - the higher root is extracted as decimal value (the lower root would be negative and thus meaningless): diag_nr = (-1 + sqrt(1 + 8 * diag_index)) / 2
      - the equation result is (usually) decimal so it should be approximated with an integer that would either be the floor or the ceiling of the equation root (usually the floor)
        - if the int ceiling value solved the equation precisely and was different from the floor then this would be the required result
        - otherwise the floor yields the requested diagonal number
      - the equation has been elaborated based on the fact that the starting index of a diagonal from the top-left part equals the total number of elements located on the previous diagonals
        - the total number of elements preceding the diagonal number diag_nr is diag_nr * (diag_nr + 1) / 2, e.g. for diagonal number 3 (indexes 6, 7, 8, 9): 3 * 4 / 2 = 6 preceding elements
        - the index number of the first diagonal element equals the total number of elements from previous diagonals: first index on diagonal 3 is 6
        - this results in the above mentioned equation
        - the equation can be generalized and used for the other indexes of the diagonal too, but in that case a decimal number is obtained
          (which by using the above mentioned floor/ceiling approximations yields the required diagonal number)
    - case 2: the diagonal index is on a maximum size diagonal (center area)
        - use the fact that the maximum size diagonals have equal size
          - there can be one or more maximum diagonals
          - there can be only maximum diagonals for line, column or element matrixes
        - also use the total elements count of the preceding top-left diagonals
    - case 3: the diagonal index is on a diagonal following the maximum size diagonals (bottom-right area)
        - use symmetry to the top-left part, namely determine which is the symmetric top-left diagonal index
        - obtain the symmetric top-left coordinates and then by applying symmetry again (this time on coordinates) the required bottom-right row and column number would be obtained
*/
template<typename T>
std::pair<MatrixPoint, std::optional<matrix_diff_t>> mapDiagonalIndexToRowAndColumnNr(const Matrix<T>& matrix, matrix_diff_t diagonalIndex)
{
    std::pair<MatrixPoint, std::optional<matrix_diff_t>> result;

    if (!matrix.isEmpty())
    {
        auto&[coordinates, diagonalIndexToConvert]{result};
        auto&[rowNr, columnNr]{coordinates};

        const matrix_size_t c_NrOfRows{matrix.getNrOfRows()};
        const matrix_size_t c_NrOfColumns{matrix.getNrOfColumns()};
        const matrix_diff_t c_TotalElementsCount{static_cast<matrix_diff_t>(c_NrOfRows) * static_cast<matrix_diff_t>(c_NrOfColumns)};
        const matrix_size_t c_MaxDiagonalSize{std::min(c_NrOfRows, c_NrOfColumns)};

        const matrix_diff_t c_FirstMaxDiagonalNr{static_cast<matrix_diff_t>(c_MaxDiagonalSize - 1)};
        const matrix_diff_t c_ElementsCountBeforeFirstMaxDiagonal{c_FirstMaxDiagonalNr * (c_FirstMaxDiagonalNr + 1) / 2};

        // for this calculation symmetry is used: diagonals following max size diagonals are symmetric to diagonals before max size diagonals
        const matrix_diff_t c_ElementsCountIncludingMaxDiagonals{c_TotalElementsCount - c_ElementsCountBeforeFirstMaxDiagonal};

        diagonalIndexToConvert = std::clamp<matrix_diff_t>(diagonalIndex, 0, c_TotalElementsCount - 1);

        if (diagonalIndexToConvert < c_ElementsCountBeforeFirstMaxDiagonal)
        {
            // case 1: index is in the top-left diagonals (before max diagonals)
            const double c_DecimalDiagonalNr{(std::sqrt(1.0 + 8.0 * static_cast<double>(*diagonalIndexToConvert)) - 1.0) / 2.0};
            const matrix_diff_t c_DiagonalNumberFloor{static_cast<matrix_diff_t>(std::floor(c_DecimalDiagonalNr))};
            const matrix_diff_t c_DiagonalNumberCeiling{static_cast<matrix_diff_t>(std::ceil(c_DecimalDiagonalNr))};

            /* Proving of the above mentioned equation for integer roots:
               - this verification is required in order to account for any rounding errors resulting in the calculation of c_DecimalDiagonalNr
               - more specifically a correction is required for the case when c_DecimalDiagonalNr should be precisely an integer according to the equation (e.g. 3),
                 however due to a rounding error a decimal number slightly less then the required one (e.g. 2.999998) is obtained (which would yield 2 when using floor)
            */
            const matrix_diff_t c_DoubledCeilingIncremented{2 * c_DiagonalNumberCeiling + 1};
            const bool c_ShouldUseCeiling{c_DoubledCeilingIncremented * c_DoubledCeilingIncremented == (8 * *diagonalIndexToConvert + 1)};

            const matrix_diff_t c_DiagonalNr{c_ShouldUseCeiling ? c_DiagonalNumberCeiling : c_DiagonalNumberFloor};
            const matrix_size_t c_DiagonalSize{static_cast<matrix_size_t>(c_DiagonalNr + 1)};
            const matrix_diff_t c_ElementsCountBeforeDiagonal{c_DiagonalNr > 0 ? (c_DiagonalNr * (c_DiagonalNr + 1) / 2) : 0};
            const matrix_size_t c_RelativeDiagonalIndex{static_cast<matrix_size_t>(*diagonalIndexToConvert - c_ElementsCountBeforeDiagonal)};
            rowNr = c_DiagonalSize - 1 - c_RelativeDiagonalIndex; // relative index should be between 0 and diagonal size - 1, so no overflow risk
            columnNr = c_RelativeDiagonalIndex;
        }
        else if (diagonalIndexToConvert < c_ElementsCountIncludingMaxDiagonals)
        {
            // case 2: index is in the max size diagonals
            const matrix_diff_t c_DiagonalNr{c_FirstMaxDiagonalNr + (*diagonalIndexToConvert - c_ElementsCountBeforeFirstMaxDiagonal) / static_cast<matrix_diff_t>(c_MaxDiagonalSize)};
            const matrix_size_t c_RelativeDiagonalIndex{static_cast<matrix_size_t>(*diagonalIndexToConvert - c_ElementsCountBeforeFirstMaxDiagonal) % c_MaxDiagonalSize}; // index relative to containing diagonal
            const matrix_size_t c_RelativeMaxDiagonalNr{static_cast<matrix_size_t>(c_DiagonalNr - c_FirstMaxDiagonalNr)}; // max diagonal number relative to the number of max diagonals
            rowNr = c_NrOfRows <= c_NrOfColumns ? c_NrOfRows - 1 - c_RelativeDiagonalIndex : c_NrOfColumns + c_RelativeMaxDiagonalNr - 1 - c_RelativeDiagonalIndex;
            columnNr = c_NrOfRows <= c_NrOfColumns ? c_RelativeMaxDiagonalNr + c_RelativeDiagonalIndex : c_RelativeDiagonalIndex;
        }
        else
        {
            // case 3: index is in the bottom-right diagonals (after the max diagonals)
            const matrix_diff_t c_SymmetricalDiagonalIndex{c_TotalElementsCount - 1 - *diagonalIndexToConvert};
            const auto c_SymmetricalResult{mapDiagonalIndexToRowAndColumnNr(matrix, c_SymmetricalDiagonalIndex)};
            const auto&[symmetricalCoordinates, _]{c_SymmetricalResult};
            const auto&[symmetricalRowNr, symmetricalColumnNr]{symmetricalCoordinates};
            rowNr = c_NrOfRows - 1 - *symmetricalRowNr;
            columnNr = c_NrOfColumns - 1 - *symmetricalColumnNr;
        }
    }

    return result;
}

/* This function retrieves the diagonal index based on the provided coordinates (row and column number)
   - same considerations as for previous function regarding usage of Matrix and type-independent relationship between diagonal index and coordinates
   - similar to the previous function, for empty matrixes a null value is returned
*/
template<typename T>
std::pair<std::optional<matrix_diff_t>, MatrixPoint> mapRowAndColumnNrToDiagonalIndex(const Matrix<T>& matrix, MatrixPoint rowAndColumnNr)
{
    std::pair<std::optional<matrix_diff_t>, MatrixPoint> result;

    if (!matrix.isEmpty() && rowAndColumnNr.first.has_value() && rowAndColumnNr.second.has_value())
    {
        auto&[diagonalIndex, rowAndColumnNrToConvert]{result};
        auto&[rowNr, columnNr]{rowAndColumnNrToConvert};

        const matrix_size_t c_NrOfRows{matrix.getNrOfRows()};
        const matrix_size_t c_NrOfColumns{matrix.getNrOfColumns()};
        const matrix_diff_t c_TotalElementsCount{static_cast<matrix_diff_t>(c_NrOfRows) * static_cast<matrix_diff_t>(c_NrOfColumns)};
        const matrix_diff_t c_NrOfDiagonals{static_cast<matrix_diff_t>(c_NrOfRows) + static_cast<matrix_diff_t>(c_NrOfColumns) - 1};
        const matrix_size_t c_MaxDiagonalSize{std::min(c_NrOfRows, c_NrOfColumns)};

        rowNr = std::clamp<matrix_size_t>(*rowAndColumnNr.first, 0, c_NrOfRows - 1);
        columnNr = std::clamp<matrix_size_t>(*rowAndColumnNr.second, 0, c_NrOfColumns - 1);

        const matrix_diff_t c_DiagonalNr{static_cast<matrix_diff_t>(*rowNr) + static_cast<matrix_diff_t>(*columnNr)};
        const matrix_size_t c_RelativeDiagonalIndex{std::min(c_NrOfRows - 1 - *rowNr, *columnNr)};
        const matrix_diff_t c_FirstMaxDiagonalNr{static_cast<matrix_diff_t>(c_MaxDiagonalSize - 1)};
        const matrix_diff_t c_NrOfMaxSizeDiagonals{(c_NrOfDiagonals - 2 * c_FirstMaxDiagonalNr)};
        const matrix_diff_t c_LastMaxDiagonalNr{c_FirstMaxDiagonalNr + c_NrOfMaxSizeDiagonals - 1};

        if (c_DiagonalNr < c_FirstMaxDiagonalNr)
        {
            const matrix_diff_t c_ElementsCountBeforeDiagonal{c_DiagonalNr * (c_DiagonalNr + 1) / 2};
            diagonalIndex = c_ElementsCountBeforeDiagonal + static_cast<matrix_diff_t>(c_RelativeDiagonalIndex);
        }
        else if (c_DiagonalNr <= c_LastMaxDiagonalNr)
        {
            const matrix_diff_t c_ElementsCountBeforeFirstMaxDiagonal{c_FirstMaxDiagonalNr * (c_FirstMaxDiagonalNr + 1) / 2};
            const matrix_diff_t c_ElementsCountOnPrecedingMaxDiagonals{(c_DiagonalNr - c_FirstMaxDiagonalNr) * static_cast<matrix_diff_t>(c_MaxDiagonalSize)};
            diagonalIndex = c_ElementsCountBeforeFirstMaxDiagonal + c_ElementsCountOnPrecedingMaxDiagonals + static_cast<matrix_diff_t>(c_RelativeDiagonalIndex);
        }
        else
        {
            const matrix_size_t c_SymmetricalRowNr{c_NrOfRows - 1 - *rowNr};
            const matrix_size_t c_SymmetricalColumnNr{c_NrOfColumns - 1 - *columnNr};
            const auto c_SymmetricalResult{mapRowAndColumnNrToDiagonalIndex(matrix, {c_SymmetricalRowNr, c_SymmetricalColumnNr})};
            const auto&[symmetricalDiagonalIndex, _]{c_SymmetricalResult};
            diagonalIndex = c_TotalElementsCount - 1 - *symmetricalDiagonalIndex;
        }
    }

    return result;
}

#endif // MATRIXUTILS_H
