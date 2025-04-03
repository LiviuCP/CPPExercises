#include <iostream>
#include <algorithm>
#include <cassert>

#include "chesstable.h"

ChessTable::ChessTable(matrix_size_t nrOfRows, matrix_size_t nrOfColumns)
    : m_Table{}
    , m_IsFullyTraversed{false}
{
    assert(nrOfRows > 0 && nrOfColumns > 0 && "Invalid chess table dimensions provided");
    m_Table.resizeWithValue(nrOfRows, nrOfColumns, 0);
}

void ChessTable::setSize(matrix_size_t nrOfRows, matrix_size_t nrOfColumns)
{
    if (nrOfRows > 0 && nrOfColumns > 0)
    {
        m_Table.resizeWithValue(nrOfRows, nrOfColumns, 0);
        m_IsFullyTraversed = false;
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

void ChessTable::printTable() const
{
    for (matrix_size_t row{0}; row < m_Table.getNrOfRows(); ++row)
    {
        for (Matrix<int>::ConstZIterator it{m_Table.constZRowBegin(row)}; it != m_Table.constZRowEnd(row); ++it)
        {
            std::cout << *it << " ";
        }

        std::cout << std::endl;
    }
}

bool ChessTable::isFullyTraversed() const
{
    return m_IsFullyTraversed;
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

bool ChessTable::_isValidMovePosition(ChessTable::Point position) const
{
    bool isValid{_isValidTablePosition(position.m_X, position.m_Y) && m_Table.at(position.m_X, position.m_Y) == 0};
    return isValid;
}

bool ChessTable::_isValidTablePosition(matrix_diff_t x, matrix_diff_t y) const
{
    const bool c_IsValid{x >= 0 && static_cast<matrix_size_t>(x) < m_Table.getNrOfRows() && y >= 0 && static_cast<matrix_size_t>(y) < m_Table.getNrOfColumns()};
    return c_IsValid;
}

ChessTable::Point::Point(matrix_diff_t x, matrix_diff_t y)
    : m_X{x}, m_Y{y}
{
}
