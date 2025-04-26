#-------------------------------------------------
#
# Project created by QtCreator 2025-03-30T14:47:11
#
#-------------------------------------------------

QT       += core gui

CONFIG   += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FractionCalculator
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tfrac.cpp \
    teditor.cpp \
    history.cpp

HEADERS += \
        mainwindow.h \
    tfrac.h \
    teditor.h \
    tmemory.h \
    adt_control.h \
    adt_proc.h \
    history.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
