#pragma once

#include <iostream>

class MeteoData
{
public:
    MeteoData();
    MeteoData(int temp, double pressure, float hum);

    void setTemperature(int temp);
    int getTemperature() const;

    void setAtmosphericPressure(double pressure);
    double getAtmosphericPressure() const;

    void setHumidity(float hum);
    float getHumidity() const;

    friend std::ostream& operator<<(std::ostream& out, const MeteoData& data);

private:
    int temperature;
    double atmosphericPressure;
    float humidity;
};
