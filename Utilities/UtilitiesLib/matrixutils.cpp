#include "matrixutils.h"

/* This file is required for preventing any link errors on X-Code (MacOS).
   Neither the file nor the below dummy function should be deleted.
*/

// used for suppressing warning message
int dummyFunctionMatrixUtilsCPP()
{
    return 0;
}

SizeVector Utilities::toSizeVector(const MatrixSizeVector& matrixSizeVector)
{
    SizeVector sizeVector;
    sizeVector.reserve(matrixSizeVector.size());
    std::transform(matrixSizeVector.begin(), matrixSizeVector.end(), std::back_inserter(sizeVector),
                   [](const auto& element) { return static_cast<size_t>(element); });

    return sizeVector;
}
