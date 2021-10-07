TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    concurrentservermain.cpp \
    ../Server/server.cpp

HEADERS += \
    ../Server/server.h
