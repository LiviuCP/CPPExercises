#pragma once

class BaseVirtual
{
public:
    BaseVirtual();
    virtual ~BaseVirtual();
};

class DerivedVirtual : public BaseVirtual
{
public:
    DerivedVirtual();
    ~DerivedVirtual();
};
