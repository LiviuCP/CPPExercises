#pragma once

class HDMIDisplay;

/* client class, connects either to target (HDMIDisplay) or adaptee (VGADisplay - via adapter) */
class HDMICard
{
public:
    HDMICard();
    void connectHDMIDisplay(HDMIDisplay* display);
    void disconnectHDMIDisplay();
    void controlDisplay();
    void requestDisplaySerialNumber() const;

private:
    HDMIDisplay* m_pDisplay;
};
