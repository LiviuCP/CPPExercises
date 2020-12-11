TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../External/Matrix/MatrixLib
INCLUDEPATH += ../Shared
INCLUDEPATH += ../LexicographicSorting

SOURCES += \
        main.cpp \
    ../LexicographicSorting/lexicographicalsorter.cpp \
    ../Shared/matrixutils.cpp \
    boxutils.cpp

HEADERS += \
    boxutils.h
