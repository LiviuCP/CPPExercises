TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../External/Matrix/MatrixLib/Matrix
INCLUDEPATH += ../External/Matrix/MatrixLib/Utils

SOURCES += \
        main.cpp \
        chesstable.cpp

HEADERS += \
    chesstable.h
