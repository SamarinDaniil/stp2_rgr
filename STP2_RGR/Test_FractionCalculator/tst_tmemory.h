#ifndef TST_TMEMORY_H
#define TST_TMEMORY_H

#include <QObject>
#include <QtTest>
#include <QString>

#include "../FractionCalculator/tmemory.h"
#include "../FractionCalculator/tfrac.h"

class TestTMemory : public QObject
{
    Q_OBJECT

public:
    TestTMemory();
    ~TestTMemory();

private slots:
    void testInitAndRecall();
    void testInitAndRecall_data();

    void testAdd();
    void testAdd_data();

    void testClear();

    void testState();

    void testEditStore();
    void testEditAdd();
    void testEditClear();
    void testEditRecall();
};

#endif // TST_TMEMORY_H
