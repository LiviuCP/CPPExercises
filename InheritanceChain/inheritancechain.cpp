#include <iostream>

#include "inheritancechain.h"

Parent::Parent()
    : mParentId{getId()}
{
    create();
}

Parent::~Parent()
{
    destroy();
}

void Parent::create()
{
    std::cout << "Constructing parent" << std::endl;
}

void Parent::destroy()
{
    std::cout << "Destroying parent" << std::endl << std::endl;
}

size_t Parent::getId() const
{
    std::cout << "Providing parent id" << std::endl;
    return 0;
}

Child::Child()
    : mChildId{getId()}
{
    create();
}

Child::~Child()
{
    destroy();
}

void Child::create()
{
    std::cout << "Constructing child" << std::endl;
}

void Child::destroy()
{
    std::cout << "Destroying child" << std::endl;
}

size_t Child::getId() const
{
    std::cout << "Providing child id" << std::endl;
    return 1;
}

GrandChild::GrandChild()
    : mGrandChildId{getId()}
{
    create();
}

GrandChild::~GrandChild()
{
    destroy();
}

void GrandChild::create()
{
    std::cout << "Constructing grandchild" << std::endl;
}

void GrandChild::destroy()
{
    std::cout << "Destroying grandchild" << std::endl;
}

size_t GrandChild::getId() const
{
    std::cout << "Providing grandchild id" << std::endl;
    return 2;
}
