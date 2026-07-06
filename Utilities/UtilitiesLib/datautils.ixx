/* General purpose data conversion functions */

module;

#include <iostream>
#include <list>
#include <string>
#include <vector>

// TODO: move macros to consumer or to separate .h file
#define FAIL_DEALLOC(ptr, message)                                                                                     \
    std::free(ptr);                                                                                                    \
    ptr = nullptr;                                                                                                     \
    QFAIL(message);

#define DEALLOC(ptr)                                                                                                   \
    std::free(ptr);                                                                                                    \
    ptr = nullptr;

// TODO: move to separate .h file
#if (defined(__APPLE__) && defined(__MACH__))
#define MACOS
#endif

export module datautils;

using SizeVector = std::vector<size_t>;

using DataWord = std::vector<bool>;
using DataSet = std::vector<DataWord>;

export namespace Utilities
{
bool convertBitStringToDataWord(const std::string& bitString, DataWord& word);
DataWord invertDataWord(const DataWord& word);

/* Convenience function for getting a std::list<DataType>::iterator based on a "virtual index", i.e. number of hops from
 * the starting element */
template <typename DataType>
typename std::list<DataType>::iterator getListIteratorAtIndex(std::list<DataType>& myList, size_t index);

void leftTrimWhiteSpace(std::string& str);
void rightTrimWhiteSpace(std::string& str);
void trimWhiteSpace(std::string& str);
} // namespace Utilities

export std::istream& operator>>(std::istream& in, DataSet& dataSet);
export std::ostream& operator<<(std::ostream& out, const DataSet& dataSet);
export std::ostream& operator<<(std::ostream& out, const DataWord& word);
export std::ostream& operator<<(std::ostream& out, const SizeVector& indexes);
