#pragma once

#include <cstdlib>
#include <algorithm>

#include "customcpp20concepts.h"
#include "datautils.h"

namespace Variadic
{
    /* variadic functions created in connection to CPP17 standard (no abbreviated templates or CPP20 concepts applied) */

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> binaryLeftFoldMinus(DataType initialValue, ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> unaryLeftFoldMinus(ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> binaryRightFoldMinus(DataType initialValue, ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> unaryRightFoldMinus(ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> binaryLeftFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> unaryLeftFoldMinusRecursiveImplementation(ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> binaryRightFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> unaryRightFoldMinusRecursiveImplementation(ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    std::pair<DataType,DataType> getMinMaxArgument(const DataType& first, const DataType& second, const ArgumentsList&... argumentsList);

    // helper functions

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> addArgumentsToInitialValue(DataType initialValue, ArgumentsList... argumentsList);

    template<typename DataType, typename ...ArgumentsList>
    ValueSizePair<DataType> binaryRightFoldMinusRecursiveImplementationHelper(DataType initialValue, ArgumentsList... argumentsList);

    /* variadic functions created in connection to CPP20 standard (abbreviated templates and CPP20 concepts applied) */

    template<numeric DataType, std::convertible_to<DataType> ...ArgumentsList>
    DataType computeAverageWithVariadicTemplateAndLambda(ArgumentsList ...argumentsList);

    template<hasAddOperator DataType, std::same_as<DataType> ...ArgumentsList>
    void addBeforeAndAfterWithVariadicTemplateAndLambda(DataType before, DataType after, ArgumentsList& ...argumentsList);

    auto getSumOfArgumentSquares(numeric auto initialValue, std::convertible_to<decltype(initialValue)> auto... argumentsList);

    template<numeric ...ArgumentsList>
    auto createTupleWithAlternatingSigns(const ArgumentsList& ...argumentsList);

    template<numeric DataType, std::same_as<DataType> ...ArgumentsList>
    auto createTupleWithCumulatedValues(DataType initialValue, const ArgumentsList& ...argumentsList);

    template<numeric ...FirstTupleArgs, numeric ...SecondTupleArgs, numeric DataType>
    bool areTuplesEqual(const std::tuple<FirstTupleArgs...>& first, const std::tuple<SecondTupleArgs...>& second, DataType epsilon);

    // helper functions
    auto square(numeric auto value);
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::binaryLeftFoldMinus(DataType initialValue, ArgumentsList... argumentsList)
{
    const DataType c_Result{(initialValue-...-argumentsList)};
    constexpr size_t c_Count{sizeof...(argumentsList)};

    return {c_Result, c_Count};
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::unaryLeftFoldMinus(ArgumentsList... argumentsList)
{
    const DataType c_Result{(...-argumentsList)};
    constexpr size_t c_Count{sizeof...(argumentsList)};

    return {c_Result, c_Count};
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::binaryRightFoldMinus(DataType initialValue, ArgumentsList... argumentsList)
{
    const DataType c_Result{(argumentsList-...-initialValue)};
    constexpr size_t c_Count{sizeof...(argumentsList)};

    return {c_Result, c_Count};
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::unaryRightFoldMinus(ArgumentsList... argumentsList)
{
    const DataType c_Result{(argumentsList-...)};
    constexpr size_t c_Count{sizeof...(argumentsList)};

    return {c_Result, c_Count};
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::binaryLeftFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList)
{
    DataType result{initialValue};
    constexpr size_t c_Count{sizeof...(argumentsList)};

    if constexpr (c_Count > 0)
    {
        result -= addArgumentsToInitialValue(argumentsList...).first;
    }

    return {result, c_Count};
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::unaryLeftFoldMinusRecursiveImplementation(ArgumentsList... argumentsList)
{
    static_assert(sizeof...(argumentsList) > 0, "Minimum one argument is required in the parameters pack!");

    auto[result, count]{binaryLeftFoldMinusRecursiveImplementation(argumentsList...)};
    return {result, count + 1};
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::binaryRightFoldMinusRecursiveImplementation(DataType initialValue, ArgumentsList... argumentsList)
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

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::unaryRightFoldMinusRecursiveImplementation(ArgumentsList... argumentsList)
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

// there should be at least 2 arguments for retrieving min/max value and these arguments (first, second) are the starting point for calculating the two values
template<typename DataType, typename ...ArgumentsList>
std::pair<DataType,DataType> Variadic::getMinMaxArgument(const DataType& first, const DataType& second, const ArgumentsList&... argumentsList)
{
    DataType minValue{std::min(first, second)};
    DataType maxValue{std::max(first, second)};

    (void(minValue = std::min(minValue, argumentsList)),...);
    (void(maxValue = std::max(maxValue, argumentsList)),...);

    return {minValue, maxValue};
}

template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::addArgumentsToInitialValue(DataType initialValue, ArgumentsList... argumentsList)
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
template<typename DataType, typename ...ArgumentsList>
ValueSizePair<DataType> Variadic::binaryRightFoldMinusRecursiveImplementationHelper(DataType initialValue, ArgumentsList... argumentsList)
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

template<numeric DataType, std::convertible_to<DataType> ...ArgumentsList>
DataType Variadic::computeAverageWithVariadicTemplateAndLambda(ArgumentsList... argumentsList)
{
    static_assert(sizeof...(argumentsList) > 0, "At least one argument required for calculating average!");
    static_assert(std::is_convertible_v<decltype(sizeof...(argumentsList)), DataType>);

    auto computeAverage{[argumentsList...]() {
        const DataType c_Sum{(DataType{} + ... + static_cast<DataType>(argumentsList))};
        const DataType c_Average{c_Sum / static_cast<DataType>(sizeof...(argumentsList))};

        return c_Average;
    }};

    return computeAverage();
}

template<hasAddOperator DataType, std::same_as<DataType> ...ArgumentsList>
void Variadic::addBeforeAndAfterWithVariadicTemplateAndLambda(DataType before, DataType after, ArgumentsList& ...argumentsList)
{
    static_assert(sizeof...(argumentsList) > 0, "At least one pack argument required for prepending/appending the given values!");

    auto addBeforeAndAfter{[before, after, &argumentsList...](){
        (void(argumentsList = before + argumentsList + after),...);
    }};

    addBeforeAndAfter();
}

// return type of the function is deduced from the type of the first argument
auto Variadic::getSumOfArgumentSquares(numeric auto initialValue, std::convertible_to<decltype(initialValue)> auto... argumentsList)
{
    return (square(initialValue) + ... + static_cast<decltype(initialValue)>(square(argumentsList)));
}

template<numeric ...ArgumentsList>
auto Variadic::createTupleWithAlternatingSigns(const ArgumentsList& ...argumentsList)
{
    std::tuple result{argumentsList...};

    auto setAlternatingSignsToTupleElements{[&result]<std::size_t... Idx>(std::index_sequence<Idx...>){
        ((std::get<Idx>(result) = (1 == Idx % 2) ? -std::abs(std::get<Idx>(result))
                                                 :  std::abs(std::get<Idx>(result))),...);
    }};

    setAlternatingSignsToTupleElements(std::make_index_sequence<sizeof...(argumentsList)>());

    return result;
}

template<numeric DataType, std::same_as<DataType> ...ArgumentsList>
auto Variadic::createTupleWithCumulatedValues(DataType initialValue, const ArgumentsList& ...argumentsList)
{
    constexpr size_t c_ArgumentsCount{sizeof...(argumentsList)};

    if constexpr (c_ArgumentsCount > 0)
    {
        std::tuple result{argumentsList...};
        DataType currentCumulatedValue{initialValue};

        auto updateTupleWithCumulatedValues{[&result, &currentCumulatedValue]<std::size_t... Idx>(std::index_sequence<Idx...>){
            ((std::get<Idx>(result) += currentCumulatedValue, currentCumulatedValue = std::get<Idx>(result)),...);
        }};

        updateTupleWithCumulatedValues(std::make_index_sequence<c_ArgumentsCount>());

        return result;
    }
    else
    {
        return std::tuple{initialValue};
    }
}

/* For (heterogenous) tuples to be equal three conditions should be fulfilled:
   - equal sizes
   - elements with same indexes should be of the same type (including sign)
   - elements with same indexes should have the same value (for non-floating point types)
     or the differences between values should be smaller than the provided epsilon (for floating point types)
*/
template<numeric ...FirstTupleArgs, numeric ...SecondTupleArgs, numeric DataType>
bool Variadic::areTuplesEqual(const std::tuple<FirstTupleArgs...>& first, const std::tuple<SecondTupleArgs...>& second, DataType epsilon)
{
    auto areTupleTypesAndValuesEqual{[&first, &second, epsilon]<std::size_t... Idx>(std::index_sequence<Idx...>){
        auto areValuesEqual{[epsilon](auto first, auto second){
            if constexpr (std::is_floating_point_v<decltype(first)>)
            {
                static_assert(std::is_convertible_v<DataType, decltype(first)>);
                return std::abs(first - second) < std::abs(static_cast<decltype(first)>(epsilon));
            }
            else
            {
                return first == second;
            }
        }};

        bool areEqualTuples{false};

        if constexpr (((std::is_same_v<decltype(std::get<Idx>(first)), decltype(std::get<Idx>(second))>)&&...))
        {
            areEqualTuples = ((areValuesEqual(std::get<Idx>(first), std::get<Idx>(second))) &&...);
        }

        return areEqualTuples;
    }};

    bool result{false};

    if constexpr (sizeof...(FirstTupleArgs) == sizeof...(SecondTupleArgs))
    {
        if constexpr (0 == sizeof...(FirstTupleArgs))
        {
            result = true;
        }
        else
        {
            result = areTupleTypesAndValuesEqual(std::make_index_sequence<sizeof...(FirstTupleArgs)>());
        }
    }

    return result;
}

auto Variadic::square(numeric auto value)
{
    return value * value;
}
