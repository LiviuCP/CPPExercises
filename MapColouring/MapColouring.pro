TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../External/Matrix/MatrixLib
INCLUDEPATH += ../Shared

SOURCES += \
        main.cpp \
    mapcolouringutils.cpp

HEADERS += \
    mapcolouringutils.h
