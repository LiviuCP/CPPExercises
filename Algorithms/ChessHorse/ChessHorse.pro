TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../External/Matrix/MatrixLib/Matrix

SOURCES += \
        main.cpp \
        chesstable.cpp

HEADERS += \
    chesstable.h
