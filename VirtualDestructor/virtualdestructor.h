#ifndef VIRTUALDESTRUCTOR_H
#define VIRTUALDESTRUCTOR_H

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

#endif // VIRTUALDESTRUCTOR_H
