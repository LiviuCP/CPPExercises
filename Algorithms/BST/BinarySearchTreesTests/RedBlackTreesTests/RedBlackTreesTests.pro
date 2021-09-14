QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../BinarySearchTrees
INCLUDEPATH += ../TestUtils

SOURCES +=  tst_redblacktreestests.cpp \
    ../../BinarySearchTrees/binarysearchtree.cpp \
    ../../BinarySearchTrees/redblacktree.cpp

LIBS += -L../TestUtils -lTestUtils
