#include <iostream>

#include "vgadisplay.h"

VGADisplay::VGADisplay(unsigned short serNr)
    : m_SerNr{serNr}
{
}

void VGADisplay::displayVGAOutput()
{
    std::cout << "The VGA display is showing an acceptable SD (1024 x 768) output!" << std::endl;
}

unsigned int VGADisplay::getSerNr() const
{
    return m_SerNr;
}
