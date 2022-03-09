#include <iostream>
#include <cstring>
#include <cstddef>

#include "utils.h"

using namespace std;

static constexpr size_t c_MaskSize{32};

union Primitives
{
    bool boolean;
    int integer;
    short shortInt;
    long longInt;
    long long longLongInt;
    float decFloat;
    double decDouble;
    long double decLongDouble;
    char mask[c_MaskSize];
};

union Endian
{
    uint16_t number;
    byte bytes[2];
};

int main()
{
    Primitives src, dest;
    Endian endian;

    Utilities::clearScreen();

    cout << "Created union. We will test some stuff by using it." << endl << endl;
    cout << "First, let's retrieve the byte sizes of some primitive variable types" << endl;

    memset(src.mask, 0xFF, c_MaskSize);

    memset(dest.mask, '\0', c_MaskSize);
    dest.boolean = src.boolean;
    cout << "* The size of the boolean is: " << strlen(dest.mask) << " byte(s)" << endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.integer = src.integer;
    cout << "* The size of the integer is: " << static_cast<int>(strlen(dest.mask)) << " byte(s)" << endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.shortInt = src.shortInt;
    cout << "* The size of the short integer is: " << strlen(dest.mask) << " byte(s)" << endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.longInt = src.longInt;
    cout << "* The size of the long integer is: " << strlen(dest.mask) << " byte(s)" << endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.longLongInt = src.longLongInt;
    cout << "* The size of the long long integer is: " << strlen(dest.mask) << " byte(s)" << endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.decFloat = src.decFloat;
    cout << "* The size of the float is: " << strlen(dest.mask) << " byte(s)" << endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.decDouble = src.decDouble;
    cout << "* The size of the double is: " << strlen(dest.mask) << " byte(s)" << endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.decLongDouble = src.decLongDouble;
    cout << "* The size of the long double is: " << strlen(dest.mask) << " byte(s)" << endl;

    cout << endl << "Second, let's determine the byte ordering (little/big endian)" << endl;
    endian.number = 0xF0F1;

    if (endian.bytes[1] == static_cast<byte>(0xF1))
    {
        cout << "* The computer architecture is BIG endian" << endl;
    }
    else
    {
        cout << "* The computer architecture is LITTLE endian" << endl;
    }

    cout << endl;

    return 0;
}
