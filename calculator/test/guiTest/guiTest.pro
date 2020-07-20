HOME = ../..
include ($$HOME/common.pri)
QMAKE_CXXFLAGS += -Wno-deprecated-declarations
TEMPLATE = app
TARGET = testPdCalc
INCLUDEPATH += $$HOME $$HOME/src
DESTDIR = $$HOME/bin

QT += testlib widgets

HEADERS += \
    ModelTest.h \
    model.h

# Input
SOURCES += \
    ModelTest.cpp \
    model.cpp

#unix:LIBS += -L$$HOME/lib -lpdCalcUtilities -lpdCalcUtilitiesTest \
#        -lpdCalcBackend -lpdCalcBackendTest \
#        -lpdCalcCliTest -lpdCalcCli \
#        -lpdCalcGuiTest -lpdCalcGui \
#        -lpluginsTest

#win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1 -lpdCalcUtilitiesTest1 \
#        -lpdCalcBackend1 -lpdCalcBackendTest1# -lpdCalcFrontend1 -lpdCalcGuiTest1
