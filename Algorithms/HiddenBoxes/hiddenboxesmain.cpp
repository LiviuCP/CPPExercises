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
                           const Matrix<matrix_size_t>& originalIndexes);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if (in.is_open() && out.is_open())
    {
        Matrix<matrix_size_t> sortedBoxData;
        Matrix<matrix_size_t> originalIndexes; // box indexes before lexicographic sort
        Matrix<matrix_size_t> fittingBoxes;    // original indexes of the boxes that belong to maximum fitting series

        cout << "Reading all existing box series from input file: " << c_InFile << endl << endl;

        int nrOfSeries{0};

        while (!in.eof())
        {
            in >> sortedBoxData;

            if (sortedBoxData.getNrOfRows() > 0)
            {
                LexicographicalSorter<matrix_size_t>::sort(sortedBoxData, originalIndexes, true);
                retrieveFittingBoxes(sortedBoxData, fittingBoxes);

                cout << "Writing maximum fitting range of boxes to output file: " << c_OutFile << endl;

                logFittingBoxesToFile(out, fittingBoxes, originalIndexes);
                ++nrOfSeries;
            }
        }

        cout << endl << nrOfSeries << " series found and processed" << endl << endl;
    }
    else
    {
        cerr << "Error in opening input and/or output file" << endl << endl;
    }

    return 0;
}

// logs the input file row number of each found box by taking the lexicographical ordering into consideration
void logFittingBoxesToFile(ofstream& outStream, const Matrix<matrix_size_t>& fittingBoxIndexes,
                           const Matrix<matrix_size_t>& originalIndexes)
{
    const matrix_diff_t c_NrOfFittingBoxes{
        std::distance(fittingBoxIndexes.constZBegin(), fittingBoxIndexes.constZEnd())};
    const matrix_size_t c_OriginalIndexesColumnsCount{originalIndexes.getNrOfColumns()};

    outStream << "In-order box numbers (boxes sorted increasingly by size): ";

    for (Matrix<matrix_size_t>::ConstZIterator it{fittingBoxIndexes.constZBegin()}; it != fittingBoxIndexes.constZEnd();
         ++it)
    {
        const matrix_size_t boxNumber{
            originalIndexes.at(*it / c_OriginalIndexesColumnsCount, *it % c_OriginalIndexesColumnsCount) +
            1}; // provide a "human readable" number to the box so it can easily be identifed in the input file
        outStream << boxNumber << " ";
    }

    outStream << endl << c_NrOfFittingBoxes << " fitting boxes found" << endl << endl;
}
