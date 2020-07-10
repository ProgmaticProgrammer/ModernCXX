QT += testlib
QT -= gui

INCLUDEPATH += ../../src
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    test_command.cpp \
    test_stack.cpp
