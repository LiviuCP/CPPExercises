TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    virtualdestructormain.cpp \
    virtualdestructor.cpp \
    nonvirtualdestructor.cpp

HEADERS += \
    virtualdestructor.h \
    nonvirtualdestructor.h
