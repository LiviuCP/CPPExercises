#include <cassert>

#include "mapcolouringutils.h"


bool MapColouringUtils::isValidNeighbourhoodMatrix(Matrix<bool> neighbours)
{
    bool isValid{false};

    const auto c_NrOfCountries{neighbours.getNrOfRows()};

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

std::optional<matrix_size_t> MapColouringUtils::getFirstAvailableColour(const Matrix<bool>& availableColours)
{
    // lowest index colour that is not used by a country
    std::optional<matrix_size_t> firstAvailableColour;

    if (availableColours.getNrOfRows() == 1)
    {
        // choose colour with lowest index that is not in use by a neighbor
        for (Matrix<bool>::ConstZIterator colourIt{availableColours.constZBegin()}; colourIt != availableColours.constZEnd(); ++colourIt)
        {
            if (false == *colourIt)
            {
                firstAvailableColour = *colourIt.getColumnNr();
                break;
            }
        }
    }
    else
    {
        assert(false && "Invalid colour availability matrix");
    }

    return firstAvailableColour;
}
