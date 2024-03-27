#pragma once

#include <vector>

#include "hostdatautils.h"

class ICSVAggregator
{
public:
    virtual void storeHostData(const std::vector<Data::HostNameAndInfo>& info) = 0;
};
