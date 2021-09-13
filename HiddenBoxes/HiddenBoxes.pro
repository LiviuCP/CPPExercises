TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../External/Matrix/MatrixLib/Matrix
INCLUDEPATH += ../External/Matrix/MatrixLib/Utils
INCLUDEPATH += ../Shared
INCLUDEPATH += ../LexicographicSorting

SOURCES += \
        main.cpp \
    boxutils.cpp

HEADERS += \
    boxutils.h
