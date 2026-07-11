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

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>

import mapcolouringutils;
import matrixutils;
import utils;

static const std::string c_InFile{Utilities::c_InputOutputDir + "countriesinput.txt"};
static const std::string c_OutFile{Utilities::c_InputOutputDir + "countriesoutput.txt"};

void printCountryColoursToFile(std::ofstream& output, const Matrix<matrix_size_t>& countryColours,
                               const Matrix<bool>& uniqueColours);

int main()
{
    std::ifstream in{c_InFile};
    std::ofstream out{c_OutFile};

    Utilities::clearScreen();

    if (in.is_open() && out.is_open())
    {
        Matrix<bool> neighbourhoodMatrix;
        in >> neighbourhoodMatrix;

        if (MapColouringUtils::isValidNeighbourhoodMatrix(neighbourhoodMatrix))
        {
            const auto c_NrOfCountries{neighbourhoodMatrix.getNrOfRows()};

            // matrix with dimensions 1 x c_NrOfCountries used for storing the colour assigned for each country
            Matrix<matrix_size_t> countryColours;

            // matrix with dimensions c_NrOfCountries x 1 where each row index of the matrix represents an allocated
            // colour; a single colour (0) is initially stored (initially not allocated)
            Matrix<bool> uniqueColours{{1, 1}, false};

            // no extra capacity to be allocated for this matrix (the number of countries is known after reading the
            // file)
            countryColours.reserve(c_NrOfCountries, 1);
            countryColours.resize(c_NrOfCountries, 1);

            for (Matrix<bool>::ConstZIterator countryIt{neighbourhoodMatrix.constZBegin()};
                 countryIt != neighbourhoodMatrix.constZRowEnd(0); ++countryIt)
            {
                const auto c_CurrentUncolouredCountry{*countryIt.getColumnNr()};
                const auto c_CountryColoursRowsCount{countryColours.getNrOfRows()};

                Matrix<matrix_size_t>::ConstNIterator currentUncolouredCountryIt{
                    countryColours.getConstNIterator(c_CurrentUncolouredCountry % c_CountryColoursRowsCount,
                                                     c_CurrentUncolouredCountry / c_CountryColoursRowsCount)};

                std::fill(uniqueColours.begin(), uniqueColours.end(), false);

                const auto c_UniqueColorsColumnsCount{uniqueColours.getNrOfColumns()};

                // mark colours that are in use by neighbors that were coloured in previous iterations
                for (Matrix<matrix_size_t>::ConstNIterator colouredCountryIt{countryColours.constNBegin()};
                     colouredCountryIt != currentUncolouredCountryIt; ++colouredCountryIt)
                {
                    const auto c_CurrentColouredCountry{*colouredCountryIt.getRowNr()};

                    if (true == neighbourhoodMatrix.at(c_CurrentColouredCountry, c_CurrentUncolouredCountry))
                    {
                        uniqueColours.at(*colouredCountryIt / c_UniqueColorsColumnsCount,
                                         *colouredCountryIt % c_UniqueColorsColumnsCount) =
                            true; // mark the colour used by neighbor as already allocated to a country
                    }
                }

                auto designatedColour{MapColouringUtils::getFirstAvailableColour(uniqueColours)};

                // choose colour with lowest index that is not in use by a neighbor (if available), otherwise create a
                // new colour and append it to the available colours list
                if (designatedColour.has_value())
                {
                    uniqueColours.at(*designatedColour / c_UniqueColorsColumnsCount,
                                     *designatedColour % c_UniqueColorsColumnsCount) = true;
                }
                else
                {
                    designatedColour = c_UniqueColorsColumnsCount;
                    uniqueColours.insertColumn(*designatedColour, true);
                }

                const auto c_CountryColoursColumnsCount{countryColours.getNrOfColumns()};
                countryColours.at(c_CurrentUncolouredCountry / c_CountryColoursColumnsCount,
                                  c_CurrentUncolouredCountry % c_CountryColoursColumnsCount) = *designatedColour;
            }

            printCountryColoursToFile(out, countryColours, uniqueColours);

            std::cout << "Country neighbourhood data read from: " << c_InFile << std::endl;
            std::cout << "Determined colour mapping of countries written to: " << c_OutFile << std::endl << std::endl;
        }
        else
        {
            std::cerr << "Invalid input. Please check input file: " << c_InFile << std::endl << std::endl;
        }
    }
    else
    {
        std::cerr << "Error in opening input and/or output file" << std::endl << std::endl;
    }

    return 0;
}

void printCountryColoursToFile(std::ofstream& output, const Matrix<matrix_size_t>& countryColours,
                               const Matrix<bool>& uniqueColours)
{
    assert(uniqueColours.getNrOfRows() == 1);

    output << countryColours.getNrOfRows() << " countries, " << uniqueColours.getNrOfColumns() << " colours"
           << std::endl
           << std::endl;
    output << "Colour mapping ordered by country index:" << std::endl << std::endl;

    for (Matrix<matrix_size_t>::ConstNIterator it{countryColours.constNBegin()}; it != countryColours.constNEnd(); ++it)
    {
        output << "Country " << *it.getRowNr() << " - Colour " << *it << std::endl;
    }
}
