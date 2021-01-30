QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../BinarySearchTrees
INCLUDEPATH += ../TestUtils

SOURCES +=  tst_avltreestests.cpp \
    ../../BinarySearchTrees/binarysearchtree.cpp \
    ../../BinarySearchTrees/avltree.cpp

LIBS += -L../TestUtils -lTestUtils
