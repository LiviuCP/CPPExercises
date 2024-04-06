#include <iostream>
#include <algorithm>
#include <cassert>

#include "chesstable.h"

ChessTable::ChessTable(int nrOfRows, int nrOfColumns)
    : m_Table{}
    , m_IsFullyTraversed{false}
{
    assert(nrOfRows > 0 && nrOfColumns > 0 && "Invalid chess table dimensions provided");
    m_Table.resizeWithValue(nrOfRows, nrOfColumns, 0);
}

void ChessTable::setSize(int nrOfRows, int nrOfColumns)
{
    if (nrOfRows > 0 && nrOfColumns > 0)
    {
        m_Table.resizeWithValue(nrOfRows, nrOfColumns, 0);
        m_IsFullyTraversed = false;
    }
}

void ChessTable::traverse(int startingRow, int startingColumn)
{
    if (_isValidTablePosition(startingRow, startingColumn))
    {
        // ensure any previous traversal is cleared for a fresh new start
        _resetTable();

        // each traversed position contains the number of the current move; first move belongs to the starting position
        int moveNumber{1};
        m_Table.at(startingRow, startingColumn) = moveNumber;

        Point currentPos{startingRow, startingColumn};
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
    for (int row{0}; row < m_Table.getNrOfRows(); ++row)
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
        int minNrOfSuccessors{8}; // assume each successor has maximum number of successors (8 possible) and update each time a possible next moving position with less successors is identified

        for (const auto& successor : successors)
        {
            int nrOfSuccessors{static_cast<int>(_getSuccessors(successor).size())};

            if (nrOfSuccessors < minNrOfSuccessors)
            {
                minNrOfSuccessors = nrOfSuccessors;
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

bool ChessTable::_isValidTablePosition(int x, int y) const
{
    bool isValid{x >= 0 && x < m_Table.getNrOfRows() && y >= 0 && y < m_Table.getNrOfColumns()};
    return isValid;
}

ChessTable::Point::Point(int x, int y)
    : m_X{x}, m_Y{y}
{
}
