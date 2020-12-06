#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

#include <fstream>

#include "../External/Matrix/MatrixLib/matrix.h"

std::ifstream& operator>>(std::ifstream& in, Matrix<int>& data);
std::ofstream& operator<<(std::ofstream& out, const Matrix<int>& data);

#endif // MATRIXUTILS_H
