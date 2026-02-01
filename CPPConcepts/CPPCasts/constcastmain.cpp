#include <iostream>

#include "objectprintutils.h"
#include "utils.h"

// only required for reinterpret_cast but kept here for consistency and to avoid any errors
#pragma pack(1)

#include "objects.h"

int main()
{
    ComposedObject object{-5, 2, 'a', "abcdefghij", {-1.05, 2.99}};

    Utilities::clearScreen();
    std::cout << "Initial object values: \n" << object;

    object.invertPoint();
    object.invertInt();

    // in this particular case it is safe (although unnecessary) to perform such an operation
    // as we are sure the object is not const; however in other situations (like receiving a const pointer/reference to
    // an object) it should NEVER be done as it would result in undefined behavior if the referenced object is really
    // const
    const ComposedObject* c_ObjectPtr{&object};
    const_cast<ComposedObject*>(c_ObjectPtr)->convertStringToUpperCase();

    std::cout << "Modified object values: \n" << object;

    return 0;
}
