/* General-purpose utility functions */
#pragma once

#include <string>

// ssize_t is not properly implemented on Windows so it has to be manually defined
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
} // namespace Utilities
