HOME = ../..
include ($$HOME/common.pri)
TEMPLATE = lib
TARGET = pdCalcBackend
DEPENDPATH += .
INCLUDEPATH += . $$HOME/src
unix:DESTDIR = $$HOME/lib
win32:DESTDIR = $$HOME/bin


win32:DEFINES += _USE_MATH_DEFINES

# Input
HEADERS += \
    CalcCommand.hpp \
    CalcModel.hpp \
    CommandRepository.hpp \
    CommandManager.hpp

SOURCES += \
    CalcModel.cpp \
    CommandManager.cpp

unix:LIBS += -ldl
win32:LIBS += -L$$HOME/bin -lpdCalcUtilities1
