#ifndef VARIADICTEMPLATES_H
#define VARIADICTEMPLATES_H

#include <utility>
#include <cstdlib>

template<typename DataType> using ValueSizePair = std::pair<DataType, size_t>;

namespace Variadic {
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryLeftFoldMinus(DataType initialValue, ArgumentsList... argumentsList)
    {
        const DataType c_Result{(initialValue-...-argumentsList)};
        const size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryLeftFoldMinus(ArgumentsList... argumentsList)
    {
        const DataType c_Result{(...-argumentsList)};
        const size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryRightFoldMinus(DataType initialValue, ArgumentsList... argumentsList)
    {
        const DataType c_Result{(argumentsList-...-initialValue)};
        const size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryRightFoldMinus(ArgumentsList... argumentsList)
    {
        const DataType c_Result{(argumentsList-...)};
        const size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }
}

#endif // VARIADICTEMPLATES_H
