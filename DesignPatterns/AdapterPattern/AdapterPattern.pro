TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../Shared

SOURCES += \
    adapterpatternmain.cpp \
    hdmidisplay.cpp \
    hdmicard.cpp \
    vgadisplay.cpp \
    vgaadapter.cpp

HEADERS += \
    hdmicard.h \
    hdmidisplay.h \
    vgadisplay.h \
    vgaadapter.h
