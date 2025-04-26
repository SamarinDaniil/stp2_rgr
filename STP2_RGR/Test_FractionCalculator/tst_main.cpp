// tests/tst_main.cpp
#include <QtTest>
#include <QObject>
#include <QDebug>

#include "tst_tfrac.h"
#include "tst_teditor.h"
#include "tst_tmemory.h"
#include "tst_adtproc.h"
#include "tst_adtcontrol.h"
#include "tst_history.h"

int main(int argc, char *argv[])
{
    int status = 0;
    qDebug() << "Starting Fraction Calculator tests...";
    qDebug() << "------------------------------------";

    // --- Запуск тестов для TFrac ---
    {
        qDebug() << "Running TestTFrac...";
        TestTFrac tc;
        status |= QTest::qExec(&tc, argc, argv);
        qDebug() << "TestTFrac finished.";
        qDebug() << "------------------------------------";
    }

    // --- Запуск тестов для TEditor ---
    {
        qDebug() << "Running TestTEditor...";
        TestTEditor tc;
        status |= QTest::qExec(&tc, argc, argv);
        qDebug() << "TestTEditor finished.";
        qDebug() << "------------------------------------";
    }

    // --- Запуск тестов для TMemory ---
    {
        qDebug() << "Running TestTMemory...";
        TestTMemory tc;
        status |= QTest::qExec(&tc, argc, argv);
        qDebug() << "TestTMemory finished.";
        qDebug() << "------------------------------------";
    }

     // --- Запуск тестов для ADT_Proc ---
    {
        qDebug() << "Running TestADTProc...";
        TestADTProc tc;
        status |= QTest::qExec(&tc, argc, argv);
        qDebug() << "TestADTProc finished.";
        qDebug() << "------------------------------------";
    }

    // --- Запуск тестов для ADT_Control ---
    {
        qDebug() << "Running TestADTControl...";
        TestADTControl tc;
        status |= QTest::qExec(&tc, argc, argv);
        qDebug() << "TestADTControl finished.";
        qDebug() << "------------------------------------";
    }

     // --- Запуск тестов для History ---
    {
        qDebug() << "Running TestHistory...";
        TestHistory tc;
        status |= QTest::qExec(&tc, argc, argv);
        qDebug() << "TestHistory finished.";
        qDebug() << "------------------------------------";
    }

    // -----------------------------------------

    qDebug() << "All tests finished. Final status code:" << status;
    if (status == 0) {
        qDebug() << "RESULT: All tests passed!";
    } else {
        qDebug() << "RESULT: SOME TESTS FAILED!";
    }

    return status;
}
