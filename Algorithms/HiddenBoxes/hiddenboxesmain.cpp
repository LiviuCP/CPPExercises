/* This program handles multi-dimensional boxes of different sizes
   - goal: find the maximum number of boxes that fit into each other
   - notes:
     - the dimensions of each box can be re-ordered (like rotating the box), e.g. a three-dimensional box of sizes (1,
   3, 2) can be re-ordered (1, 2, 3)
     - the boxes need to be lexicographically sorted (in increasing order); then dynamic programming is performed to
   determine the maximum number of boxes that fit into each other
*/

#include <cassert>
#include <fstream>
#include <iostream>

import boxutils;
import matrixutils;
import utils;

static const std::string c_InFile{Utilities::c_InputOutputDir + "boxesinput.txt"};
static const std::string c_OutFile{Utilities::c_InputOutputDir + "boxesoutput.txt"};

void logFittingBoxesToFile(std::ofstream& outStream, const std::vector<matrix_size_t>& fittingBoxIndexes,
                           const std::vector<matrix_size_t>& originalIndexes);

int main()
{
    std::ifstream in{c_InFile};
    std::ofstream out{c_OutFile};

    Utilities::clearScreen();

    if (in.is_open() && out.is_open())
    {
        Matrix<matrix_size_t> sortedBoxData;

        std::cout << "Reading all existing box series from input file: " << c_InFile << "\n\n";

        int nrOfSeries{0};

        while (!in.eof())
        {
            in >> sortedBoxData;

            if (sortedBoxData.getNrOfRows() > 0)
            {
                // the "original indexes" are the indexes of all boxes before lexicographic sort
                const std::vector<matrix_size_t> c_OriginalIndexes{
                    Utilities::lexicographicalSort<matrix_size_t>(sortedBoxData, true)};

                // the "fitting boxes indexes" are the "before lexicographic sort" indexes of the boxes that belong to
                // maximum fitting series
                const std::vector<matrix_size_t> c_FittingBoxesIndexes{retrieveFittingBoxes(sortedBoxData)};

                std::cout << "Writing maximum fitting range of boxes to output file: " << c_OutFile << "\n";

                logFittingBoxesToFile(out, c_FittingBoxesIndexes, c_OriginalIndexes);
                ++nrOfSeries;
            }
        }

        std::cout << "\n" << nrOfSeries << " series found and processed\n\n";
    }
    else
    {
        std::cerr << "Error in opening input and/or output file\n\n";
    }

    return 0;
}

// logs the input file row number of each found box by taking the lexicographical ordering into consideration
void logFittingBoxesToFile(std::ofstream& outStream, const std::vector<matrix_size_t>& fittingBoxIndexes,
                           const std::vector<matrix_size_t>& originalIndexes)
{
    const size_t c_NrOfFittingBoxes{fittingBoxIndexes.size()};
    const size_t c_TotalNrOfBoxes{originalIndexes.size()};

    assert(c_NrOfFittingBoxes <= c_TotalNrOfBoxes);

    outStream << "In-order box numbers (boxes sorted increasingly by size): ";

    for (auto it{fittingBoxIndexes.cbegin()}; it != fittingBoxIndexes.cend(); ++it)
    {
        if (*it >= c_TotalNrOfBoxes)
        {
            assert(false);
            break;
        }

        // provide a "human readable" number to the box so it can easily be identifed in the input file
        const matrix_size_t c_BoxNumber{originalIndexes[*it] + 1};
        outStream << c_BoxNumber << " ";
    }

    outStream << "\n" << c_NrOfFittingBoxes << " fitting boxes found\n\n";
}
