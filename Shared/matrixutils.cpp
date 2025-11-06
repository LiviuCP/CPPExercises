#include "matrixutils.h"

// this (dummy) file is required for preventing any link errors on X-Code (OS-X). Don't fill anything in, don't use it!

// used for suppressing warning message
int dummyFunctionMatrixUtilsCPP()
{
    return 0;
}

SizeVector toSizeVector(const MatrixSizeVector& matrixSizeVector)
{
    SizeVector sizeVector;
    sizeVector.reserve(matrixSizeVector.size());
    std::transform(matrixSizeVector.begin(), matrixSizeVector.end(), std::back_inserter(sizeVector), [](const auto& element){return static_cast<size_t>(element);});

    return sizeVector;
}
