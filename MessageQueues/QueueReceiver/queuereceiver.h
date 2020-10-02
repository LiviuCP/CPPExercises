#ifndef QUEUERECEIVER_H
#define QUEUERECEIVER_H

#include <string>

#include "../QueueUtils/queuedatatypes.h"

class QueueReceiver
{
public:
    QueueReceiver(std::string queueFile);

    void readFromQueue(void* data, DataTypes dataType);
    void removeQueue();

private:
    void _retrieveQueueId();

    std::string m_QueueFilename;
    int m_QueueId;
};

#endif // QUEUERECEIVER_H
