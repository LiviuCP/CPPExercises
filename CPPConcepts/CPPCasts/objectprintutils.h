#pragma once

#include <iostream>

class Point;
class ComposedObject;

std::ostream& operator<<(std::ostream& out, const Point& point);
std::ostream& operator<<(std::ostream& out, const ComposedObject& object);
