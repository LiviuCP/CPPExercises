/* General-purpose utility functions */
#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>

namespace Utilities
{
    void clearScreen()
    {
    #if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
        system("clear"); // Linux & Mac
    #else
        system("cls"); // Windows
    #endif
    }
}

#endif // UTILS_H
