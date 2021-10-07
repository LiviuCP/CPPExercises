TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    queuesendermain.cpp \
    queuesender.cpp  \
    ../QueueUtils/queuedataobjects.cpp \
    ../QueueUtils/queuedatamessages.cpp

HEADERS += \
    queuesender.h  \
    ../QueueUtils/queuedataobjects.h \
    ../QueueUtils/queuedatamessages.h \
    ../QueueUtils/queuedatatypes.h
