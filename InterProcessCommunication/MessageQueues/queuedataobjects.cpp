#include <cassert>

#include "queuedataobjects.h"

MeteoData::MeteoData()
    : temperature{0}
    , atmosphericPressure{1.0}
    , humidity{0.0f}
{
}

MeteoData::MeteoData(int temp, double pressure, float hum)
    : temperature{temp}
    , atmosphericPressure{pressure}
    , humidity{hum}
{
    assert(atmosphericPressure > 0.0 && "Non-positive atmospheric pressure member detected");
    assert(humidity >= 0.0f && humidity <= 100.0f && "Humidity is not within required range");
}

void MeteoData::setTemperature(int value)
{
    temperature = value;
}

int MeteoData::getTemperature() const
{
    return temperature;
}

void MeteoData::setAtmosphericPressure(double pressure)
{
    assert(atmosphericPressure > 0.0 && "Non-positive atmospheric pressure member detected");
    atmosphericPressure = pressure;
}

double MeteoData::getAtmosphericPressure() const
{
    return atmosphericPressure;
}

void MeteoData::setHumidity(float hum)
{
    assert(humidity >= 0.0f && humidity <= 100.0f && "Humidity is not within required range");
    humidity = hum;
}

float MeteoData::getHumidity() const
{
    return humidity;
}

std::ostream& operator<<(std::ostream& out, const MeteoData& data)
{
    out << "(T: " << data.temperature << "C, P: " << data.atmosphericPressure << "B, H: " << data.humidity << "%)";
    return out;
}
