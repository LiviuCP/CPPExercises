#pragma once

#include "hdmidisplay.h"

class VGADisplay;

/* This class is the adapter used for connecting a HDMI card (client) to a VGA Display (adaptee).
   The client doesn't know the VGA display methods so an adapter is required */
class VGAAdapter : public HDMIDisplay
{
public:
    VGAAdapter(VGADisplay* display);
    ~VGAAdapter() override;

    void displayOutput() override;

private:
    VGADisplay* m_pVGADisplay;
};
