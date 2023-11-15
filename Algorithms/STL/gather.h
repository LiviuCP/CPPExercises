#ifndef GATHER_H
#define GATHER_H

#include "datautils.h"

/* Unidimensional gathering:
   - elements from a sequence are gathered around a chosen position (gathering point) by using a unary predicate
   - generic implementation using bidirectional iterators
*/

template<typename BidirectionalIt, typename Predicate>
std::pair<BidirectionalIt, BidirectionalIt> gatherSequenceElements(BidirectionalIt startIt, BidirectionalIt endIt, BidirectionalIt gatheringPointIt, Predicate predicate)
{
    BidirectionalIt gatheringStartIt{startIt};
    BidirectionalIt gatheringEndIt{startIt};

    if (std::distance(startIt, endIt) > 0)
    {
        gatheringPointIt = std::clamp<BidirectionalIt>(gatheringPointIt, startIt, endIt,
                                                       [](const BidirectionalIt& first, const BidirectionalIt& second) {return std::distance(first, second) > 0;});
        gatheringStartIt = std::stable_partition(startIt, gatheringPointIt, [predicate](const auto& element) {return !predicate(element);});
        gatheringEndIt = std::stable_partition(gatheringPointIt, endIt, predicate);
    }

    return {gatheringStartIt, gatheringEndIt};
}

/* Bidimensional gathering:
   - elements from a Matrix gathered around a chosen point by using a unary predicate
   - "specialized" implementation, so called Matrix "points" used instead of iterators; unidimensional gather also used as building block
   - gathering performed row-by-row by using the y coordinate of the gathering point; the result is further gathered column-by-column by using the x coordinate of the gathering point
*/

template<typename DataType>
std::tuple<MatrixPoint<DataType>,
           MatrixPoint<DataType>,
           MatrixSizeType<DataType>> gatherMatrixElements(Matrix<DataType>& matrix,
                      MatrixPoint<DataType> startingPoint,
                      MatrixPoint<DataType> endingPoint,
                      MatrixPoint<DataType> gatheringPoint,
                      std::function<bool(const DataType&)>& predicate)
{
    MatrixPoint<DataType> innerRectStartingPoint{0, 0};
    MatrixPoint<DataType> innerRectEndingPoint{0, 0};
    MatrixSizeType<DataType> gatheredElementsCount{0};

    if (!matrix.isEmpty())
    {
        // normalize the starting/ending points to the matrix dimensions in order to get valid input for determining the outer bounding rectangle
        startingPoint.first = std::clamp(startingPoint.first, 0, matrix.getNrOfRows());
        startingPoint.second = std::clamp(startingPoint.second, 0, matrix.getNrOfColumns());
        endingPoint.first = std::clamp(endingPoint.first, 0, matrix.getNrOfRows());
        endingPoint.second = std::clamp(endingPoint.second, 0, matrix.getNrOfColumns());

        // get the outer bounding rectangle from which elements are being gathered (based on starting/ending points)
        MatrixSizeType<DataType> const c_OuterRectStartingRowNr{std::min(startingPoint.first, endingPoint.first)};
        MatrixSizeType<DataType> const c_OuterRectEndingRowNr{std::max(startingPoint.first, endingPoint.first)};
        MatrixSizeType<DataType> const c_OuterRectStartingColumnNr{std::min(startingPoint.second, endingPoint.second)};
        MatrixSizeType<DataType> const c_OuterRectEndingColumnNr{std::max(startingPoint.second, endingPoint.second)};

        // gathering point should belong to the outer bounding rectangle (and finally to the resulting inner rectangle)
        gatheringPoint.first = std::clamp(gatheringPoint.first, c_OuterRectStartingRowNr, c_OuterRectEndingRowNr);
        gatheringPoint.second = std::clamp(gatheringPoint.second, c_OuterRectStartingColumnNr, c_OuterRectEndingColumnNr);

        // start from the ending (maximum) outer rectangle parameter to get the corresponding starting (minimum) inner rectangle parameter (and vice-versa)
        MatrixSizeType<DataType> innerRectStartingRowNr{c_OuterRectEndingRowNr};
        MatrixSizeType<DataType> innerRectEndingRowNr{c_OuterRectStartingRowNr};
        MatrixSizeType<DataType> innerRectStartingColumnNr{c_OuterRectEndingColumnNr};
        MatrixSizeType<DataType> innerRectEndingColumnNr{c_OuterRectStartingColumnNr};

        for(MatrixSizeType<DataType> rowNr{c_OuterRectStartingRowNr}; rowNr < c_OuterRectEndingRowNr; ++rowNr)
        {
            auto rowGatheringRangeStartIt{c_OuterRectStartingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_OuterRectStartingColumnNr)};
            auto rowGatheringRangeEndIt{c_OuterRectEndingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_OuterRectEndingColumnNr)};
            auto rowGatheringPointIt{gatheringPoint.second == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, gatheringPoint.second)};

            const auto&[rowGatheredRangeStartIt, rowGatheredRangeEndIt]{gatherSequenceElements(rowGatheringRangeStartIt, rowGatheringRangeEndIt, rowGatheringPointIt, predicate)};
            innerRectStartingColumnNr = std::min(innerRectStartingColumnNr, rowGatheredRangeStartIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : rowGatheredRangeStartIt.getColumnNr());
            innerRectEndingColumnNr = std::max(innerRectEndingColumnNr, rowGatheredRangeEndIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : rowGatheredRangeEndIt.getColumnNr());
            gatheredElementsCount += std::distance(rowGatheredRangeStartIt, rowGatheredRangeEndIt); // elements are counted only once (during per-row gathering)
        }

        for(MatrixSizeType<DataType> columnNr{c_OuterRectStartingColumnNr}; columnNr < c_OuterRectEndingColumnNr; ++columnNr)
        {
            auto columnGatheringRangeStartIt{c_OuterRectStartingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_OuterRectStartingRowNr, columnNr)};
            auto columnGatheringRangeEndIt{c_OuterRectEndingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_OuterRectEndingRowNr, columnNr)};
            auto columnGatheringPointIt{gatheringPoint.first == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(gatheringPoint.first, columnNr)};

            const auto&[columnGatheredRangeStartIt, columnGatheredRangeEndIt]{gatherSequenceElements(columnGatheringRangeStartIt, columnGatheringRangeEndIt, columnGatheringPointIt, predicate)};
            innerRectStartingRowNr = std::min(innerRectStartingRowNr, columnGatheredRangeStartIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : columnGatheredRangeStartIt.getRowNr());
            innerRectEndingRowNr = std::max(innerRectEndingRowNr, columnGatheredRangeEndIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : columnGatheredRangeEndIt.getRowNr());
        }

        // determine the inner bounding rectangle (minimum bounding rectangle containing all gathered elements) - contained within or at most equal to the outer rectangle
        if (innerRectStartingRowNr != innerRectEndingRowNr && innerRectStartingColumnNr != innerRectEndingColumnNr)
        {
            innerRectStartingPoint = {innerRectStartingRowNr, innerRectStartingColumnNr};
            innerRectEndingPoint = {innerRectEndingRowNr, innerRectEndingColumnNr};
        }
        else
        {
            // the inner rectangle decays to the gathering point if the starting and ending point are on the same row/column
            innerRectStartingPoint = gatheringPoint;
            innerRectEndingPoint = gatheringPoint;
        }
    }

    return {innerRectStartingPoint, innerRectEndingPoint, gatheredElementsCount};
}

#endif // GATHER_H
