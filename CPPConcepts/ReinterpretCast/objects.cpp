// this ensures that all Point/ComposedObject member variables are contiguously placed in memory
#pragma pack(1)

#include "objects.h"

Point::Point(double x, double y)
    : m_X{x}
    , m_Y{y}
{
}

double Point::x() const
{
    return m_X;
}

double Point::y() const
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
