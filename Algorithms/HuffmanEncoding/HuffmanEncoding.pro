TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../External/Matrix/MatrixLib/Matrix
INCLUDEPATH += ../../Shared

SOURCES += \
    huffmanmain.cpp \
    huffmanencoder.cpp

HEADERS += \
    huffmanencoder.h
