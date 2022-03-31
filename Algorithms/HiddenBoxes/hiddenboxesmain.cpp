/* This program handles multi-dimensional boxes of different sizes
   - goal: find the maximum number of boxes that fit into each other
   - notes:
     - the dimensions of each box can be re-ordered (like rotating the box), e.g. a three-dimensional box of sizes (1, 3, 2) can be re-ordered (1, 2, 3)
     - the boxes need to be lexicographically sorted (in increasing order); then dynamic programming is performed to determine the maximum number of boxes that fit into each other
*/

#include <iostream>
#include <fstream>

#include "lexicographicalsorter.h"
#include "utils.h"
#include "matrixutils.h"
#include "boxutils.h"

using namespace std;

static const string c_InFile{Utilities::c_InputOutputDir + "boxesinput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "boxesoutput.txt"};

void logFittingBoxesToFile(ofstream& outStream, const Matrix<int>& fittingBoxIndexes, const Matrix<int>& originalIndexes);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        Matrix<int> sortedBoxData;
        Matrix<int> originalIndexes; // box indexes before lexicographic sort
        Matrix<int> fittingBoxes; // original indexes of the boxes that belong to maximum fitting series

        cout << "Reading all existing box series from input file: " << c_InFile << endl << endl;

        int nrOfSeries{0};

        while(!in.eof())
        {
            in >> sortedBoxData;

            if(sortedBoxData.getNrOfRows() > 0)
            {
                LexicographicalSorter<int>::sort(sortedBoxData, originalIndexes, true);
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
void logFittingBoxesToFile(ofstream& outStream, const Matrix<int>& fittingBoxIndexes, const Matrix<int>& originalIndexes)
{
    const int nrOfFittingBoxes{fittingBoxIndexes.constZEnd() - fittingBoxIndexes.constZBegin()};

    outStream << "In-order box numbers (boxes sorted increasingly by size): ";

    for (Matrix<int>::ConstZIterator it{fittingBoxIndexes.constZBegin()}; it != fittingBoxIndexes.constZEnd(); ++it)
    {
        int boxNumber{originalIndexes[*it] + 1}; // provide a "human readable" number to the box so it can easily be identifed in the input file
        outStream << boxNumber << " ";
    }

    outStream << endl << nrOfFittingBoxes << " fitting boxes found" << endl << endl;
}
