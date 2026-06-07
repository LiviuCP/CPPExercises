/* This application calculates the moves of the horse on a chess table based on an initial position. The horse should
   cover all positions and each position only once. It is also possible that no solution exists.
*/

#include "matrixutils.h"
#include "utils.h"

import chesstable;
import commandargumentsparser;

using namespace std;

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

int main(int argc, char** argv)
{
    static const std::map<ErrorType, std::string> c_ErrorMessages{
        {ErrorType::INVALID_ARGUMENTS_COUNT,
         "Insufficient arguments provided, should be at least 5 (including application file path).\n"},
        {ErrorType::INVALID_STRING, "The input contains invalid (non-numeric) characters.\n"},
        {ErrorType::INVALID_TABLE_SIZE, "The table size is invalid (both dimension should be greater than 0).\n"},
        {ErrorType::INVALID_START_POSITION,
         "The starting position is invalid (should be between 1 and rows/columns count).\n"},
    };

    Utilities::clearScreen();

    const auto result{parseCommandArguments(argc, argv)};

    if (result.has_value())
    {
        const auto& applicationInput{*result};

        ChessTable chessTable{applicationInput.m_TableLength, applicationInput.m_TableWidth};
        chessTable.traverse(applicationInput.m_StartPositionX, applicationInput.m_StartPositionY);

        displayTraversingOutcome(chessTable);
    }
    else
    {
        cerr << "Invalid input!\n\n";
        cerr << c_ErrorMessages.at(result.error()) << "\n";
        cerr << "Please check and try again.\n";
    }

    return 0;
}
