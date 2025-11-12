#include <iostream>

#include "nonvirtualdestructor.h"

BaseNonVirtual::BaseNonVirtual()
{
    std::cout << "Creating object BaseNonVirtual" << std::endl;
}

BaseNonVirtual::~BaseNonVirtual()
{
    std::cout << "Destroying object BaseNonVirtual" << std::endl;
}

DerivedNonVirtual::DerivedNonVirtual()
    : BaseNonVirtual()
{
    std::cout << "Creating object DerivedNonVirtual" << std::endl;
}

DerivedNonVirtual::~DerivedNonVirtual()
{
    std::cout << "Destroying object DerivedNonVirtual" << std::endl;
}
