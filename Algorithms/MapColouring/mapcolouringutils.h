#ifndef MAPCOLOURINGUTILS_H
#define MAPCOLOURINGUTILS_H

#include "matrix.h"

namespace MapColouringUtils
{
    bool isValidNeighbourhoodMatrix(Matrix<bool> neighbours);
    int getFirstAvailableColour(const Matrix<bool>& availableColours);
}

#endif // MAPCOLOURINGUTILS_H
