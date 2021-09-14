#include <cassert>

#include "mapcolouringutils.h"


bool MapColouringUtils::isValidNeighbourhoodMatrix(Matrix<bool> neighbours)
{
    bool isValid{false};

    const int c_NrOfCountries{neighbours.getNrOfRows()};

    if (c_NrOfCountries > 0 && c_NrOfCountries == neighbours.getNrOfColumns())
    {
        Matrix<bool> neighboursTransposed;
        neighbours.transpose(neighboursTransposed);

        if (neighbours == neighboursTransposed)
        {
            isValid = true;
        }
    }

    return isValid;
}

int MapColouringUtils::getFirstAvailableColour(const Matrix<bool>& availableColours)
{
    assert(availableColours.getNrOfRows() == 1 && "Invalid colour availability matrix");

    // lowest index colour that is not used by a country
    int firstAvailableColour{-1};

    // choose colour with lowest index that is not in use by a neighbor
    for (Matrix<bool>::ConstZIterator colourIt{availableColours.constZBegin()}; colourIt != availableColours.constZEnd(); ++colourIt)
    {
        if (false == *colourIt)
        {
            firstAvailableColour = colourIt.getCurrentColumnNr();
            break;
        }
    }

    return firstAvailableColour;
}
