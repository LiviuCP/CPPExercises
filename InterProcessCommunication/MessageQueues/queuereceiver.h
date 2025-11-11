#pragma once

#include <string>

#include "queuedatatypes.h"

class QueueReceiver
{
public:
    QueueReceiver(std::string queueFilename, bool waitForMessage = false);

    void readFromQueue(void* data, DataTypes dataType);
    void removeQueue();

private:
    void _retrieveQueueId();

    std::string m_QueueFilename;
    int m_QueueId;
    int m_Flags;
};
