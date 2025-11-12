#include <cassert>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "queuedatamessages.h"

#include "queuesender.h"

#define WRITE_MESSAGE(messageType, objectType)                                                                         \
    {                                                                                                                  \
        messageType message{static_cast<long>(dataType), *(reinterpret_cast<objectType*>(data))};                      \
        msgsnd(m_QueueId, &message, sizeof(message), IPC_NOWAIT);                                                      \
    }

constexpr int c_BaseId{543};

QueueSender::QueueSender(std::string queueFile)
    : m_QueueFilename{queueFile}
    , m_QueueId{-1}
{
    assert(m_QueueFilename.size() > 0 && "Empty string provided for queue file");
    _createQueueFile();
    _retrieveQueueId();
}

void QueueSender::writeToQueue(void* data, DataTypes dataType)
{
    if (data)
    {
        switch (dataType)
        {
        case DataTypes::INT:
            WRITE_MESSAGE(IntMessage, int);
            break;
        case DataTypes::DOUBLE:
            WRITE_MESSAGE(DoubleMessage, double);
            break;
        case DataTypes::METEODATA:
            WRITE_MESSAGE(MeteoDataMessage, MeteoData);
            break;
        default:
            assert(false && "Unknown data type");
        }
    }
}

void QueueSender::_createQueueFile()
{
    FILE* queueFile = fopen(m_QueueFilename.c_str(), "a+");
    if (!queueFile)
    {
        perror("Error in opening queue file");
        exit(-1);
    }
    else
    {
        fclose(queueFile);
    }
}

void QueueSender::_retrieveQueueId()
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
