#ifndef BOXUTILS_H
#define BOXUTILS_H

#include "matrix.h"

void retrieveFittingBoxes(const Matrix<matrix_size_t>& boxes, Matrix<matrix_size_t>& fittingBoxIndexes);
bool boxFitsIntoBox(matrix_size_t fittingBoxNumber, matrix_size_t includingBoxNumber, Matrix<matrix_size_t> boxes);

#endif // BOXUTILS_H
