/* General purpose data type(def)s and conversion functions*/
#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>

#define FAIL_DEALLOC(ptr, message)                                                                                     \
    std::free(ptr);                                                                                                    \
    ptr = nullptr;                                                                                                     \
    QFAIL(message);

#define DEALLOC(ptr)                                                                                                   \
    std::free(ptr);                                                                                                    \
    ptr = nullptr;

#if (defined(__APPLE__) && defined(__MACH__))
#define MACOS
#endif

using SizeVector = std::vector<size_t>;

using DataWord = std::vector<bool>;
using DataSet = std::vector<DataWord>;

namespace Utilities
{
bool convertBitStringToDataWord(const std::string& bitString, DataWord& word);
DataWord invertDataWord(const DataWord& word);

/* Convenience function for getting a std::list<DataType>::iterator based on a "virtual index", i.e. number of hops from
 * the starting element */
template <typename DataType>
typename std::list<DataType>::iterator getListIteratorAtIndex(std::list<DataType>& myList, size_t index)
{
    auto it{myList.begin()};

    while (index > 0 && it != myList.end())
    {
        ++it;
        --index;
    }

    return it;
}

void leftTrimWhiteSpace(std::string& str);
void rightTrimWhiteSpace(std::string& str);
void trimWhiteSpace(std::string& str);
} // namespace Utilities

std::istream& operator>>(std::istream& in, DataSet& dataSet);
std::ostream& operator<<(std::ostream& out, const DataSet& dataSet);
std::ostream& operator<<(std::ostream& out, const DataWord& word);
std::ostream& operator<<(std::ostream& out, const SizeVector& indexes);
