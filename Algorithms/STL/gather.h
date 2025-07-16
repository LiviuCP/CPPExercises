#ifndef GATHER_H
#define GATHER_H

#include "datautils.h"

/* Unidimensional gathering:
   - elements from a sequence are gathered around a chosen position (gathering point) by using a unary predicate
   - generic implementation using bidirectional iterators
   - for non-random-access bidirectional iterators it is the responsibility of the user to ensure all three argument iterators are valid and in-order (otherwise undefined behavior might occur):
        - the start iterator should point to an element that is placed BEFORE the one pointed by the end iterator (relative to the incrementation direction)
        - the gathering point iterator should be between the other two iterators or at most equal to either of them
   - for random iterators the behavior is defined as follows:
        - if the start iterator points to an element placed after the end iterator (relative to incrementation direction) no gathering takes place; the returned result
          is a pair of iterators that are equal to the start iterator
        - the algorithm clamps the value of the gathering point iterator to ensure it stands between the start and end iterators
          (only if the start/end iterators point to different positions and are in increasing order, otherwise handling occurs as in previous point)
*/

template<typename BidirectionalIt, typename Predicate>
std::pair<BidirectionalIt, BidirectionalIt> gatherSequenceElements(BidirectionalIt startIt, BidirectionalIt endIt, BidirectionalIt gatheringPointIt, Predicate predicate)
{
    using category = typename std::iterator_traits<BidirectionalIt>::iterator_category;
    static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, category>);

    BidirectionalIt gatheringStartIt{startIt};
    BidirectionalIt gatheringEndIt{startIt};

    if (std::distance(startIt, endIt) > 0)
    {
        /* The gathering point will be clamped between the starting/ending iterator only for random access iterator types (e.g. for std::vector, Matrix)
           For other iterator types (e.g. for std::list) it is the user's responsibility to ensure the gathering point is correctly placed between start/end
        */
        if constexpr(std::is_base_of_v<std::random_access_iterator_tag, category>)
        {
            gatheringPointIt = std::clamp<BidirectionalIt>(gatheringPointIt, startIt, endIt,
                                                           [](const BidirectionalIt& first, const BidirectionalIt& second) {return std::distance(first, second) > 0;});
        }

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
std::tuple<MatrixPoint, MatrixPoint, matrix_size_t> gatherMatrixElements(Matrix<DataType>& matrix,
                                                                         MatrixPoint startingPoint,
                                                                         MatrixPoint endingPoint,
                                                                         MatrixPoint gatheringPoint,
                                                                         std::function<bool(const DataType&)>& predicate)
{
    MatrixPoint innerRectStartingPoint{0u, 0u};
    MatrixPoint innerRectEndingPoint{0u, 0u};
    matrix_size_t gatheredElementsCount{0u};

    if (!matrix.isEmpty())
    {
        // normalize the starting/ending points to the matrix dimensions in order to get valid input for determining the outer bounding rectangle
        startingPoint.first = std::clamp(startingPoint.first.has_value() ? *startingPoint.first : 0u, 0u, matrix.getNrOfRows());
        startingPoint.second = std::clamp(startingPoint.second.has_value() ? *startingPoint.second : 0u, 0u, matrix.getNrOfColumns());
        endingPoint.first = std::clamp(endingPoint.first.has_value() ? *endingPoint.first : 0u, 0u, matrix.getNrOfRows());
        endingPoint.second = std::clamp(endingPoint.second.has_value() ? *endingPoint.second : 0u, 0u, matrix.getNrOfColumns());

        // get the outer bounding rectangle from which elements are being gathered (based on starting/ending points)
        matrix_size_t const c_OuterRectStartingRowNr{std::min(*startingPoint.first, *endingPoint.first)};
        matrix_size_t const c_OuterRectEndingRowNr{std::max(*startingPoint.first, *endingPoint.first)};
        matrix_size_t const c_OuterRectStartingColumnNr{std::min(*startingPoint.second, *endingPoint.second)};
        matrix_size_t const c_OuterRectEndingColumnNr{std::max(*startingPoint.second, *endingPoint.second)};

        // gathering point should belong to the outer bounding rectangle (and finally to the resulting inner rectangle)
        gatheringPoint.first = std::clamp(gatheringPoint.first.has_value() ? *gatheringPoint.first : 0u, c_OuterRectStartingRowNr, c_OuterRectEndingRowNr);
        gatheringPoint.second = std::clamp(gatheringPoint.second.has_value() ? *gatheringPoint.second : 0u, c_OuterRectStartingColumnNr, c_OuterRectEndingColumnNr);

        // start from the ending (maximum) outer rectangle parameter to get the corresponding starting (minimum) inner rectangle parameter (and vice-versa)
        matrix_size_t innerRectStartingRowNr{c_OuterRectEndingRowNr};
        matrix_size_t innerRectEndingRowNr{c_OuterRectStartingRowNr};
        matrix_size_t innerRectStartingColumnNr{c_OuterRectEndingColumnNr};
        matrix_size_t innerRectEndingColumnNr{c_OuterRectStartingColumnNr};

        for(matrix_size_t rowNr{c_OuterRectStartingRowNr}; rowNr < c_OuterRectEndingRowNr; ++rowNr)
        {
            auto rowGatheringRangeStartIt{c_OuterRectStartingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_OuterRectStartingColumnNr)};
            auto rowGatheringRangeEndIt{c_OuterRectEndingColumnNr == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, c_OuterRectEndingColumnNr)};
            auto rowGatheringPointIt{*gatheringPoint.second == matrix.getNrOfColumns() ? matrix.zRowEnd(rowNr) : matrix.getZIterator(rowNr, *gatheringPoint.second)};

            const auto&[rowGatheredRangeStartIt, rowGatheredRangeEndIt]{gatherSequenceElements(rowGatheringRangeStartIt, rowGatheringRangeEndIt, rowGatheringPointIt, predicate)};
            innerRectStartingColumnNr = std::min(innerRectStartingColumnNr, rowGatheredRangeStartIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : *rowGatheredRangeStartIt.getColumnNr());
            innerRectEndingColumnNr = std::max(innerRectEndingColumnNr, rowGatheredRangeEndIt == matrix.zRowEnd(rowNr) ? matrix.getNrOfColumns() : *rowGatheredRangeEndIt.getColumnNr());
            gatheredElementsCount += std::distance(rowGatheredRangeStartIt, rowGatheredRangeEndIt); // elements are counted only once (during per-row gathering)
        }

        for(matrix_size_t columnNr{c_OuterRectStartingColumnNr}; columnNr < c_OuterRectEndingColumnNr; ++columnNr)
        {
            auto columnGatheringRangeStartIt{c_OuterRectStartingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_OuterRectStartingRowNr, columnNr)};
            auto columnGatheringRangeEndIt{c_OuterRectEndingRowNr == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(c_OuterRectEndingRowNr, columnNr)};
            auto columnGatheringPointIt{*gatheringPoint.first == matrix.getNrOfRows() ? matrix.nColumnEnd(columnNr) : matrix.getNIterator(*gatheringPoint.first, columnNr)};

            const auto&[columnGatheredRangeStartIt, columnGatheredRangeEndIt]{gatherSequenceElements(columnGatheringRangeStartIt, columnGatheringRangeEndIt, columnGatheringPointIt, predicate)};
            innerRectStartingRowNr = std::min(innerRectStartingRowNr, columnGatheredRangeStartIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : *columnGatheredRangeStartIt.getRowNr());
            innerRectEndingRowNr = std::max(innerRectEndingRowNr, columnGatheredRangeEndIt == matrix.nColumnEnd(columnNr) ? matrix.getNrOfRows() : *columnGatheredRangeEndIt.getRowNr());
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
