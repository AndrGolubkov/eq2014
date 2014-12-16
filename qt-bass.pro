#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T16:20:29
#
#-------------------------------------------------

QT       += core gui

TARGET = qt-bass
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    visualequalizer.cpp

HEADERS  += widget.h \
    visualequalizer.h

FORMS    += widget.ui

win32: LIBS += -L$$PWD/bass/ -lbass

INCLUDEPATH += $$PWD/bass
DEPENDPATH += $$PWD/bass

win32: PRE_TARGETDEPS += $$PWD/bass/bass.lib
