TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../Shared

SOURCES += \
    timermain.cpp \
    timer.cpp

HEADERS += \
    timer.h

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread
