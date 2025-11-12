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

    for (matrix_size_t boxIndex{1}; boxIndex < c_NrOfBoxes; ++boxIndex)
    {
        for (matrix_size_t prevBoxIndex{0}; prevBoxIndex < boxIndex; ++prevBoxIndex)
        {
            if (boxFitsIntoBox(prevBoxIndex, boxIndex, boxes) &&
                ((fittingBoxesCounts[prevBoxIndex] + 1) > fittingBoxesCounts[boxIndex]))
            {
                fittingBoxesCounts[boxIndex] = fittingBoxesCounts[prevBoxIndex] + 1;
                prevFittingBoxIndexes[boxIndex] = prevBoxIndex;
            }
        }

        if (fittingBoxesCounts[boxIndex] > fittingBoxesCounts[maxFittingIndex])
        {
            maxFittingIndex = boxIndex;
        }
    }

    Matrix<matrix_size_t> recoveredIndexes{{1, 1}, maxFittingIndex};
    auto currentIndexToCheck{prevFittingBoxIndexes[maxFittingIndex]};

    while (currentIndexToCheck.has_value())
    {
        recoveredIndexes.insertColumn(0, *currentIndexToCheck);
        currentIndexToCheck = prevFittingBoxIndexes[*currentIndexToCheck];
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
