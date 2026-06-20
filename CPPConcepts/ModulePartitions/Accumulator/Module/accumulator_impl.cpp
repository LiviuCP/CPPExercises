module accumulator:accumulator_impl;
import :accumulator_header;

template <Accumulateable T> void Accumulator<T>::accumulate(const T& value)
{
    m_AccumulatedValue += value;
}

template <Accumulateable T> void Accumulator<T>::reset()
{
    m_AccumulatedValue = {};
}

template <Accumulateable T> T Accumulator<T>::getAccumulatedValue() const
{
    return m_AccumulatedValue;
}
