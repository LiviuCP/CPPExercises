#ifndef VARIADICTEMPLATES_H
#define VARIADICTEMPLATES_H

#include <utility>
#include <cstdlib>

template<typename DataType> using ValueSizePair = std::pair<DataType, size_t>;

namespace Variadic {
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryLeftFoldMinus(DataType initialValue, ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryLeftFoldMinus(ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryRightFoldMinus(DataType initialValue, ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryRightFoldMinus(ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryLeftFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryLeftFoldMinusRecursiveImplementation(ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryRightFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryRightFoldMinusRecursiveImplementation(ArgumentsList... argumentsList);

    // helper functions
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> addArgumentsToInitialValue(DataType initialValue, ArgumentsList... argumentsList);
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryRightFoldMinusRecursiveImplementationHelper(DataType initialValue, ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryLeftFoldMinus(DataType initialValue, ArgumentsList... argumentsList)
    {
        const DataType c_Result{(initialValue-...-argumentsList)};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryLeftFoldMinus(ArgumentsList... argumentsList)
    {
        const DataType c_Result{(...-argumentsList)};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryRightFoldMinus(DataType initialValue, ArgumentsList... argumentsList)
    {
        const DataType c_Result{(argumentsList-...-initialValue)};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryRightFoldMinus(ArgumentsList... argumentsList)
    {
        const DataType c_Result{(argumentsList-...)};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        return {c_Result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryLeftFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList)
    {
        DataType result{initialValue};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        if constexpr (c_Count > 0)
        {
            result -= addArgumentsToInitialValue(argumentsList...).first;
        }

        return {result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryLeftFoldMinusRecursiveImplementation(ArgumentsList... argumentsList)
    {
        static_assert(sizeof...(argumentsList) > 0, "Minimum one argument is required in the parameters pack!");

        auto[result, count]{binaryLeftFoldMinusRecursiveImplementation(argumentsList...)};
        return {result, count + 1};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryRightFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList)
    {
        DataType result{initialValue};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        if constexpr (c_Count > 0)
        {
            if constexpr (1 == c_Count % 2)
            {
                result = binaryRightFoldMinusRecursiveImplementationHelper(initialValue, argumentsList...).first;
            }
            else
            {
                // match total number of arguments to be odd so the helper function can evaluate and calculate the required value correctly
                result += binaryRightFoldMinusRecursiveImplementationHelper(argumentsList..., DataType{}).first;
            }
        }

        return {result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> unaryRightFoldMinusRecursiveImplementation(ArgumentsList... argumentsList)
    {
        constexpr size_t c_Count{sizeof...(argumentsList)};
        static_assert(c_Count > 0, "Minimum one argument is required in the parameters pack!");

        DataType result{};

        if constexpr (0 == c_Count % 2)
        {
            /* this sign correction is required because for an even number of pack arguments the called binary method
               (that uses the first argument from pack as initial value) inverts the sign of all operands (comparing to requirement)
            */
            result = -binaryRightFoldMinusRecursiveImplementation(argumentsList...).first;
        }
        else
        {
            // the binary implementation should be called with an odd number of arguments within parameter pack so a "neutral" initial value (DataType{}) should be chosen
            result = binaryRightFoldMinusRecursiveImplementation(DataType{}, argumentsList...).first;
        }

        return {result, c_Count};
    }

    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> addArgumentsToInitialValue(DataType initialValue, ArgumentsList... argumentsList)
    {
        DataType result{initialValue};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        if constexpr (c_Count > 0)
        {
            result += addArgumentsToInitialValue(argumentsList...).first;
        }

        return {result, c_Count};
    }

    /* This function works correctly in implementing the binary right fold minus behavior only when called with an odd number of argumentsList... items (initialValue not taken into account)
       For this reason:
        - it is not advisable to use it directly (instead wrap it into another function, like binaryRightFoldMinusRecursiveImplementation())
        - prior to using it the argumentsList... parameters count should be matched in order to ensure it is an odd number (see binaryRightFoldMinusRecursiveImplementation() for more details)
    */
    template<typename DataType, typename ...ArgumentsList> ValueSizePair<DataType> binaryRightFoldMinusRecursiveImplementationHelper(DataType initialValue, ArgumentsList... argumentsList)
    {
        DataType result{initialValue};
        constexpr size_t c_Count{sizeof...(argumentsList)};

        if constexpr (c_Count > 0)
        {
            if constexpr (1 == c_Count % 2)
            {
                result = -result;
            }

            result += binaryRightFoldMinusRecursiveImplementationHelper(argumentsList...).first;
        }

        return {result, c_Count};
    }
}

#endif // VARIADICTEMPLATES_H
