#pragma once

#include "matrix.h"

namespace MapColouringUtils
{
bool isValidNeighbourhoodMatrix(Matrix<bool> neighbours);
std::optional<matrix_size_t> getFirstAvailableColour(const Matrix<bool>& availableColours);
} // namespace MapColouringUtils
