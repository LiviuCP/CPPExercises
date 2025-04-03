#ifndef MAPCOLOURINGUTILS_H
#define MAPCOLOURINGUTILS_H

#include "matrix.h"
#include "datautils.h"

namespace MapColouringUtils
{
    bool isValidNeighbourhoodMatrix(Matrix<bool> neighbours);
    std::optional<matrix_size_t> getFirstAvailableColour(const Matrix<bool>& availableColours);
}

#endif // MAPCOLOURINGUTILS_H
