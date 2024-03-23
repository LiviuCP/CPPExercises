#pragma once

#include <string>

namespace Data {
    struct HostInfo
    {
        std::string m_MacAddress;
        std::string m_IpAddress;
        std::string m_IpClass;
    };

    using HostNameAndInfo = std::pair<std::string, HostInfo>;
}
