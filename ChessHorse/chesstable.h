#ifndef CHESSTABLE_H
#define CHESSTABLE_H

#include <string>
#include <vector>

#include "../External/Matrix/MatrixLib/matrix.h"

class ChessTable
{
public:
    ChessTable() = delete;
    ChessTable(int nrOfRows, int nrOfColumns);

    void setSize(int nrOfRows, int columns);
    void traverse(int startingRow = 0, int startingColumn = 0);

    void printTable() const;
    bool isFullyTraversed() const;

private:
    struct Point
    {
        int m_X;
        int m_Y;

        Point(int x, int y);
    };

    void _resetTable();

    bool _getBestSuccessor(const Point& currentPosition, Point& bestSuccessor) const;
    std::vector<Point> _getSuccessors(const Point& currentPosition) const;
    bool _isValidMovePosition(Point position) const;
    bool _isValidTablePosition(int x, int y) const;

    Matrix<int> m_Table;
    bool m_IsFullyTraversed;
};

#endif // CHESSTABLE_H
