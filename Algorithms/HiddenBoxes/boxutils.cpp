#include <cassert>

#include "boxutils.h"

std::vector<matrix_size_t> retrieveFittingBoxes(const Matrix<matrix_size_t>& boxes)
{
    // total number of boxes belonging to series
    const matrix_size_t c_NrOfBoxes{boxes.getNrOfRows()};

    // row index of the box containing largest number of boxes that fit into each other (and into it) - initially the
    // first box in the lexicographically sorted series
    matrix_size_t maxFittingIndex{0};

    // fitting boxes count for each box (including the box)
    Matrix<matrix_size_t> fittingBoxesCounts{{c_NrOfBoxes, 1}, 1};

    // index (in lexicographically sorted series) of first preceding box that fits into current box; by default -1 (no
    // preceding fitting box)
    Matrix<std::optional<matrix_size_t>> prevFittingBoxIndexes{{c_NrOfBoxes, 1}, std::nullopt};
    const matrix_size_t c_PrevFittingBoxIndexesColumnsCount{prevFittingBoxIndexes.getNrOfColumns()};

    for (matrix_size_t boxIndex{1}; boxIndex < c_NrOfBoxes; ++boxIndex)
    {
        const matrix_size_t c_FittingBoxesCountsColumnsCount{fittingBoxesCounts.getNrOfColumns()};
        matrix_size_t& currentFittingBoxesCount{fittingBoxesCounts.at(boxIndex / c_FittingBoxesCountsColumnsCount,
                                                                      boxIndex % c_FittingBoxesCountsColumnsCount)};

        for (matrix_size_t prevBoxIndex{0}; prevBoxIndex < boxIndex; ++prevBoxIndex)
        {
            const matrix_size_t& c_PrevFittingBoxesCount{fittingBoxesCounts.at(
                prevBoxIndex / c_FittingBoxesCountsColumnsCount, prevBoxIndex % c_FittingBoxesCountsColumnsCount)};
            std::optional<matrix_size_t>& currentPrevFittingBoxIndex{prevFittingBoxIndexes.at(
                boxIndex / c_PrevFittingBoxIndexesColumnsCount, boxIndex % c_PrevFittingBoxIndexesColumnsCount)};

            if (boxFitsIntoBox(prevBoxIndex, boxIndex, boxes) &&
                (c_PrevFittingBoxesCount + 1 > currentFittingBoxesCount))
            {
                currentFittingBoxesCount = c_PrevFittingBoxesCount + 1;
                currentPrevFittingBoxIndex = prevBoxIndex;
            }
        }

        const matrix_size_t& c_MaxFittingBoxesCount{fittingBoxesCounts.at(
            maxFittingIndex / c_FittingBoxesCountsColumnsCount, maxFittingIndex % c_FittingBoxesCountsColumnsCount)};

        if (currentFittingBoxesCount > c_MaxFittingBoxesCount)
        {
            maxFittingIndex = boxIndex;
        }
    }

    std::vector<matrix_size_t> recoveredIndexes;

    // recovered indexes should be prepended to the found ones, yet they will be initially appended instead due to the
    // std::vector behavior/functionality; finally the vector will be reversed, see below
    recoveredIndexes.reserve(c_NrOfBoxes);
    recoveredIndexes.push_back(maxFittingIndex);

    std::optional<matrix_size_t> currentIndexToCheck{prevFittingBoxIndexes.at(
        maxFittingIndex / c_PrevFittingBoxIndexesColumnsCount, maxFittingIndex % c_PrevFittingBoxIndexesColumnsCount)};

    while (currentIndexToCheck.has_value())
    {
        recoveredIndexes.push_back(*currentIndexToCheck);
        currentIndexToCheck = prevFittingBoxIndexes.at(*currentIndexToCheck / c_PrevFittingBoxIndexesColumnsCount,
                                                       *currentIndexToCheck % c_PrevFittingBoxIndexesColumnsCount);
    }

    std::reverse(recoveredIndexes.begin(), recoveredIndexes.end());

    return recoveredIndexes;
}

bool boxFitsIntoBox(matrix_size_t fittingBoxNumber, matrix_size_t includingBoxNumber,
                    const Matrix<matrix_size_t>& boxes)
{
    const matrix_size_t c_BoxesRowsCount{boxes.getNrOfRows()};
    const matrix_size_t c_BoxesColumnsCount{boxes.getNrOfColumns()};

    assert(fittingBoxNumber < includingBoxNumber && "Invalid box number (fitting box)");
    assert(includingBoxNumber < c_BoxesRowsCount && "Invalid box number (including box)");

    // although the dimension 1 case can be handled, it is desirable that the minimum box dimension is 2, otherwise the
    // solution would become "uninteresting" (each box would fit into another one when the dimension is 1)
    assert(c_BoxesColumnsCount > 1 && "Invalid dimensions for boxes");

    bool fitsIntoBox{false};

    if (fittingBoxNumber < includingBoxNumber && includingBoxNumber < c_BoxesRowsCount)
    {
        fitsIntoBox = true;

        for (matrix_size_t column{0}; column < c_BoxesColumnsCount; ++column)
        {
            if (boxes.at(fittingBoxNumber, column) >= boxes.at(includingBoxNumber, column))
            {
                fitsIntoBox = false;
                break;
            }
        }
    }

    return fitsIntoBox;
}
