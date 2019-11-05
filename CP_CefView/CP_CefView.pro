#-------------------------------------------------
#
# Project created by QtCreator 2019-11-05T16:33:58
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
qtHaveModule(printsupport): QT += printsupport

TARGET = CP_CefView
TEMPLATE = lib

CONFIG += c++11

DEFINES += CP_CEFVIEW_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ThirdLib_PATH = D:/qtfile/QtCef/src
INCLUDEPATH += $$ThirdLib_PATH/cef
win32:CONFIG(release, debug|release): LIBS += -L$$ThirdLib_PATH/cef/bin/Release/ -llibcef_dll_wrapper -llibcef
else:win32:CONFIG(debug, debug|release): LIBS += -L$$ThirdLib_PATH/cef/bin/Debug/ -llibcef_dll_wrapper -llibcef

LIBS += DbgHelp.Lib Advapi32.lib User32.lib Shell32.lib

SOURCES += \
        QCefClient.cpp \
        QCefClientApp.cpp \
        QCefClientHandler.cpp \
        cefbrowserstackwidget.cpp \
        cefheadfile.cpp \
        cp_cefview.cpp \
        qcefbrowserwidget.cpp \
        qcefmessageevent.cpp \
        qcefrenderapp.cpp \
        qcefv8handler.cpp \
        qcefwebview.cpp

HEADERS += \
        QCefClient.h \
        QCefClientApp.h \
        QCefClientHandler.h \
        cefbrowserstackwidget.h \
        cefheadfile.h \
        cp_cefview.h \
        cp_cefview_global.h  \
        qcefbrowserwidget.h \
        qcefmessageevent.h \
        qcefrenderapp.h \
        qcefv8handler.h \
        qcefwebview.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    cefbrowserstackwidget.ui \
    qcefbrowserwidget.ui \
    qcefwebview.ui

win32 {
    QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
    QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01
    DEFINES += _ATL_XP_TARGETING
}
