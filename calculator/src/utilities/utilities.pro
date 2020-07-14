HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcUtilities
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin

DEFINES += BUILDING_UTILITIES

# Input
HEADERS += Exception.hpp \
           Observer.hpp \
           Publisher.hpp \
           Command.hpp

