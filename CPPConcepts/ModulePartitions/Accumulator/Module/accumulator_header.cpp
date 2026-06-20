module;

#include <concepts>

export module accumulator:accumulator_header;

template <typename T>
concept Accumulateable = std::default_initializable<T> && requires(T a, T b) {
    {
        a += b
    } -> std::convertible_to<T>;
};

/* Accumulator class that uses the += operator to accumulate values
   The starting value is the default initialized value
*/

export template <Accumulateable T> class Accumulator
{
public:
    Accumulator() = default;

    void accumulate(const T& value);
    void reset();

    T getAccumulatedValue() const;

private:
    T m_AccumulatedValue;
};
