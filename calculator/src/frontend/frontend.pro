HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcFrontend
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin


win32:DEFINES += _USE_MATH_DEFINES

# Input
HEADERS += UserInterface.hpp

SOURCES += UserInterface.cpp

unix:LIBS += -ldl
win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1

