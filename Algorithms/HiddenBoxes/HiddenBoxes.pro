TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../External/Matrix/MatrixLib/Matrix
INCLUDEPATH += ../../Shared
INCLUDEPATH += ../LexicographicSorting

SOURCES += \
    hiddenboxesmain.cpp \
    boxutils.cpp


HEADERS += \
    boxutils.h
