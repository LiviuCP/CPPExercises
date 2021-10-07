TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    queuereceivermain.cpp \
    queuereceiver.cpp \
    ../QueueUtils/queuedataobjects.cpp \
    ../QueueUtils/queuedatamessages.cpp

HEADERS += \
    queuereceiver.h \
    ../QueueUtils/queuedataobjects.h \
    ../QueueUtils/queuedatamessages.h \
    ../QueueUtils/queuedatatypes.h
