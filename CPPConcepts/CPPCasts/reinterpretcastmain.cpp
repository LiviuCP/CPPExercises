#include <algorithm>
#include <cctype>

#include "objectprintutils.h"
#include "utils.h"

// this ensures that all Point/ComposedObject member variables are contiguously placed in memory
// (otherwise reinterpret_cast might fail due to padding bytes)
#pragma pack(1)

#include "objects.h"

/* This small application has been written just for fun!
   This should NEVER be applied in real-world scenarios as it is an obvious violation of the elementary encapsulation
   principles of OOP!

   Otherwise mentioned: "Do not try this at home!"
*/

int main()
{
    ComposedObject object{-5, 2, 'a', "abcdefghij", {-1.05, 2.99}};

    Utilities::clearScreen();
    std::cout << "Initial object values: \n" << object;

    // modify the private members of the object "from outside"
    // (including the members of the point which are private to the containing object as well)

    int* intPtr{reinterpret_cast<int*>(&object)};
    short* shortPtr = reinterpret_cast<short*>(intPtr + 1);
    char* charPtr{reinterpret_cast<char*>(shortPtr + 1)};
    std::string* strPtr{reinterpret_cast<std::string*>(charPtr + 1)};
    double* pointXPtr{reinterpret_cast<double*>(strPtr + 1)};
    double* pointYPtr{reinterpret_cast<double*>(pointXPtr + 1)};

    // in this particular case it is safe to swap the int and short as we know the values and there is no overflow risk
    const int c_TempInt = *intPtr;
    *intPtr = *shortPtr;
    *shortPtr = static_cast<short>(c_TempInt);

    *charPtr = std::toupper(*charPtr);

    std::reverse(strPtr->begin(), strPtr->end());

    *pointXPtr *= -1.0;
    *pointYPtr *= -1.0;

    std::cout << "Modified object values: \n" << object;

    return 0;
}
