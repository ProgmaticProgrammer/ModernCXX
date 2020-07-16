HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcBackendTest
DEPENDPATH += $$HOME/src/shared
INCLUDEPATH += . $$HOME $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin
DEFINES += BACKEND_TEST_DIR=\\\"$$PWD\\\"
unix:DEFINES += PLUGIN_TEST_FILE=\\\"plugins.unix.pdp\\\"
win32:DEFINES += PLUGIN_TEST_FILE=\\\"plugins.win.pdp\\\"


# Input
HEADERS += \
            CommandTest.h \
            CommandRepositoryTest.h \
            CommandManagerTest.h \
            CommandDispatcherTest.h \
            ModelEventsObserver.h \
            ModelTest.h

SOURCES += \
            CommandTest.cpp \
            CommandRepositoryTest.cpp \
            CommandManagerTest.cpp \
            CommandDispatcherTest.cpp \
            ModelTest.cpp

unix:LIBS += -L$$HOME/lib -lpdCalcUtilities -lpdCalcBackend
win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcBackend1 -lpdCalcFrontend1

QT -= gui core
QT += testlib
