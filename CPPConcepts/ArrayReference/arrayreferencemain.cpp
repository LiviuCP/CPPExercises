#include <iostream>

using namespace std;

constexpr size_t c_ArraySize{10};

void incrementArrayElements(int (&array)[c_ArraySize]);
void displayArrayInformation(const int (&array)[c_ArraySize]);

int main()
{
    int testArray[c_ArraySize]{2, -5, 4, 2, 3, 4, 8, 0, -1, 2};

    displayArrayInformation(testArray);
    incrementArrayElements(testArray);
    displayArrayInformation(testArray);

    return 0;
}

void incrementArrayElements(int (&array)[10])
{
    cout << "Incrementing array elements" << endl;

    for (auto& element : array)
    {
        ++element;
    }
}

void displayArrayInformation(const int (&array)[c_ArraySize])
{
    cout << "The array elements are: ";

    for (auto element : array)
    {
        cout << element << " ";
    }

    cout << endl;

    int count{0}, sum{0};
    for (auto& element : array)  // actually reference is not required, yet just wanted to test it works (it does but it is not allowed to modify element for obvious reasons)
    {
        sum += element;
        ++count;
    }

    cout << "Their count is " << count << " and sum is " << sum << endl;
}
