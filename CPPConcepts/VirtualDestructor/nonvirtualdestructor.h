#pragma once

class BaseNonVirtual
{
public:
    BaseNonVirtual();
    ~BaseNonVirtual();
};

class DerivedNonVirtual : public BaseNonVirtual
{
public:
    DerivedNonVirtual();
    ~DerivedNonVirtual();
};
