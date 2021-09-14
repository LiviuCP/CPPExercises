#ifndef CLIENT_H
#define CLIENT_H

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

#endif // CLIENT_H
