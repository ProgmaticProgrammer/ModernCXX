HOME = ..
include ($$HOME/common.pri)
TEMPLATE = app
TARGET = app
DEPENDPATH += .
INCLUDEPATH += $$HOME/src
DESTDIR = $$HOME/bin

QT += widgets core

SOURCES += main.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcGui -lpdCalcBackend -lpdCalcUtilities
win32:LIBS += -L$$HOME/bin -lpdCalcFrontend1 -lpdCalcBackend1 -lpdCalcUtilities1
