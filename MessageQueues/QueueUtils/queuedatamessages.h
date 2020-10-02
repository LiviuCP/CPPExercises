#ifndef QUEUEDATASTRUCTS_H
#define QUEUEDATASTRUCTS_H

#include "queuedataobjects.h"

struct IntMessage
{
    long objectType;
    int object;

    // make thorough initialization of each member mandatory (see QueueReceiver READ_MESSAGE comments)
    IntMessage() = delete;
    IntMessage(long objType, int obj);
};

struct DoubleMessage
{
    long objectType;
    double object;

    // make thorough initialization of each member mandatory (see QueueReceiver READ_MESSAGE comments)
    DoubleMessage() = delete;
    DoubleMessage(long objType, double obj);
};

struct MeteoDataMessage
{
    long objectType;
    MeteoData object;

    // make thorough initialization of each member mandatory (see QueueReceiver READ_MESSAGE comments)
    MeteoDataMessage() = delete;
    MeteoDataMessage(long objType, const MeteoData &obj);
};

#endif // QUEUEDATASTRUCTS_H
