#ifndef CUSTOMCPP20CONCEPTS_H
#define CUSTOMCPP20CONCEPTS_H

#include <concepts>

template<typename DataType> concept hasSizeMethod = requires(DataType dataType) // "requires" expression (resulting concept can be fed to a "requires" clause, see below)
{
    {dataType.size()} -> std::same_as<size_t>;
};

template<typename DataType> concept nonBooleanIntegral = std::integral<DataType> && (!std::same_as<DataType, bool>);
template<typename DataType> concept numeric = nonBooleanIntegral<DataType> || std::floating_point<DataType>;

#endif // CUSTOMCPP20CONCEPTS_H
