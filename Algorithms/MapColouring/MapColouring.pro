TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../External/Matrix/MatrixLib/Matrix
INCLUDEPATH += ../../Shared

SOURCES += \
    mapcolouringmain.cpp \
    mapcolouringutils.cpp

HEADERS += \
    mapcolouringutils.h
