#include <cstdlib>

#include "utils.h"

void Utilities::clearScreen()
{
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    system("clear"); // Linux & Mac
#elif defined(_WIN32)
    system("cls"); // Windows
#endif
}
