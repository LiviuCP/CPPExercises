/* This mini-app illustrates the way objects are being created and destroyed and how virtual functions are called in the creation and destruction phases.
   The object is being constructed starting with the base and until the most derived part.
   The destruction occurs in reverse order.
   The virtual functions are being called according to "what the object is" at invoking time.
   !!! It is advisable not to call any virtual functions from constructor or destructor as this might lead to unexpected results !!!
*/

#include <iostream>

#include "inheritancechain.h"

using namespace std;

int main()
{
    cout << "OBJECT CREATION" << endl;
    cout << endl << "1) Parent object" << endl;
    Parent parent;
    cout << endl << "2) Child object" << endl;
    Child child;
    cout << endl << "3) Grandchild object" << endl;
    GrandChild grandChild;
    cout << endl << "OBJECT DESTRUCTION" << endl << endl;
    return 0;
}

/* Bottom line: the object is what the currently executing constructor/destructor is!
   For Grandchild object:
    - at construction: Parent -> Child -> GrandChild (once a constructor returns the object is promoted to next derived level and the corresponding constructor continues executing)
    - at destruction: GrandChild -> Child -> Parent (once a destructor finishes executing the object is demoted to the previous derived level and the corresponding destructor is called)
*/

