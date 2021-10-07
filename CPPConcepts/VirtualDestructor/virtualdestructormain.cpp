/* This is a mini-app that demonstrates the importance of using virtual destructor along with inheritance
   !!! Always use VIRTUAL destructor when inheritance is intended !!!
   This ensures that when having a Base* pointer to a Derived object and a delete operation is invoked to the pointer the destructor of the derived object is being called
   (same as for any other virtual function declared in Base with "virtual" keyword).
   The derived destructor will in turn call the base destructor once the derived part deallocation is over.
   The normal operation mode is: destructor executes the constructor operation in reverse!!!
*/

#include <iostream>

#include "nonvirtualdestructor.h"
#include "virtualdestructor.h"
#include "utils.h"

using namespace std;

int main()
{
    Utilities::clearScreen();

    cout << "Case 1a: base pointer to derived class, no virtual destructor" << endl;
    BaseNonVirtual* baseNonVirtual{new DerivedNonVirtual};
    delete baseNonVirtual; // only destructor of BaseNonVirtual is called (no virtual keyword in destructor)

    cout << endl << "Case 1b: derived pointer to derived class, no virtual destructor" << endl;
    DerivedNonVirtual* derivedNonVirtual{new DerivedNonVirtual};
    delete derivedNonVirtual; // destructor of DerivedNonVirtual is called, then destructor of BaseNonVirtual

    cout << endl << "Case 2a: base pointer to derived class, virtual destructor" << endl;
    BaseVirtual* baseVirtual{new DerivedVirtual};
    delete baseVirtual; // destructor of DerivedVirtual is called (virtual), then destructor of BaseVirtual

    cout << "Case 2b: derived pointer to derived class, virtual destructor" << endl;
    DerivedVirtual* derivedVirtual{new DerivedVirtual};
    delete derivedVirtual; // destructor of DerivedVirtual is called, then destructor of BaseVirtual

    return 0;
}


