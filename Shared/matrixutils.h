/* Each CPPExercises subproject that includes matrixutils.h should have following path setup in INCLUDEPATH (relative to the subproject dir):
   1) CPPExercises/External/Matrix/MatrixLib/Matrix
   2) CPPExercises/External/Matrix/MatrixLib/Utils
*/

#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

#include <fstream>

#include "../External/Matrix/MatrixLib/Matrix/matrix.h"

template<typename T>
std::ifstream& operator>>(std::ifstream& in, Matrix<T>& data);

template<typename T>
std::ofstream& operator<<(std::ofstream& out, const Matrix<T>& data);

template<typename T>
std::ifstream& operator>>(std::ifstream& in, Matrix<T>& data)
{
    bool success{false};

    Matrix<T> matrix{};

    int nrOfRows{0};
    int nrOfColumns{0};

    in >> nrOfRows;

    if (!in.fail() && nrOfRows > 0)
    {
        in >> nrOfColumns;

        if (!in.fail() && nrOfColumns > 0)
        {
            matrix.resize(nrOfRows, nrOfColumns);
            success = true;

            for (typename Matrix<T>::ZIterator it{matrix.zBegin()}; it != matrix.zEnd(); ++it)
            {
                in >> *it;

                if (in.fail())
                {
                    success = false;
                    break;
                }
            }

            if (success)
            {
                data = std::move(matrix);
            }
        }
    }

    // ensure an empty matrix is provided if reading has not been done successfully
    if (!success)
    {
        data.clear();
    }

    return in;
}

template<typename T>
std::ofstream& operator<<(std::ofstream& out, const Matrix<T>& data)
{
    for(int row{0}; row < data.getNrOfRows(); ++row)
    {
        for (typename Matrix<T>::ConstZIterator it{data.constZRowBegin(row)}; it != data.constZRowEnd(row); ++it)
        {
            out << *it << " ";
        }

        out << "\n";
    }

    return out;
}


#endif // MATRIXUTILS_H
