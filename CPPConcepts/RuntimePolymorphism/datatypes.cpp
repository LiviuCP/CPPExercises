#include <sstream>

#include "datatypes.h"
#include "datautils.h"

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

template<>
void display<BottleCapacity>(const BottleCapacity& data, std::ofstream& out, size_t indentation)
{
    out << std::string(indentation, ' ') << data << "l bottle" << std::endl;
}

/***************************************************************************************************************************************************/

template<>
void display<Color>(const Color& data, std::ofstream &out, size_t indentation)
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

template<>
void display<ColorPair>(const ColorPair& data, std::ofstream& out, size_t indentation)
{
    if (out.is_open())
    {
        out << std::string(indentation, ' ') << "(" << c_ColorMapping.at(data.first) << ", " << c_ColorMapping.at(data.second) << ")" << std::endl;
    }
    else
    {
        assert(false);
    }
}

template<>
size_t countContent<ColorPair>(const ColorPair&)
{
    return 2;
}

/***************************************************************************************************************************************************/

template<>
void display<FoodMix>(const FoodMix& data, std::ofstream& out, size_t indentation)
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
                out << " ("<< c_ColorMapping.at(data.at(rowNr, columnNr)) << ") ";
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
