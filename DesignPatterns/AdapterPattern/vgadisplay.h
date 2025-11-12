#pragma once

/* This class is the adaptee. The client (HDMICard) doesn't "know" the adaptee functions so a direct connection cannot
   be established. An adapter is required to match the VGADisplay functionality to the required one (HDMIDisplay) */
class VGADisplay
{
public:
    VGADisplay(unsigned short serNr);
    void displayVGAOutput();
    unsigned int getSerNr() const;

private:
    unsigned short m_SerNr;
};
