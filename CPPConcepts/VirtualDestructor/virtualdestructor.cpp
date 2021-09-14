#include <iostream>

#include "virtualdestructor.h"

BaseVirtual::BaseVirtual()
{
    std::cout << "Creating object BaseVirtual" << std::endl;
}

BaseVirtual::~BaseVirtual()
{
    std::cout << "Destroying object BaseVirtual" << std::endl;
}

DerivedVirtual::DerivedVirtual()
    : BaseVirtual()
{
    std::cout << "Creating object DerivedVirtual" << std::endl;
}

DerivedVirtual::~DerivedVirtual()
{
    std::cout << "Destroying object DerivedVirtual" << std::endl;
}
