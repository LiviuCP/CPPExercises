#include <iostream>

#include "vgadisplay.h"
#include "vgaadapter.h"

VGAAdapter::VGAAdapter(VGADisplay* display)
    : HDMIDisplay()
    , m_pVGADisplay{display}
{
    if (m_pVGADisplay)
    {
        m_SerialNumber = std::to_string(m_pVGADisplay->getSerNr());
    }
}

VGAAdapter::~VGAAdapter()
{
    m_pVGADisplay = nullptr;
}

void VGAAdapter::displayOutput()
{
    if (m_pVGADisplay)
    {
        m_pVGADisplay->displayVGAOutput();
    }
    else
    {
        std::cout << "No display is connected to HDMI card" << std::endl;
    }
}
