#ifndef CHESSTABLE_H
#define CHESSTABLE_H

#include <vector>

#include "matrixutils.h"

class ChessTable
{
public:
    ChessTable(matrix_size_t nrOfRows, matrix_size_t nrOfColumns);

    void traverse(matrix_size_t startingRow = 0, matrix_size_t startingColumn = 0);

    bool isFullyTraversed() const;
    const IntMatrix& getTraversedPositions() const;

private:
    struct Point
    {
        matrix_diff_t m_X;
        matrix_diff_t m_Y;

        Point(matrix_diff_t x, matrix_diff_t y);
    };

    void _resetTable();

    bool _getBestSuccessor(const Point& currentPosition, Point& bestSuccessor) const;
    std::vector<Point> _getSuccessors(const Point& currentPosition) const;
    bool _isValidMovePosition(const Point& position) const;
    bool _isValidTablePosition(matrix_diff_t x, matrix_diff_t y) const;

    IntMatrix m_Table;
    bool m_IsFullyTraversed;
};

#endif // CHESSTABLE_H
