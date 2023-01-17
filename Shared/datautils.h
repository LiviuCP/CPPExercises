/* General purpose data type(def)s and conversion functions*/
#ifndef DATAUTILS_H
#define DATAUTILS_H

#include <vector>
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
using StringIntPairVector = std::vector<StringIntPair>;
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
}

#endif // DATAUTILS_H
