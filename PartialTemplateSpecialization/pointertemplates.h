#ifndef POINTERTEMPLATES_H
#define POINTERTEMPLATES_H

/* GENERALIZATIONS */
template<typename T> bool isPointer(T t)
{
    (void)t;
    return false;
}

template<typename T> bool isSecondOrderPointer(T t)
{
    (void)t;
    return false;
}

/* SPECIALIZATIONS */

template<typename T> bool isPointer(T* t)
{
    (void)t;
    return true;
}

template<typename T> bool isSecondOrderPointer(T** t)
{
    (void)t;
    return true;
}

#endif // POINTERTEMPLATES_H
