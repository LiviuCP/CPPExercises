#include "queuedatamessages.h"

DoubleMessage::DoubleMessage(long objType, double obj)
    : objectType{objType}
    , object{obj}
{
}

MeteoDataMessage::MeteoDataMessage(long objType, const MeteoData &obj)
    : objectType{objType}
    , object{obj}
{
}

IntMessage::IntMessage(long objType, int obj)
    : objectType{objType}
    , object{obj}
{
}
