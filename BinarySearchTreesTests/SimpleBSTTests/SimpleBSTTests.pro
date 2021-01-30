QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../BinarySearchTrees
INCLUDEPATH += ../TestUtils

SOURCES +=  tst_simplebsttests.cpp \
    ../../BinarySearchTrees/binarysearchtree.cpp \

LIBS += -L../TestUtils -lTestUtils
