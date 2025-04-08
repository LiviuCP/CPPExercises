/* This is a small application that illustrates coloring a map so:
   - each country has neighbors of different colours
   - the total number of colours used for colouring is as small as possible

   Notes:
   - a greedy algorithm is used so it is not guaranteed that the optimal solution is achieved
   - the countries and colours will not be named but represented as indexes starting from 0
   - a neighbourhood matrix is provided in the input file:
     - each row/column number corresponds to a country
     - the intersection contains the neighborhood relationship ( true (!= 0): neighbors, false (== 0): not neighbors)
   - the output file contains the chosen mapping of colours to countries
*/

#include <iostream>
#include <algorithm>

#include "matrix.h"
#include "utils.h"
#include "matrixutils.h"
#include "mapcolouringutils.h"

using namespace std;

static const string c_InFile{Utilities::c_InputOutputDir + "countriesinput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "countriesoutput.txt"};

void printCountryColoursToFile(ofstream& output, const Matrix<matrix_size_t>& countryColours, const Matrix<bool>& uniqueColours);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        Matrix<bool> neighbourhoodMatrix;
        in >> neighbourhoodMatrix;

        if (MapColouringUtils::isValidNeighbourhoodMatrix(neighbourhoodMatrix))
        {
            const auto c_NrOfCountries{neighbourhoodMatrix.getNrOfRows()};

            // matrix with dimensions 1 x c_NrOfCountries used for storing the colour assigned for each country
            Matrix<matrix_size_t> countryColours;

            // matrix with dimensions c_NrOfCountries x 1 where each row index of the matrix represents an allocated colour; a single colour (0) is initially stored (initially not allocated)
            Matrix<bool> uniqueColours{{1, 1}, false};

            // no extra capacity to be allocated for this matrix (the number of countries is known after reading the file)
            countryColours.resize(c_NrOfCountries, 1, c_NrOfCountries, 1);

            for (Matrix<bool>::ConstZIterator countryIt{neighbourhoodMatrix.constZBegin()}; countryIt != neighbourhoodMatrix.constZRowEnd(0); ++countryIt)
            {
                const auto c_CurrentUncolouredCountry{*countryIt.getColumnNr()};
                Matrix<matrix_size_t>::ConstNIterator currentUncolouredCountryIt{countryColours.getConstNIterator(c_CurrentUncolouredCountry)};

                std::fill(uniqueColours.begin(), uniqueColours.end(), false);

                // mark colours that are in use by neighbors that were coloured in previous iterations
                for (Matrix<matrix_size_t>::ConstNIterator colouredCountryIt{countryColours.constNBegin()}; colouredCountryIt != currentUncolouredCountryIt; ++colouredCountryIt)
                {
                    const auto c_CurrentColouredCountry{*colouredCountryIt.getRowNr()};

                    if (true == neighbourhoodMatrix.at(c_CurrentColouredCountry, c_CurrentUncolouredCountry))
                    {
                        uniqueColours[*colouredCountryIt] = true; // mark the colour used by neighbor as already allocated to a country
                    }
                }

                auto designatedColour{MapColouringUtils::getFirstAvailableColour(uniqueColours)};

                // choose colour with lowest index that is not in use by a neighbor (if available), otherwise create a new colour and append it to the available colours list
                if (designatedColour.has_value())
                {
                    uniqueColours[*designatedColour] = true;
                }
                else
                {
                    designatedColour = uniqueColours.getNrOfColumns();
                    uniqueColours.insertColumn(*designatedColour, true);
                }

                countryColours[c_CurrentUncolouredCountry] = *designatedColour;
            }

            printCountryColoursToFile(out, countryColours, uniqueColours);

            cout << "Country neighbourhood data read from: " << c_InFile << endl;
            cout << "Determined colour mapping of countries written to: " << c_OutFile << endl << endl;
        }
        else
        {
            cerr << "Invalid input. Please check input file: " << c_InFile << endl << endl;
        }
    }
    else
    {
        cerr << "Error in opening input and/or output file" << endl << endl;
    }

    return 0;
}

void printCountryColoursToFile(ofstream& output, const Matrix<matrix_size_t>& countryColours, const Matrix<bool>& uniqueColours)
{
    assert(uniqueColours.getNrOfRows() == 1);

    output << countryColours.getNrOfRows() << " countries, " << uniqueColours.getNrOfColumns() << " colours" << endl << endl;
    output << "Colour mapping ordered by country index:" << endl << endl;

    for (Matrix<matrix_size_t>::ConstNIterator it{countryColours.constNBegin()}; it != countryColours.constNEnd(); ++it)
    {
        output << "Country " << *it.getRowNr() << " - Colour " << *it << endl;
    }
}
