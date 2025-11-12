#include <iostream>

#include "utils.h"

using namespace std;

/* This small program shows the difference between early and late binding. */

class Base
{
public:
    virtual ~Base();

    virtual void foo1();
    void foo2();

    /* These two methods are not declared virtual and are only declared in Base.
       They still "belong" to the Base class even when called from Derived.
       Consequently their binding behavior when calling the foo() counterparts is determined in Base.
    */

    void bar1();
    void bar2();
};

class Derived : public Base
{
public:
    /* foo1() is declared virtual in Base so a late binding occurs when calling it from bar1().
       Declaring it with or without virtual keyword in Derived has no effect on this behavior.
    */

    void foo1();

    /* bar2() will still call Base::foo2() even if foo2() is declared as virtual in Derived.
       This is because it hasn't been declared virtual in Base.
       Declaring foo2() with or without virtual keyword in Derived has no effect on this behavior.
    */

    virtual void foo2();
};

int main()
{
    Utilities::clearScreen();

    Derived derived;

    /* foo1() is declared virtual in Base class so a late binding is performed at runtime when invoking it from bar1()
       even if bar1() belongs to Base and is not declared virtual. This means foo1() is called according to the object
       type which is Derived in this case.
    */

    derived.bar1();

    /* foo2() is not declared virtual in Base class.
       As bar2() was only declared in Base and is not virtual (so it "belongs" to Base even when called with a Derived
       object) an early binding is performed at compile time for the call to foo2(). This means bar2() will always call
       Base::foo2() no matter the object type. The fact that foo2() is declared virtual in Derived class has no effect
       on this behavior. Only when declaring it virtual in Base class could Derived::foo2() be called by bar2() for a
       Derived object.
    */

    derived.bar2();
}

Base::~Base()
{
}

void Base::foo1()
{
    cout << "Base::foo1()" << endl;
}

void Base::foo2()
{
    cout << "Base::foo2()" << endl;
}

void Base::bar1()
{
    cout << "bar1() calling foo1()" << endl;
    foo1();
}

void Base::bar2()
{
    cout << "bar2() calling foo2()" << endl;
    foo2();
}

void Derived::foo1()
{
    cout << "Derived::foo1()" << endl;
}

void Derived::foo2()
{
    cout << "Derived::foo2()" << endl;
}
