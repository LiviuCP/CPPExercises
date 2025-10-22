#include <algorithm>
#include <cassert>

#include "chesstable.h"

ChessTable::ChessTable(matrix_size_t nrOfRows, matrix_size_t nrOfColumns)
    : m_IsFullyTraversed{false}
{
    if (nrOfRows > 0 && nrOfColumns > 0)
    {
        m_Table.resize(nrOfRows, nrOfColumns, 0);
    }
    else
    {
        assert(false && "Invalid chess table dimensions provided");
    }
}

void ChessTable::traverse(matrix_size_t startingRow, matrix_size_t startingColumn)
{
    if (_isValidTablePosition(startingRow, startingColumn))
    {
        // ensure any previous traversal is cleared for a fresh new start
        _resetTable();

        // each traversed position contains the number of the current move; first move belongs to the starting position
        matrix_size_t moveNumber{1};
        m_Table.at(startingRow, startingColumn) = moveNumber;

        Point currentPos{static_cast<matrix_diff_t>(startingRow), static_cast<matrix_diff_t>(startingColumn)};
        Point successor{currentPos}; // stores the best successor (initially initialized with the current position so it is not left uninitialized - could be any other initial value)
        bool hasSuccessor{_getBestSuccessor(currentPos, successor)};

        // traverse the table and update each found best successor position
        while(hasSuccessor)
        {
            currentPos = successor;
            ++moveNumber;
            m_Table.at(currentPos.m_X, currentPos.m_Y) = moveNumber;
            hasSuccessor = _getBestSuccessor(currentPos, successor);
        }

        // check that all table positions have been passed through (no "dead-end" has been encountered)
        m_IsFullyTraversed = (moveNumber == m_Table.getNrOfRows() * m_Table.getNrOfColumns());
    }
}

bool ChessTable::isFullyTraversed() const
{
    return m_IsFullyTraversed;
}

const IntMatrix& ChessTable::getTraversedPositions() const
{
    return m_Table;
}

void ChessTable::_resetTable()
{
    m_IsFullyTraversed = false;
    std::fill(m_Table.begin(), m_Table.end(), 0);
}

// best successor is the one with minimum number of successors (successor = eligible next move position meaning it is both existing within table and not traversed in a previous move)
bool ChessTable::_getBestSuccessor(const ChessTable::Point& currentPosition, ChessTable::Point& bestSuccessor) const
{
    bool hasSuccessors{false};

    std::vector<Point> successors{_getSuccessors(currentPosition)};

    if (successors.size() > 0)
    {
        hasSuccessors = true;
        matrix_size_t minNrOfSuccessors{8}; // assume each successor has maximum number of successors (8 possible) and update each time a possible next moving position with less successors is identified

        for (const auto& successor : successors)
        {
            const auto c_NrOfSuccessors{_getSuccessors(successor).size()};

            if (c_NrOfSuccessors < minNrOfSuccessors)
            {
                minNrOfSuccessors = c_NrOfSuccessors;
                bestSuccessor = successor;
            }
        }
    }

    return hasSuccessors;
}

std::vector<ChessTable::Point> ChessTable::_getSuccessors(const ChessTable::Point& currentPosition) const
{
    std::vector<Point> result;

    // possible horse moving positions (relative to current position)
    const std::vector<Point> c_HorseMoveOffsets{{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    for (const auto& offset : c_HorseMoveOffsets)
    {
        Point successor{currentPosition.m_X + offset.m_X, currentPosition.m_Y + offset.m_Y};

        if (_isValidMovePosition(successor))
        {
            result.push_back(successor);
        }
    }

    return result;
}

bool ChessTable::_isValidMovePosition(const ChessTable::Point& position) const
{
    bool c_IsValidPos{_isValidTablePosition(position.m_X, position.m_Y) && m_Table.at(position.m_X, position.m_Y) == 0};
    return c_IsValidPos;
}

bool ChessTable::_isValidTablePosition(matrix_diff_t x, matrix_diff_t y) const
{
    const matrix_diff_t c_NrOfTableRows{static_cast<matrix_diff_t>(m_Table.getNrOfRows())};
    const matrix_diff_t c_NrOfTableColumns{static_cast<matrix_diff_t>(m_Table.getNrOfColumns())};
    const bool c_IsValidXPos{x >= 0 && x < c_NrOfTableRows};
    const bool c_IsValidYPos{y >= 0 && y < c_NrOfTableColumns};
    const bool c_IsValidPos{c_IsValidXPos && c_IsValidYPos};

    return c_IsValidPos;
}

ChessTable::Point::Point(matrix_diff_t x, matrix_diff_t y)
    : m_X{x}, m_Y{y}
{
}
