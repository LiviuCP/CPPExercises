#pragma once

#include <vector>

#include "datautils.h"

class ICSVAggregator
{
public:
    virtual void storeHostData(const std::vector<Data::HostNameAndInfo>& info) = 0;
};
