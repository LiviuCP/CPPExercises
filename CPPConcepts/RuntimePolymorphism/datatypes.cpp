module;

#include <map>
#include <sstream>
#include <utility>

#include "datawrapper.h"

export module datatypes;
import matrix;

export enum class Color : unsigned short
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

export class Thermometer
{
public:
    Thermometer(double temp, bool isDigital = true);
    std::string getInfo() const;

    void setTemperature(double temperature);

private:
    double mTemperature;
    bool mIsDigital;
};

export void display(const Thermometer& thermometer, std::ofstream& out, size_t indentation);

export using BottleCapacity = double;

template <> void display(const BottleCapacity& data, std::ofstream& out, size_t indentation);

template <> void display<Color>(const Color& data, std::ofstream& out, size_t indentation);

export using ColorPair = std::pair<Color, Color>;

template <> void display<ColorPair>(const ColorPair& data, std::ofstream& out, size_t indentation);

template <> size_t countContent<ColorPair>(const ColorPair&);

export using FoodMix = std::pair<std::string, std::string>;

template <> void display<FoodMix>(const FoodMix& data, std::ofstream& out, size_t position);

export using ColorBoard = Matrix<Color>;

/* Template specialization is not mandatory for any of the datatypes.
   It is sufficient to follow the function signature rules, e.g. display(const [DataType]& type, std::ostream& out,
   size_t position).
*/
export void display(const ColorBoard& data, std::ofstream& out, size_t);
export size_t countContent(const ColorBoard& data);

/* IMPLEMENTATIONS */

const std::map<Color, std::string> c_ColorMapping{
    {Color::RED, "red"},   {Color::GREEN, "green"},   {Color::BLUE, "blue"},     {Color::MAGENTA, "magenta"},
    {Color::LILA, "lila"}, {Color::ORANGE, "orange"}, {Color::YELLOW, "yellow"}, {Color::BROWN, "brown"}};

/***************************************************************************************************************************************************/

Thermometer::Thermometer(double temp, bool isDigital)
    : mTemperature{temp}
    , mIsDigital{isDigital}
{
}

std::string Thermometer::getInfo() const
{
    std::string info{mIsDigital ? "digital" : "analogic"};

    info += " thermometer, temperature: ";

    std::ostringstream out;

    out << mTemperature;
    info += out.str() + "C";

    return info;
}

void Thermometer::setTemperature(double temperature)
{
    mTemperature = temperature;
}

void display(const Thermometer& thermometer, std::ofstream& out, size_t indentation)
{
    out << std::string(indentation, ' ') << thermometer.getInfo() << std::endl;
}

/***************************************************************************************************************************************************/

template <> void display<BottleCapacity>(const BottleCapacity& data, std::ofstream& out, size_t indentation)
{
    out << std::string(indentation, ' ') << data << "l bottle" << std::endl;
}

/***************************************************************************************************************************************************/

template <> void display<Color>(const Color& data, std::ofstream& out, size_t indentation)
{
    if (out.is_open())
    {
        out << std::string(indentation, ' ') << "single color " << c_ColorMapping.at(data) << std::endl;
    }
    else
    {
        assert(false);
    }
}

/***************************************************************************************************************************************************/

template <> void display<ColorPair>(const ColorPair& data, std::ofstream& out, size_t indentation)
{
    if (out.is_open())
    {
        out << std::string(indentation, ' ') << "(" << c_ColorMapping.at(data.first) << ", "
            << c_ColorMapping.at(data.second) << ")" << std::endl;
    }
    else
    {
        assert(false);
    }
}

template <> size_t countContent<ColorPair>(const ColorPair&)
{
    return 2;
}

/***************************************************************************************************************************************************/

template <> void display<FoodMix>(const FoodMix& data, std::ofstream& out, size_t indentation)
{
    if (out.is_open())
    {
        out << std::string(indentation, ' ') << "" << data.first << " + " << data.second << "" << std::endl;
    }
    else
    {
        assert(false);
    }
}

/***************************************************************************************************************************************************/

void display(const ColorBoard& data, std::ofstream& out, size_t indentation)
{
    if (out.is_open())
    {
        for (matrix_size_t rowNr{0}; rowNr < data.getNrOfRows(); ++rowNr)
        {
            out << std::string(indentation, ' ') << "[";

            for (matrix_size_t columnNr{0}; columnNr < data.getNrOfColumns(); ++columnNr)
            {
                out << " (" << c_ColorMapping.at(data.at(rowNr, columnNr)) << ") ";
            }

            out << "]" << std::endl;
        }
    }
    else
    {
        assert(false);
    }
}

size_t countContent(const ColorBoard& data)
{
    return data.getNrOfRows() * data.getNrOfColumns();
}
