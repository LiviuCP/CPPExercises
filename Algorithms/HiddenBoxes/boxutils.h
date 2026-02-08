#pragma once

#include "matrix.h"

std::vector<matrix_size_t> retrieveFittingBoxes(const Matrix<matrix_size_t>& boxes);
bool boxFitsIntoBox(matrix_size_t fittingBoxNumber, matrix_size_t includingBoxNumber,
                    const Matrix<matrix_size_t>& boxes);
