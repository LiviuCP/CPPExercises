#include <algorithm>
#include <cctype>

// this ensures that all Point/ComposedObject member variables are contiguously placed in memory
#pragma pack(1)

#include "objects.h"

Point::Point(double x, double y)
    : m_X{x}
    , m_Y{y}
{
}

const double& Point::x() const
{
    return m_X;
}

const double& Point::y() const
{
    return m_Y;
}

ComposedObject::ComposedObject(int32_t int_, int16_t short_, int8_t ch, const std::string& str, const Point& point)
    : m_Int{int_}
    , m_Short{short_}
    , m_Ch{ch}
    , m_Str{str}
    , m_Point{point}
{
}

void ComposedObject::convertStringToUpperCase()
{
    std::transform(m_Str.cbegin(), m_Str.cend(), m_Str.begin(), [](char ch) { return std::toupper(ch); });
}

// const_cast can be used here as it is presumed the ComposedObject (hence the Point) is mutable
// (no const keyword applied to invertPoint() method)
// however const_casting could easily be avoided by moving this method to the Point class
void ComposedObject::invertPoint()
{
    double* x{const_cast<double*>(&m_Point.x())};
    double* y{const_cast<double*>(&m_Point.y())};

    *x *= -1;
    *y *= -1;
}

// const_casting the "this" pointer is a bad use of the const_cast
// this is because the const method can also be applied to a const ComposedObject
// modifying a const object by casting away its const-ness results in undefined behavior
void ComposedObject::invertInt() const
{
    const_cast<ComposedObject*>(this)->m_Int *= -1;
}

int32_t ComposedObject::getInt() const
{
    return m_Int;
}

int16_t ComposedObject::getShort() const
{
    return m_Short;
}

int8_t ComposedObject::getChar() const
{
    return m_Ch;
}

const std::string& ComposedObject::getString() const
{
    return m_Str;
}

const Point& ComposedObject::getPoint() const
{
    return m_Point;
}
