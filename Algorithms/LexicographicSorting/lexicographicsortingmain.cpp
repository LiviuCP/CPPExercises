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
                                  const std::vector<matrix_size_t>& originalRowNumbers);

int main()
{
    Matrix<int> matrix;
    std::ifstream in{c_InFile};

    Utilities::clearScreen();

    if (in.is_open())
    {
        cout << "Reading matrix elements from input file: " << c_InFile << "\n\n";
        in >> matrix;

        if (!matrix.isEmpty())
        {
            const std::vector<matrix_size_t> c_OriginalRowNumbers{LexicographicalSorter<int>::sort(matrix, true)};
            cout << "The matrix has been successfully read and lexicographically sorted\n";

            std::ofstream out{c_OutFile};

            if (out.is_open())
            {
                logLexicographicalSortOutput(out, matrix, c_OriginalRowNumbers);
                cout << "Output has been written to file: " << c_OutFile << "\n\n";
            }
            else
            {
                cerr << "Error in opening output file: " << c_OutFile << "\n";
            }
        }
        else
        {
            cerr << "Invalid matrix data\n\n";
        }
    }
    else
    {
        cerr << "Error opening file: " << c_InFile << "\n";
    }

    return 0;
}

void logLexicographicalSortOutput(ofstream& outStream, const Matrix<int>& sortedMatrix,
                                  const std::vector<matrix_size_t>& originalRowNumbers)
{
    assert(originalRowNumbers.size() == sortedMatrix.getNrOfRows());

    outStream << "The lexicographically sorted matrix is:\n\n";
    outStream << sortedMatrix << "\n";
    outStream << "Number of each row before lexicographic sorting:\n\n";

    for (auto it{originalRowNumbers.cbegin()}; it != originalRowNumbers.cend(); ++it)
    {
        outStream << *it << "\n";
    }
}
