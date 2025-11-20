#pragma once

#include <string>

class Point
{
public:
    Point(double x, double y);

    const double& x() const;
    const double& y() const;

private:
    double m_X;
    double m_Y;
};

class ComposedObject
{
public:
    ComposedObject(int32_t int_, int16_t short_, int8_t ch, const std::string& str, const Point& point);

    void convertStringToUpperCase();
    void invertPoint();

    // this method is an example of bad usage of const_cast (see definition for more details)
    void invertInt() const;

    int32_t getInt() const;
    int16_t getShort() const;
    int8_t getChar() const;
    const std::string& getString() const;
    const Point& getPoint() const;

private:
    int32_t m_Int;
    int16_t m_Short;
    int8_t m_Ch;
    std::string m_Str;
    Point m_Point;
};
