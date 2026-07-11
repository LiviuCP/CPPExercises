#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

import utils;

static constexpr size_t c_MaskSize{32};

union Primitives {
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

union Endian {
    uint16_t number;
    std::byte bytes[2];
};

int main()
{
    Primitives src, dest;
    Endian endian;

    Utilities::clearScreen();

    std::cout << "Created union. We will test some stuff by using it." << std::endl << std::endl;
    std::cout << "First, let's retrieve the byte sizes of some primitive variable types" << std::endl;

    memset(src.mask, 0xFF, c_MaskSize);

    memset(dest.mask, '\0', c_MaskSize);
    dest.boolean = src.boolean;
    std::cout << "* The size of the boolean is: " << strlen(dest.mask) << " byte(s)" << std::endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.integer = src.integer;
    std::cout << "* The size of the integer is: " << static_cast<int>(strlen(dest.mask)) << " byte(s)" << std::endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.shortInt = src.shortInt;
    std::cout << "* The size of the short integer is: " << strlen(dest.mask) << " byte(s)" << std::endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.longInt = src.longInt;
    std::cout << "* The size of the long integer is: " << strlen(dest.mask) << " byte(s)" << std::endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.longLongInt = src.longLongInt;
    std::cout << "* The size of the long long integer is: " << strlen(dest.mask) << " byte(s)" << std::endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.decFloat = src.decFloat;
    std::cout << "* The size of the float is: " << strlen(dest.mask) << " byte(s)" << std::endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.decDouble = src.decDouble;
    std::cout << "* The size of the double is: " << strlen(dest.mask) << " byte(s)" << std::endl;

    memset(dest.mask, '\0', c_MaskSize);
    dest.decLongDouble = src.decLongDouble;
    std::cout << "* The size of the long double is: " << strlen(dest.mask) << " byte(s)" << std::endl;

    std::cout << std::endl << "Second, let's determine the byte ordering (little/big endian)" << std::endl;
    endian.number = 0xF0F1;

    if (endian.bytes[1] == static_cast<std::byte>(0xF1))
    {
        std::cout << "* The computer architecture is BIG endian" << std::endl;
    }
    else
    {
        std::cout << "* The computer architecture is LITTLE endian" << std::endl;
    }

    std::cout << std::endl;

    return 0;
}
