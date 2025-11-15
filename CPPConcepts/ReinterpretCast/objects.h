#pragma once
#pragma pack(1)

#include <string>

class Point
{
public:
    Point(double x, double y);

    double x() const;
    double y() const;

private:
    double m_X;
    double m_Y;
};

class ComposedObject
{
public:
    ComposedObject(int32_t int_, int16_t short_, int8_t ch, const std::string& str, const Point& point);

    int32_t getInt() const;
    int16_t getShort() const;
    int8_t getChar() const;
    const std::string& getString() const;
    const Point& getPoint() const;

private:
    // pragma pack ensures that all bytes are contiguous
    // (otherwise reinterpret_cast might fail due to padding bytes)
    int32_t m_Int;
    int16_t m_Short;
    int8_t m_Ch;
    std::string m_Str;
    Point m_Point;
};
