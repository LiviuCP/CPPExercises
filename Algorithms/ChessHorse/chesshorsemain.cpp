/* This application calculates the moves of the horse on a chess table based on an initial position. The horse should cover all positions and each position only once.
   It is also possible that no solution exists.
*/
#include <iostream>

#include "chesstable.h"
#include "userinput.h"
#include "utils.h"

using namespace std;

void displayTraversingOutcome(const ChessTable& chessTable);

int main(int argc, char** argv)
{
    Utilities::clearScreen();

    constexpr int c_RequiredParametersCount{4};

    if (argc > c_RequiredParametersCount)
    {
        const UserInput userInput{{argv[1]}, {argv[2]}, {argv[3]}, {argv[4]}};

        if (userInput.isValid())
        {
            ChessTable chessTable{*userInput.m_TableLength, *userInput.m_TableWidth};
            chessTable.traverse(*userInput.m_StartPositionX - 1, *userInput.m_StartPositionY - 1);

            displayTraversingOutcome(chessTable);
        }
        else
        {
            cerr << "Invalid input!\n\n";
            cerr << "Possible reasons:\n";
            cerr << "- invalid (non-numeric) characters\n";
            cerr << "- invalid table size (both dimension should be greater than 0)\n";
            cerr << "- invalid starting position (should be between 1 and rows/columns count)\n\n";
            cerr << "Please check and try again.\n";
        }
    }
    else
    {
        Utilities::clearScreen();
        cerr << "Insufficient arguments count (should be " << c_RequiredParametersCount << ")!\n";
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
            for (Matrix<int>::ConstZIterator it{c_TraversedPositions.constZRowBegin(row)}; it != c_TraversedPositions.constZRowEnd(row); ++it)
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
