#include <fstream>
#include <iostream>

#include "matrixutils.h"

std::ifstream& operator>>(std::ifstream& in, Matrix<int>& data)
{
    bool success{false};

    Matrix<int> matrix{};

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

            for (Matrix<int>::ZIterator it{matrix.zBegin()}; it != matrix.zEnd(); ++it)
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

std::ofstream& operator<<(std::ofstream& out, const Matrix<int>& data)
{
    for(int row{0}; row < data.getNrOfRows(); ++row)
    {
        for (Matrix<int>::ConstZIterator it{data.constZRowBegin(row)}; it != data.constZRowEnd(row); ++it)
        {
            out << *it << " ";
        }

        out << "\n";
    }

    return out;
}
