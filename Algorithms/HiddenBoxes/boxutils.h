#ifndef BOXUTILS_H
#define BOXUTILS_H

#include "matrix.h"

void retrieveFittingBoxes(const Matrix<int>& boxes, Matrix<int>& fittingBoxIndexes);
bool boxFitsIntoBox(int fittingBoxNumber, int includingBoxNumber, Matrix<int> boxes);

#endif // BOXUTILS_H
