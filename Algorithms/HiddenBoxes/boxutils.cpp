#include <cassert>

#include "boxutils.h"

void retrieveFittingBoxes(const Matrix<matrix_size_t>& boxes, Matrix<matrix_size_t>& fittingBoxIndexes)
{
    // total number of boxes belonging to series
    const auto c_NrOfBoxes{boxes.getNrOfRows()};

    // row index of the box containing largest number of boxes that fit into each other (and into it) - initially the
    // first box in the lexicographically sorted series
    matrix_size_t maxFittingIndex{0};

    // fitting boxes count for each box (including the box)
    Matrix<matrix_size_t> fittingBoxesCounts{{c_NrOfBoxes, 1}, 1};

    // index (in lexicographically sorted series) of first preceding box that fits into current box; by default -1 (no
    // preceding fitting box)
    Matrix<std::optional<matrix_size_t>> prevFittingBoxIndexes{{c_NrOfBoxes, 1}, std::nullopt};
    const auto c_PrevFittingBoxIndexesColumnsCount{prevFittingBoxIndexes.getNrOfColumns()};

    for (matrix_size_t boxIndex{1}; boxIndex < c_NrOfBoxes; ++boxIndex)
    {
        const auto c_FittingBoxesCountsColumnsCount{fittingBoxesCounts.getNrOfColumns()};
        auto& currentFittingBoxesCount{fittingBoxesCounts.at(boxIndex / c_FittingBoxesCountsColumnsCount,
                                                             boxIndex % c_FittingBoxesCountsColumnsCount)};

        for (matrix_size_t prevBoxIndex{0}; prevBoxIndex < boxIndex; ++prevBoxIndex)
        {
            const auto& c_PrevFittingBoxesCount{fittingBoxesCounts.at(prevBoxIndex / c_FittingBoxesCountsColumnsCount,
                                                                      prevBoxIndex % c_FittingBoxesCountsColumnsCount)};
            auto& currentPrevFittingBoxIndex{prevFittingBoxIndexes.at(boxIndex / c_PrevFittingBoxIndexesColumnsCount,
                                                                      boxIndex % c_PrevFittingBoxIndexesColumnsCount)};

            if (boxFitsIntoBox(prevBoxIndex, boxIndex, boxes) &&
                (c_PrevFittingBoxesCount + 1 > currentFittingBoxesCount))
            {
                currentFittingBoxesCount = c_PrevFittingBoxesCount + 1;
                currentPrevFittingBoxIndex = prevBoxIndex;
            }
        }

        const auto& c_MaxFittingBoxesCount{fittingBoxesCounts.at(maxFittingIndex / c_FittingBoxesCountsColumnsCount,
                                                                 maxFittingIndex % c_FittingBoxesCountsColumnsCount)};

        if (currentFittingBoxesCount > c_MaxFittingBoxesCount)
        {
            maxFittingIndex = boxIndex;
        }
    }

    Matrix<matrix_size_t> recoveredIndexes{{1, 1}, maxFittingIndex};
    auto currentIndexToCheck{prevFittingBoxIndexes.at(maxFittingIndex / c_PrevFittingBoxIndexesColumnsCount,
                                                      maxFittingIndex % c_PrevFittingBoxIndexesColumnsCount)};

    while (currentIndexToCheck.has_value())
    {
        recoveredIndexes.insertColumn(0, *currentIndexToCheck);
        currentIndexToCheck = prevFittingBoxIndexes.at(*currentIndexToCheck / c_PrevFittingBoxIndexesColumnsCount,
                                                       *currentIndexToCheck % c_PrevFittingBoxIndexesColumnsCount);
    }

    fittingBoxIndexes = std::move(recoveredIndexes);
}

bool boxFitsIntoBox(matrix_size_t fittingBoxNumber, matrix_size_t includingBoxNumber, Matrix<matrix_size_t> boxes)
{
    assert(boxes.getNrOfColumns() > 1 && "Invalid dimensions for boxes");
    assert((fittingBoxNumber >= 0 && fittingBoxNumber < boxes.getNrOfRows()) && "Invalid box number (fitting box)");
    assert((includingBoxNumber >= 0 && includingBoxNumber < boxes.getNrOfRows()) &&
           "Invalid box number (including box)");

    bool fitsIntoBox{true};

    for (matrix_size_t column{0}; column < boxes.getNrOfColumns(); ++column)
    {
        if (boxes.at(fittingBoxNumber, column) >= boxes.at(includingBoxNumber, column))
        {
            fitsIntoBox = false;
            break;
        }
    }

    return fitsIntoBox;
}
