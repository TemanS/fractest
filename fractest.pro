
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fractest
TEMPLATE = app

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../lib

LIBS += -L$$PWD/../../lib -lmathpack
LIBS += -L$$PWD/../../lib -lqpgui

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib -lmathpack
#else:unix: LIBS += -L$$PWD/../../lib -lmathpack

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib -lqpgui
#else:unix: LIBS += -L$$PWD/../../lib -lqpgui

SOURCES +=\
        fractest.cpp \
        ft-main.cpp

HEADERS  += fractest.h \
    ftnamespace.h


debug {
    DESTDIR = ../debug
}

release {
    DESTDIR = ../release
}


# Explicitly static link
#
# win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../qpgui/release/qpgui.lib
# else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../qpgui/debug/qpgui.lib
# else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../qpgui/libqpgui.a
