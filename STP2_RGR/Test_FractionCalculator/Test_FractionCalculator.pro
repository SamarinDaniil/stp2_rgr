# tests/tests.pro

QT       += testlib core gui

CONFIG   += testcase console warn_on
CONFIG   -= app_bundle

TARGET = fractioncalculator_tests
TEMPLATE = app

# Указываем путь к исходникам основного проекта
INCLUDEPATH += $$PWD/../FractionCalculator # Убедись, что папка называется именно FractionCalculator

SOURCES += \
    tst_main.cpp \
    tst_tfrac.cpp \
    tst_teditor.cpp \
    tst_tmemory.cpp \
    tst_adtproc.cpp \
    tst_adtcontrol.cpp \
    tst_history.cpp

# Заголовочные файлы тестов (каждый указан ОДИН раз)
HEADERS += \
    tst_tfrac.h \
    tst_teditor.h \
    tst_tmemory.h \
    tst_adtproc.h \
    tst_adtcontrol.h \
    tst_history.h

# Исходные файлы тестируемого кода (из основного проекта)
SOURCES += \
    $$PWD/../FractionCalculator/tfrac.cpp \
    $$PWD/../FractionCalculator/teditor.cpp \
    $$PWD/../FractionCalculator/history.cpp

# Заголовочные файлы тестируемого кода (из основного проекта)
HEADERS += \
    $$PWD/../FractionCalculator/tfrac.h \
    $$PWD/../FractionCalculator/teditor.h \
    $$PWD/../FractionCalculator/tmemory.h \
    $$PWD/../FractionCalculator/adt_proc.h \
    $$PWD/../FractionCalculator/adt_control.h \
    $$PWD/../FractionCalculator/history.h

# Опционально: Если используете C++17
CONFIG += c++17
