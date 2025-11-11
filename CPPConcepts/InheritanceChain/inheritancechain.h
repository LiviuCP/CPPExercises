#pragma once

#include <cstdlib>

class Parent
{
public:
    Parent();
    virtual ~Parent();
    virtual void create();
    virtual void destroy();

    // this function could be implemented statically but it was done this way in order to demonstrate what happens in the initialization list of the constructor when a virtual function is being called
    virtual size_t getId() const;
private:
    size_t mParentId;
};

class Child : public Parent
{
public:
    Child();
    ~Child() override;
    void create() override;
    void destroy() override;
    size_t getId() const override;
private:
    size_t mChildId;
};

class GrandChild final : public Child
{
public:
    GrandChild();
    ~GrandChild() override;
    void create() override;
    void destroy() override;
    size_t getId() const override;
private:
    size_t mGrandChildId;
};
