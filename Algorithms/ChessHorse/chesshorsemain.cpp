/* This application calculates the moves of the horse on a chess table based on an initial position. The horse should
   cover all positions and each position only once. It is also possible that no solution exists.
*/

#include <iostream>

import chesstable;
import commandargumentsparser;
import utils;

int main(int argc, char** argv)
{
    Utilities::clearScreen();

    const CommandArgumentsParsingResult result{parseCommandArguments(argc, argv)};

    if (result.has_value())
    {
        const auto& applicationInput{*result};
        traverseChessTable(applicationInput.m_TableLength, applicationInput.m_TableWidth,
                           applicationInput.m_StartPositionX, applicationInput.m_StartPositionY);
    }
    else
    {
        std::cerr << "Invalid input!\n\n";
        std::cerr << buildErrorMessage(result.error()) << "\n";
        std::cerr << "Please check and try again.\n";
    }

    return 0;
}
