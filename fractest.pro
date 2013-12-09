#-------------------------------------------------
#
# Project created by QtCreator 2011-12-05T17:40:01
#
#-------------------------------------------------

QT       += core gui

TARGET = fractest
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES +=\
        fractest.cpp \
        ft-main.cpp

HEADERS  += fractest.h \
    ftnamespace.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../qpgui/release/ -lqpgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../qpgui/debug/ -lqpgui
else:symbian: LIBS += -lqpgui
else:unix: LIBS += -L$$PWD/../qpgui/ -lqpgui

debug {
    DESTDIR = $$PWD/debug
}

release {
    DESTDIR = $$PWD/release
}

INCLUDEPATH += $$PWD/../qpgui
DEPENDPATH += $$PWD/../qpgui

# Explicitly static link
#
# win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../qpgui/release/qpgui.lib
# else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../qpgui/debug/qpgui.lib
# else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../qpgui/libqpgui.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../mathpack/release/ -lmathpack
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../mathpack/debug/ -lmathpack
else:symbian: LIBS += -lmathpack
else:unix: LIBS += -L$$PWD/../mathpack/ -lmathpack

INCLUDEPATH += $$PWD/../mathpack
DEPENDPATH += $$PWD/../mathpack

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../mathpack/release/mathpack.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../mathpack/debug/mathpack.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../mathpack/libmathpack.a
