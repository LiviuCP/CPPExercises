#ifndef NONVIRTUALDESTRUCTOR_H
#define NONVIRTUALDESTRUCTOR_H

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

#endif // NONVIRTUALDESTRUCTOR_H
