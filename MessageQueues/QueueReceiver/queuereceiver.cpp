#include <sys/ipc.h>
#include <sys/msg.h>
#include <cassert>

#include <iostream>

#include "../QueueUtils/queuedatamessages.h"

#include "queuereceiver.h"

#define READ_MESSAGE(messageType, objectType)                                                                                                                           \
{                                                                                                                                                                       \
    messageType message{static_cast<long>(dataType), objectType{}}; /* the message struct needs to be initialized, otherwise the msgrcv is likely to crash */           \
                                                                                                                                                                        \
    if (msgrcv(m_QueueId, &message, sizeof(message), static_cast<long>(dataType), MSG_NOERROR | IPC_NOWAIT) < 0)                                                        \
    {                                                                                                                                                                   \
        fprintf(stderr, "Issues with receiving message\n");                                                                                                             \
    }                                                                                                                                                                   \
    else                                                                                                                                                                \
    {                                                                                                                                                                   \
        *(reinterpret_cast<objectType*>(data)) = message.object;                                                                                                        \
    }                                                                                                                                                                   \
}

constexpr int c_BaseId{543};

QueueReceiver::QueueReceiver(std::string queueFile)
    : m_QueueFilename{queueFile}
    , m_QueueId{-1}
{
    assert(m_QueueFilename.size() > 0 && "Empty string provided for queue file");
    _retrieveQueueId();
}

QueueReceiver::~QueueReceiver()
{
    if (msgctl(m_QueueId, IPC_RMID, nullptr) < 0)
    {
        perror("There was an issue with removing the queue\n");
        exit(-1);
    }
}

void QueueReceiver::readFromQueue(void* data, DataTypes dataType)
{
    if (data)
    {
        switch(dataType)
        {
        case DataTypes::INT:
            READ_MESSAGE(IntMessage, int);
            break;
        case DataTypes::DOUBLE:
            READ_MESSAGE(DoubleMessage, double);
            break;
        case DataTypes::METEODATA:
            READ_MESSAGE(MeteoDataMessage, MeteoData);
            break;
        default:
            assert(false && "Unknown data type");
        }
    }
}

void QueueReceiver::_retrieveQueueId()
{
    const key_t queueKey = ftok(m_QueueFilename.c_str(), c_BaseId);
    if (queueKey < 0)
    {
        perror("The queue key could not be obtained\n");
        exit(-1);
    }

    m_QueueId = msgget(queueKey, 0666 | IPC_CREAT);
    if (m_QueueId < 0)
    {
        perror("The queue id could not be obtained\n");
        exit(-1);
    }
}
