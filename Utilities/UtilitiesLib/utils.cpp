/* General-purpose utility functions */

module;

#include <cstdlib>
#include <string>

// ssize_t is not properly implemented on Windows so it has to be manually defined
// TODO: move to separate .h file
#ifdef _WIN32
using ssize_t = long long;
#endif

export module utils;

export namespace Utilities
{
#ifdef _WIN32
const std::string c_InputOutputDir{"C:\\InputOutputFiles\\"};
#else
const std::string c_InputOutputDir{"/tmp/InputOutputFiles/"};
#endif

void clearScreen()
{
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    system("clear"); // Linux & Mac
#elif defined(_WIN32)
    system("cls"); // Windows
#endif
}
} // namespace Utilities
