TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        chesstable.cpp

HEADERS += \
    ../External/Matrix/MatrixLib/matrix.h \
    chesstable.h
