TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../../Shared

SOURCES += \
    concurrentservermain.cpp \
    ../Server/server.cpp

HEADERS += \
    ../Server/server.h
