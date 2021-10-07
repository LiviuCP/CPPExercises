TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../../Shared

SOURCES += \
    clientmain.cpp \
    client.cpp

HEADERS += \
    client.h
