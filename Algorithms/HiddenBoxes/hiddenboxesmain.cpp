/* This program handles multi-dimensional boxes of different sizes
   - goal: find the maximum number of boxes that fit into each other
   - notes:
     - the dimensions of each box can be re-ordered (like rotating the box), e.g. a three-dimensional box of sizes (1,
   3, 2) can be re-ordered (1, 2, 3)
     - the boxes need to be lexicographically sorted (in increasing order); then dynamic programming is performed to
   determine the maximum number of boxes that fit into each other
*/

#include <fstream>
#include <iostream>

#include "boxutils.h"
#include "lexicographicalsorter.h"
#include "matrixutils.h"
#include "utils.h"

using namespace std;

static const string c_InFile{Utilities::c_InputOutputDir + "boxesinput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "boxesoutput.txt"};

void logFittingBoxesToFile(ofstream& outStream, const Matrix<matrix_size_t>& fittingBoxIndexes,
                           const std::vector<matrix_size_t>& originalIndexes);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if (in.is_open() && out.is_open())
    {
        Matrix<matrix_size_t> sortedBoxData;
        Matrix<matrix_size_t>
            fittingBoxesIndexes; // original indexes of the boxes that belong to maximum fitting series

        cout << "Reading all existing box series from input file: " << c_InFile << "\n\n";

        int nrOfSeries{0};

        while (!in.eof())
        {
            in >> sortedBoxData;

            if (sortedBoxData.getNrOfRows() > 0)
            {
                // the "original indexes" are the indexes of all boxes before lexicographic sort
                const std::vector<matrix_size_t> c_OriginalIndexes{
                    LexicographicalSorter<matrix_size_t>::sort(sortedBoxData, true)};
                retrieveFittingBoxes(sortedBoxData, fittingBoxesIndexes);

                cout << "Writing maximum fitting range of boxes to output file: " << c_OutFile << "\n";

                logFittingBoxesToFile(out, fittingBoxesIndexes, c_OriginalIndexes);
                ++nrOfSeries;
            }
        }

        cout << "\n" << nrOfSeries << " series found and processed\n\n";
    }
    else
    {
        cerr << "Error in opening input and/or output file\n\n";
    }

    return 0;
}

// logs the input file row number of each found box by taking the lexicographical ordering into consideration
void logFittingBoxesToFile(ofstream& outStream, const Matrix<matrix_size_t>& fittingBoxIndexes,
                           const std::vector<matrix_size_t>& originalIndexes)
{
    const matrix_diff_t c_NrOfFittingBoxes{
        std::distance(fittingBoxIndexes.constZBegin(), fittingBoxIndexes.constZEnd())};
    const size_t c_TotalNrOfBoxes{originalIndexes.size()};

    assert(c_NrOfFittingBoxes <= c_TotalNrOfBoxes);

    outStream << "In-order box numbers (boxes sorted increasingly by size): ";

    for (Matrix<matrix_size_t>::ConstZIterator it{fittingBoxIndexes.constZBegin()}; it != fittingBoxIndexes.constZEnd();
         ++it)
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
