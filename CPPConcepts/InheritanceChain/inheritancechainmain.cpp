/* This mini-app illustrates the way objects are being created and destroyed and how virtual functions are called in the
   creation and destruction phases. The object is being constructed starting with the base and until the most derived
   part. The destruction occurs in reverse order. The virtual functions are being called according to "what the object
   is" at invoking time.
   !!! It is advisable not to call any virtual functions from constructor or destructor as this might lead to unexpected
   results !!!
*/

#include <iostream>

#include "inheritancechain.h"
#include "utils.h"

int main()
{
    Utilities::clearScreen();

    std::cout << "OBJECT CREATION" << std::endl;
    std::cout << std::endl << "1) Parent object" << std::endl;
    Parent parent;
    std::cout << std::endl << "2) Child object" << std::endl;
    Child child;
    std::cout << std::endl << "3) Grandchild object" << std::endl;
    GrandChild grandChild;
    std::cout << std::endl << "OBJECT DESTRUCTION" << std::endl << std::endl;

    return 0;
}

/* Bottom line: the object is what the currently executing constructor/destructor is!
   For Grandchild object:
    - at construction: Parent -> Child -> GrandChild (once a constructor returns the object is promoted to next derived
   level and the corresponding constructor continues executing)
    - at destruction: GrandChild -> Child -> Parent (once a destructor finishes executing the object is demoted to the
   previous derived level and the corresponding destructor is called)
*/
