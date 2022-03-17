/* General-purpose utility functions */
#ifndef UTILS_H
#define UTILS_H

//ssize_t is not properly implemented on Windows so it has to be manually defined
#ifdef _WIN32
using ssize_t = long long;
#endif

namespace Utilities
{
    void clearScreen();
}

#endif // UTILS_H
