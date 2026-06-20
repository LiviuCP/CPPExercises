#include <cassert>
#include <print>
#include <string>
#include <vector>

import accumulator;

template <typename T> void accumulateMultipleValues(Accumulator<T>& accumulator, const std::vector<T>& values)
{
    for (const auto& value : values)
    {
        accumulator.accumulate(value);
    }
}

/* Mini-application that illustrates the usage of C++2X module partitions

   The goal is to split a template class (Accumulator) into a declarations (header) and definitions (implementation)
   file. In pre-C++20 standards this wouldn't be possible without some "hacks" (normally the class declarations and
   definitions would need to live in the same .h file).
*/

int main()
{
    const std::vector<int> c_IntValues{-2, 3, 5, 4};
    const std::vector<double> c_DecimalValues{5.4, 2.3, -4.8, 0.2};
    const std::vector<std::string> c_StringValues{"This ", "is ", "Sparta", "!"};

    Accumulator<int> intAccumulator;
    Accumulator<double> decimalAccumulator;
    Accumulator<std::string> stringAccumulator;

    accumulateMultipleValues(intAccumulator, c_IntValues);
    accumulateMultipleValues(decimalAccumulator, c_DecimalValues);
    accumulateMultipleValues(stringAccumulator, c_StringValues);

    std::println("The accumulated integer value of {} is: {}", c_IntValues, intAccumulator.getAccumulatedValue());
    std::println("The accumulated decimal value of {} is: {:.1f}", c_DecimalValues,
                 decimalAccumulator.getAccumulatedValue());
    std::println("The accumulated string value of {} is: \"{}\"", c_StringValues,
                 stringAccumulator.getAccumulatedValue());

    std::println();
    std::print("Resetting...");

    intAccumulator.reset();
    decimalAccumulator.reset();
    stringAccumulator.reset();

    if (intAccumulator.getAccumulatedValue() == 0 && decimalAccumulator.getAccumulatedValue() == 0.0 &&
        stringAccumulator.getAccumulatedValue().empty())
    {
        std::println("all accumulators successfully reset!");
    }
    else
    {
        assert(false);
        std::println("at least one accumulator hasn't been correctly reset or an error occurred!");
    }

    return 0;
}
