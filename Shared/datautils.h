/* General purpose data type(def)s and conversion functions*/
#ifndef DATAUTILS_H
#define DATAUTILS_H

#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include <string>

#include "matrix.h"

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

// std::nullopt corresponds to the "negative" value (previously noted -1)
using MatrixPoint = std::pair<std::optional<matrix_size_t>, std::optional<matrix_size_t>>;

using IntMatrix = Matrix<int>;
using IntPair = std::pair<int, int>;
using IntPairMatrix = Matrix<IntPair>;
using StringMatrix = Matrix<std::string>;
using StringSet = std::set<std::string>;
using StringIntPair = std::pair<std::string, int>;
using StringIntPairMatrix = Matrix<StringIntPair>;
using StringIntMap = std::map<std::string, int>;
using IntVector = std::vector<int>;
using IntVectorMatrix = Matrix<IntVector>;
using IntVectorDiff = IntVector::difference_type;
using StringVector = std::vector<std::string>;
using StringVectorMatrix = Matrix<StringVector>;
using IntPairVector = std::vector<std::pair<int, int>>;
using StringIntPairVector = std::vector<StringIntPair>;
using IntList = std::list<int>;
using BoolMatrix = Matrix<bool>;
using SizePair = std::pair<size_t, size_t>;
using TripleSizeTuple = std::tuple<size_t, size_t, size_t>;

using ConstIntZIteratorPair = std::pair<IntMatrix::ConstZIterator, IntMatrix::ConstZIterator>;
using ConstStringIntZIteratorPair = std::pair<StringIntPairMatrix::ConstZIterator, StringIntPairMatrix::ConstZIterator>;

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
}

#endif // DATAUTILS_H
