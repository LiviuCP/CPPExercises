#ifndef HDMI_H
#define HDMI_H

#include <string>

/* This class is the target.
   The client (HDMI card) is aware of its methods and can directly connect to it. */
class HDMIDisplay
{
public:
    HDMIDisplay(std::string serialNumber);
    virtual ~HDMIDisplay();
    std::string getSerialNumber();

    virtual void displayOutput();

protected:
    HDMIDisplay(); // only to be used by adapter class which builds serial number string based on passed adaptee object

    std::string m_SerialNumber;
};



#endif // HDMI_H
