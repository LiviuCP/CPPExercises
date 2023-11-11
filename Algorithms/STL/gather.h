#ifndef GATHER_H
#define GATHER_H

#include "datautils.h"

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

template<typename DataType>
std::tuple<MatrixPoint<DataType>,
           MatrixPoint<DataType>,
           MatrixSizeType<DataType>> gatherMatrixElements(Matrix<DataType>& matrix,
                      MatrixPoint<DataType> startingPoint,
                      MatrixPoint<DataType> endingPoint,
                      MatrixPoint<DataType> gatheringPoint,
                      std::function<bool(const DataType&)>& predicate)
{
    MatrixPoint<DataType> gatheringStartingPoint{0, 0};
    MatrixPoint<DataType> gatheringEndingPoint{0, 0};
    MatrixSizeType<DataType> gatheredElementsCount{0};

    if (!matrix.isEmpty())
    {
        startingPoint.first = std::clamp(startingPoint.first, 0, matrix.getNrOfRows());
        startingPoint.second = std::clamp(startingPoint.second, 0, matrix.getNrOfColumns());
        endingPoint.first = std::clamp(endingPoint.first, 0, matrix.getNrOfRows());
        endingPoint.second = std::clamp(endingPoint.second, 0, matrix.getNrOfColumns());

        // get the input bounding rectangle
        MatrixSizeType<DataType> const c_StartingRowNr{std::min(startingPoint.first, endingPoint.first)};
        MatrixSizeType<DataType> const c_EndingRowNr{std::max(startingPoint.first, endingPoint.first)};
        MatrixSizeType<DataType> const c_StartingColumnNr{std::min(startingPoint.second, endingPoint.second)};
        MatrixSizeType<DataType> const c_EndingColumnNr{std::max(startingPoint.second, endingPoint.second)};

        gatheringPoint.first = std::clamp(gatheringPoint.first, c_StartingRowNr, c_EndingRowNr);
        gatheringPoint.second = std::clamp(gatheringPoint.second, c_StartingColumnNr, c_EndingColumnNr);

        // start from the opposite end of each interval in order to determine the min/max row/column numbers that limit the gathered elements
        MatrixSizeType<DataType> minGatheringRowNr{c_EndingRowNr};
        MatrixSizeType<DataType> maxGatheringRowNr{c_StartingRowNr};
        MatrixSizeType<DataType> minGatheringColumnNr{c_EndingColumnNr};
        MatrixSizeType<DataType> maxGatheringColumnNr{c_StartingColumnNr};

        for(auto rowNr{c_StartingRowNr}; rowNr < c_EndingRowNr; ++rowNr)
        {
            auto rowRangeStartIt{c_StartingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_StartingColumnNr)};
            auto rowRangeEndIt{c_EndingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_EndingColumnNr)};
            auto rowGatheringPointIt{gatheringPoint.second == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, gatheringPoint.second)};

            const auto&[gatheringRowStartIt, gatheringRowEndIt]{gatherSequenceElements(rowRangeStartIt, rowRangeEndIt, rowGatheringPointIt, predicate)};
            minGatheringColumnNr = std::min(minGatheringColumnNr, gatheringRowStartIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : gatheringRowStartIt.getColumnNr());
            maxGatheringColumnNr = std::max(maxGatheringColumnNr, gatheringRowEndIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : gatheringRowEndIt.getColumnNr());
            gatheredElementsCount += std::distance(gatheringRowStartIt, gatheringRowEndIt);
        }

        for(auto columnNr{c_StartingColumnNr}; columnNr < c_EndingColumnNr; ++columnNr)
        {
            auto columnRangeStartIt{c_StartingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_StartingRowNr, columnNr)};
            auto columnRangeEndIt{c_EndingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_EndingRowNr, columnNr)};
            auto columnGatheringPointIt{gatheringPoint.first == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(gatheringPoint.first, columnNr)};

            const auto&[gatheringColumnStartIt, gatheringColumnEndIt]{gatherSequenceElements(columnRangeStartIt, columnRangeEndIt, columnGatheringPointIt, predicate)};
            minGatheringRowNr = std::min(minGatheringRowNr, gatheringColumnStartIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : gatheringColumnStartIt.getRowNr());
            maxGatheringRowNr = std::max(maxGatheringRowNr, gatheringColumnEndIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : gatheringColumnEndIt.getRowNr());
        }

        // determine the output bounding rectangle (should be the gathering point if the input bounding rectangle is a row, column or single element)
        if (minGatheringRowNr != maxGatheringRowNr && minGatheringColumnNr != maxGatheringColumnNr)
        {
            gatheringStartingPoint = {minGatheringRowNr, minGatheringColumnNr};
            gatheringEndingPoint = {maxGatheringRowNr, maxGatheringColumnNr};
        }
        else
        {
            gatheringStartingPoint = gatheringPoint;
            gatheringEndingPoint = gatheringPoint;
        }
    }

    return {gatheringStartingPoint, gatheringEndingPoint, gatheredElementsCount};
}

#endif // GATHER_H
