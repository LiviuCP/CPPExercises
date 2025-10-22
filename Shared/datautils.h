/* General purpose data type(def)s and conversion functions*/
#ifndef DATAUTILS_H
#define DATAUTILS_H

#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include <string>

#define FAIL_DEALLOC(ptr, message) \
    std::free(ptr); \
    ptr = nullptr; \
    QFAIL(message);

#define DEALLOC(ptr) \
    std::free(ptr); \
    ptr = nullptr;

#if (defined (__APPLE__) && defined (__MACH__))
#define MACOS
#endif

template<typename DataType>
using ValueSizePair = std::pair<DataType, size_t>;

using IntPair = std::pair<int, int>;
using StringSet = std::set<std::string>;
using StringIntPair = std::pair<std::string, int>;
using StringIntMap = std::map<std::string, int>;
using IntVector = std::vector<int>;
using IntVectorDiff = IntVector::difference_type;
using StringVector = std::vector<std::string>;
using IntPairVector = std::vector<std::pair<int, int>>;
using StringIntPairVector = std::vector<StringIntPair>;
using IntList = std::list<int>;
using SizePair = std::pair<size_t, size_t>;
using TripleSizeTuple = std::tuple<size_t, size_t, size_t>;

using DataWord = std::vector<bool>;
using DataSet = std::vector<DataWord>;

namespace Utilities
{
    bool convertBitStringToDataWord(const std::string& bitString, DataWord& word);
    DataWord invertDataWord(const DataWord& word);

    /* Convenience function for getting a std::list<DataType>::iterator based on a "virtual index", i.e. number of hops from the starting element */
    template<typename DataType>
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
}

#endif // DATAUTILS_H
