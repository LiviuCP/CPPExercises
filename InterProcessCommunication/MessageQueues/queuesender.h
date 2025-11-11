#pragma once

#include <string>

#include "queuedatatypes.h"

class QueueSender
{
public:
    QueueSender(std::string queueFile);
    void writeToQueue(void* data, DataTypes dataType);
private:
    void _createQueueFile();
    void _retrieveQueueId();

    std::string m_QueueFilename;
    int m_QueueId;
};
