#include <iostream>

#include "hdmicard.h"
#include "hdmidisplay.h"

HDMICard::HDMICard()
    : m_pDisplay{nullptr}
{
}

void HDMICard::connectHDMIDisplay(HDMIDisplay* display)
{
    m_pDisplay = display;
}

void HDMICard::disconnectHDMIDisplay()
{
    m_pDisplay = nullptr;
}

void HDMICard::controlDisplay()
{
    if (m_pDisplay)
    {
        m_pDisplay->displayOutput();
    }
    else
    {
        std::cout << "No display is connected to HDMI card" << std::endl;
    }
}

void HDMICard::requestDisplaySerialNumber() const
{
    if (m_pDisplay)
    {
        std::cout << "Display sent following serial number: " << m_pDisplay->getSerialNumber() << std::endl;
    }
    else
    {
        std::cout << "No display is connected to HDMI card" << std::endl;
    }
}
