QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../BinarySearchTrees

SOURCES += tst_mixedtreetypestests.cpp \
    ../../BinarySearchTrees/binarysearchtree.cpp \
    ../../BinarySearchTrees/redblacktree.cpp \
    ../../BinarySearchTrees/avltree.cpp \

