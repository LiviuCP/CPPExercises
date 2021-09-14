#include <cassert>

#include "lexicographicalsorter.h"
#include "boxutils.h"

void retrieveFittingBoxes(const Matrix<int>& boxes, Matrix<int>& fittingBoxIndexes)
{
    // total number of boxes belonging to series
    const int c_NrOfBoxes{boxes.getNrOfRows()};

    // row index of the box containing largest number of boxes that fit into each other (and into it) - initially the first box in the lexicographically sorted series
    int maxFittingIndex{0};

    // fitting boxes count for each box (including the box)
    Matrix<int> fittingBoxesCounts{c_NrOfBoxes, 1, 1};

    // index (in lexicographically sorted series) of first preceding box that fits into current box; by default -1 (no preceding fitting box)
    Matrix<int> prevFittingBoxIndexes{c_NrOfBoxes, 1, -1};

    for (int boxIndex{1}; boxIndex < c_NrOfBoxes; ++boxIndex)
    {
        for (int prevBoxIndex{0}; prevBoxIndex < boxIndex; ++prevBoxIndex)
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

    Matrix<int> recoveredIndexes{1, 1, maxFittingIndex};
    int currentIndexToCheck{prevFittingBoxIndexes[maxFittingIndex]};

    while(currentIndexToCheck != -1)
    {
        recoveredIndexes.insertColumn(0, currentIndexToCheck);
        currentIndexToCheck = prevFittingBoxIndexes[currentIndexToCheck];
    }

    fittingBoxIndexes = std::move(recoveredIndexes);
}

bool boxFitsIntoBox(int fittingBoxNumber, int includingBoxNumber, Matrix<int> boxes)
{
    assert(boxes.getNrOfColumns() > 1 && "Invalid dimensions for boxes");
    assert((fittingBoxNumber >= 0 && fittingBoxNumber < boxes.getNrOfRows()) && "Invalid box number (fitting box)");
    assert((includingBoxNumber >= 0 && includingBoxNumber < boxes.getNrOfRows()) && "Invalid box number (including box)");

    bool fitsIntoBox{true};

    for (int column{0}; column < boxes.getNrOfColumns(); ++column)
    {
        if (boxes.at(fittingBoxNumber, column) >= boxes.at(includingBoxNumber, column))
        {
            fitsIntoBox = false;
            break;
        }
    }

    return fitsIntoBox;
}
