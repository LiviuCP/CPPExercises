#include "objectprintutils.h"

// same contiguous memory requirement for objects as in the other files
#pragma pack(1)

#include "objects.h"

std::ostream& operator<<(std::ostream& out, const Point& point)
{
    out << "(" << point.x() << ", " << point.y() << ")";
    return out;
}

std::ostream& operator<<(std::ostream& out, const ComposedObject& object)
{
    out << "\tint32_t: " << object.getInt() << "\n";
    out << "\tint16_t: " << object.getShort() << "\n";
    out << "\tint8_t: " << object.getChar() << "\n";
    out << "\tstd::string: " << object.getString() << "\n";
    out << "\tpoint: " << object.getPoint() << "\n";

    return out;
}
