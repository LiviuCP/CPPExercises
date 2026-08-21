// When building the project on Mac using Ninja and LLVM Clang, I noticed
// that a compilation error would be triggered unless adding export to the
// implementation partition.
// On Linux the export itself triggers a compilation error!
#if defined(__APPLE__) && defined(__MACH__)
export module accumulator:accumulator_impl;
#else
module accumulator:accumulator_impl;
#endif

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
