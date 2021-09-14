/* This application calculates the moves of the horse on a chess table based on an initial position. The horse should cover all positions and each position only once.
   It is also possible that no solution exists.
*/
#include <iostream>

#include "chesstable.h"

using namespace std;

void retrieveInput(int& input, std::string inputRequestMessage);

int main()
{
    int tableLength{-1};
    int tableWidth{-1};
    int startPositionX{-1};
    int startPositionY{-1};

    cout << "Enter the chess table dimensions:" << endl;
    retrieveInput(tableLength, "Length = ");
    retrieveInput(tableWidth, "Width = ");
    cout << endl;
    cout << "Enter the initial position of the horse:" << endl;
    retrieveInput(startPositionX, "Horizontal = ");
    retrieveInput(startPositionY, "Vertical = ");
    cout << endl;

    if (tableLength <= 0 || tableWidth <= 0 || startPositionX < 1 || startPositionX > tableLength || startPositionY < 1 || startPositionY > tableWidth)
    {
        cerr << "Invalid table dimensions and/or start position of the horse" << endl;
    }
    else
    {
        ChessTable chessTable{tableLength, tableWidth};
        chessTable.traverse(startPositionX-1, startPositionY-1);

        if (chessTable.isFullyTraversed())
        {
            cout << "The resulting chess table traversing is:" << endl << endl;
            chessTable.printTable();
        }
        else
        {
            cout << "No table traversing solution found!" << endl;
        }
    }

    cout << endl;

    return 0;
}

void retrieveInput(int& input, std::string inputRequestMessage)
{
    cout << inputRequestMessage;
    cin >> input;

    if (cin.fail())
    {
        cerr << "Bad input. Aborted" << endl;
        exit(-1);
    }
}
