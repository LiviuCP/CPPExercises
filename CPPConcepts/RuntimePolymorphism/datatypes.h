#pragma once

#include <map>
#include <utility>

#include "datawrapper.h"
#include "matrix.h"

enum class Color : unsigned short
{
    RED,
    GREEN,
    BLUE,
    MAGENTA,
    LILA,
    ORANGE,
    YELLOW,
    BROWN
};

const std::map<Color, std::string> c_ColorMapping{
    {Color::RED, "red"},   {Color::GREEN, "green"},   {Color::BLUE, "blue"},     {Color::MAGENTA, "magenta"},
    {Color::LILA, "lila"}, {Color::ORANGE, "orange"}, {Color::YELLOW, "yellow"}, {Color::BROWN, "brown"}};

class Thermometer
{
public:
    Thermometer(double temp, bool isDigital = true);
    std::string getInfo() const;

    void setTemperature(double temperature);

private:
    double mTemperature;
    bool mIsDigital;
};

void display(const Thermometer& thermometer, std::ofstream& out, size_t indentation);

using BottleCapacity = double;

void display(const BottleCapacity& data, std::ofstream& out, size_t indentation);

template <> void display<Color>(const Color& data, std::ofstream& out, size_t indentation);

using ColorPair = std::pair<Color, Color>;

template <> void display<ColorPair>(const ColorPair& data, std::ofstream& out, size_t indentation);

template <> size_t countContent<ColorPair>(const ColorPair&);

using FoodMix = std::pair<std::string, std::string>;

template <> void display<FoodMix>(const FoodMix& data, std::ofstream& out, size_t position);

using ColorBoard = Matrix<Color>;

/* Template specialization is not mandatory for any of the datatypes.
   It is sufficient to follow the function signature rules, e.g. display(const [DataType]& type, std::ostream& out,
   size_t position).
*/
void display(const ColorBoard& data, std::ofstream& out, size_t);
size_t countContent(const ColorBoard& data);
