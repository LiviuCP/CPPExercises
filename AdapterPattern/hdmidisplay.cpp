#include <iostream>
#include <cassert>

#include "hdmidisplay.h"

HDMIDisplay::HDMIDisplay(std::string serialNumber)
    : m_SerialNumber{serialNumber}
{
    assert(m_SerialNumber.size() > 0 && "Empty serial number string passed");
}

HDMIDisplay::HDMIDisplay()
    : m_SerialNumber{}
{
}

HDMIDisplay::~HDMIDisplay()
{
}

std::string HDMIDisplay::getSerialNumber()
{
    return m_SerialNumber;
}

void HDMIDisplay::displayOutput()
{
    std::cout << "The HDMI display is showing a clear Full HD (1920 x 1080) output!" << std::endl;
}
