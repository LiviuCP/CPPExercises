/* This application implements the adapter pattern

   The involved objects are:
   - a client class (HDMICard)
   - a target class (HDMIDisplay): client can directly access it
   - an adaptee (VGADisplay): client cannot directly access it as the methods are not known to client
   - an adapter (VGAAdapter): required by client to connect to adaptee

   Possible connections:
   - HDMICard -> HDMIDisplay
   - HDMICard -> VGAAdapter -> VGADisplay
*/

#include <iostream>

#include "hdmicard.h"
#include "hdmidisplay.h"
#include "vgaadapter.h"
#include "vgadisplay.h"

import utils;

int main()
{
    Utilities::clearScreen();

    std::cout << "We bought a new HDMI card" << std::endl;
    HDMICard card;
    card.controlDisplay();

    std::cout << "We connect the card to a modern HDMI display" << std::endl;
    HDMIDisplay newDisplay{"SN-044055"};
    card.connectHDMIDisplay(&newDisplay);
    card.controlDisplay();
    card.requestDisplaySerialNumber();
    card.disconnectHDMIDisplay();

    std::cout << std::endl << "Now we want to see if the old VGA monitor still works. " << std::endl;
    std::cout << "For this we are using a HDMI-to-VGA adapter" << std::endl;
    VGADisplay oldDisplay{45234u};
    VGAAdapter adapter{&oldDisplay};
    card.connectHDMIDisplay(&adapter);
    card.controlDisplay();
    card.requestDisplaySerialNumber();

    return 0;
}
