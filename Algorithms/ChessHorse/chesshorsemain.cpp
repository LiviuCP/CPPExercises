/* This application calculates the moves of the horse on a chess table based on an initial position. The horse should
   cover all positions and each position only once. It is also possible that no solution exists.
*/
#include <iostream>
#include <map>

#include "chesstable.h"
#include "commandargumentsparser.h"
#include "utils.h"

static const std::map<ResultType, std::string> c_ErrorMessages{
    {ResultType::INVALID_ARGUMENTS_COUNT,
     "Insufficient arguments provided, should be at least 5 (including application file path).\n"},
    {ResultType::INVALID_STRING, "The input contains invalid (non-numeric) characters.\n"},
    {ResultType::INVALID_TABLE_SIZE, "The table size is invalid (both dimension should be greater than 0).\n"},
    {ResultType::INVALID_START_POSITION,
     "The starting position is invalid (should be between 1 and rows/columns count).\n"},
};

using namespace std;

void displayTraversingOutcome(const ChessTable& chessTable);

int main(int argc, char** argv)
{
    Utilities::clearScreen();

    const auto& [resultType, applicationInput]{parseCommandArguments(argc, argv)};

    if (resultType == ResultType::SUCCESS)
    {
        if (applicationInput.has_value())
        {
            ChessTable chessTable{applicationInput->m_TableLength, applicationInput->m_TableWidth};
            chessTable.traverse(applicationInput->m_StartPositionX, applicationInput->m_StartPositionY);

            displayTraversingOutcome(chessTable);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        cerr << "Invalid input!\n\n";
        cerr << c_ErrorMessages.at(resultType) << "\n";
        cerr << "Please check and try again.\n";
    }

    return 0;
}

void displayTraversingOutcome(const ChessTable& chessTable)
{
    if (chessTable.isFullyTraversed())
    {
        const IntMatrix& c_TraversedPositions{chessTable.getTraversedPositions()};
        cout << "The resulting chess table traversing is:\n\n";

        for (matrix_size_t row{0}; row < c_TraversedPositions.getNrOfRows(); ++row)
        {
            for (Matrix<int>::ConstZIterator it{c_TraversedPositions.constZRowBegin(row)};
                 it != c_TraversedPositions.constZRowEnd(row); ++it)
            {
                cout << *it << " ";
            }

            cout << "\n";
        }
    }
    else
    {
        cout << "No table traversing solution found!\n";
    }
}
