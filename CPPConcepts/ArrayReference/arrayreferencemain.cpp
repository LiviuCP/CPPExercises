#include <iostream>

import utils;

constexpr size_t c_ArraySize{10};

void incrementArrayElements(int (&array)[c_ArraySize]);
void displayArrayInformation(const int (&array)[c_ArraySize]);

int main()
{
    int testArray[c_ArraySize]{2, -5, 4, 2, 3, 4, 8, 0, -1, 2};

    Utilities::clearScreen();

    displayArrayInformation(testArray);
    incrementArrayElements(testArray);
    displayArrayInformation(testArray);

    return 0;
}

void incrementArrayElements(int (&array)[10])
{
    std::cout << "Incrementing array elements" << std::endl;

    for (auto& element : array)
    {
        ++element;
    }
}

void displayArrayInformation(const int (&array)[c_ArraySize])
{
    std::cout << "The array elements are: ";

    for (auto element : array)
    {
        std::cout << element << " ";
    }

    std::cout << std::endl;

    int count{0}, sum{0};
    for (auto& element : array) // actually reference is not required, yet just wanted to test it works (it does but it
                                // is not allowed to modify element for obvious reasons)
    {
        sum += element;
        ++count;
    }

    std::cout << "Their count is " << count << " and sum is " << sum << std::endl;
}
