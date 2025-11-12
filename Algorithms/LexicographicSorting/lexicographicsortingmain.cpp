#include <cassert>
#include <fstream>
#include <iostream>

#include "lexicographicalsorter.h"
#include "matrixutils.h"
#include "utils.h"

using namespace std;

static const string c_InFile{Utilities::c_InputOutputDir + "lexicographicinput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "lexicographicoutput.txt"};

void logLexicographicalSortOutput(ofstream& outStream, const Matrix<int>& sortedMatrix,
                                  const Matrix<matrix_size_t>& originalRowNumbersMatrix);

int main()
{
    Matrix<int> matrix;
    Matrix<matrix_size_t> originalRowNumbers;
    std::ifstream in{c_InFile};

    Utilities::clearScreen();

    if (in.is_open())
    {
        cout << "Reading matrix elements from input file: " << c_InFile << endl << endl;
        in >> matrix;

        if (!matrix.isEmpty())
        {
            (void)LexicographicalSorter<int>::sort(matrix, originalRowNumbers, true);
            cout << "The matrix has been successfully read and lexicographically sorted" << endl;

            std::ofstream out{c_OutFile};

            if (out.is_open())
            {
                logLexicographicalSortOutput(out, matrix, originalRowNumbers);
                cout << "Output has been written to file: " << c_OutFile << endl << endl;
            }
            else
            {
                cerr << "Error in opening output file: " << c_OutFile << endl;
            }
        }
        else
        {
            cerr << "Invalid matrix data" << endl << endl;
        }
    }
    else
    {
        cerr << "Error opening file: " << c_InFile << endl;
    }

    return 0;
}

void logLexicographicalSortOutput(ofstream& outStream, const Matrix<int>& sortedMatrix,
                                  const Matrix<matrix_size_t>& originalRowNumbersMatrix)
{
    assert(originalRowNumbersMatrix.getNrOfColumns() == 1 &&
           originalRowNumbersMatrix.getNrOfRows() == sortedMatrix.getNrOfRows());

    outStream << "The lexicographically sorted matrix is:\n\n";
    outStream << sortedMatrix << endl;
    outStream << "Number of each row before lexicographic sorting: " << endl << endl;

    for (Matrix<matrix_size_t>::ConstNIterator it{originalRowNumbersMatrix.constNBegin()};
         it != originalRowNumbersMatrix.constNEnd(); ++it)
    {
        outStream << *it << endl;
    }
}
