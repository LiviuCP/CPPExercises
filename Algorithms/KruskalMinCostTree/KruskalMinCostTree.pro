TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../External/Matrix/MatrixLib/Matrix
INCLUDEPATH += ../../Shared

SOURCES += \
    main.cpp \
    kruskaltree.cpp

HEADERS += \
    kruskaltree.h
