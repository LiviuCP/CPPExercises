/* General-purpose utility functions */
#ifndef UTILS_H
#define UTILS_H

#include <string>

//ssize_t is not properly implemented on Windows so it has to be manually defined
#ifdef _WIN32
using ssize_t = long long;
#endif

namespace Utilities
{
#ifdef _WIN32
    static const std::string c_InputOutputDir{"C:\\InputOutputFiles\\"};
#else
    static const std::string c_InputOutputDir{"/tmp/InputOutputFiles/"};
#endif

    void clearScreen();
}

#endif // UTILS_H
