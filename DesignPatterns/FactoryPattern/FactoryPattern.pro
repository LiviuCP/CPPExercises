TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../Shared

SOURCES += \
    factorypatternmain.cpp \
    server.cpp \
    servertypes.cpp

HEADERS += \
    server.h \
    servertypes.h
