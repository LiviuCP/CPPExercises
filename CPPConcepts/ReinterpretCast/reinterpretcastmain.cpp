#pragma pack(1)

#include <algorithm>
#include <cctype>
#include <iostream>

#include "objects.h"
#include "utils.h"

/* This small application has been written just for fun!
   This should NEVER be applied in real-world scenarios as it is an obvious violation of the elementary encapsulation
   principles of OOP!

   Otherwise mentioned: "Do not try this at home!"
*/

void printObject(const ComposedObject& object);

int main()
{
    Utilities::clearScreen();

    ComposedObject object{-5, 2, 'a', "abcdefghij", {-1.05, 2.99}};

    std::cout << "Before modifications the object had following values: \n";
    printObject(object);

    // modify the private members of the object "from outside"
    // (including the members of the point which are private to the containing object as well)

    int* intPtr{reinterpret_cast<int*>(&object)};
    short* shortPtr = reinterpret_cast<short*>(intPtr + 1);
    char* charPtr{reinterpret_cast<char*>(shortPtr + 1)};
    std::string* strPtr{reinterpret_cast<std::string*>(charPtr + 1)};
    double* pointXPtr{reinterpret_cast<double*>(strPtr + 1)};
    double* pointYPtr{reinterpret_cast<double*>(pointXPtr + 1)};

    const int c_TempInt = *intPtr;
    *intPtr = *shortPtr;
    *shortPtr = static_cast<short>(c_TempInt);

    *charPtr = std::toupper(*charPtr);

    std::reverse(strPtr->begin(), strPtr->end());

    *pointXPtr *= -1.0;
    *pointYPtr *= -1.0;

    std::cout << "\nAfter \"illegal\" modifications the object has following values: \n";
    printObject(object);

    return 0;
}

void printObject(const ComposedObject& object)
{
    std::cout << "\tInt value: " << object.getInt() << "\n";
    std::cout << "\tShort value: " << object.getShort() << "\n";
    std::cout << "\tChar value: " << object.getChar() << "\n";
    std::cout << "\tString value: " << object.getString() << "\n";

    const Point& point{object.getPoint()};

    std::cout << "\tPoint value: (" << point.x() << ", " << point.y() << ")\n";
}
