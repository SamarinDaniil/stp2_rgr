#ifndef TST_ADTPROC_H
#define TST_ADTPROC_H

#include <QObject>
#include <QtTest>
#include <QString>

#include "../FractionCalculator/adt_proc.h"
#include "../FractionCalculator/tfrac.h"

class TestADTProc : public QObject
{
    Q_OBJECT

public:
    TestADTProc();
    ~TestADTProc();

private slots:
    void testInit();
    void testReset();

    void testSetOperandsAndOperation();

    void testDoOperationAdd();
    void testDoOperationSub();
    void testDoOperationMul();
    void testDoOperationDiv();
    void testDoOperationNone(); // Когда операция 0
    void testDoOperationDivByZero(); // Деление на ноль

    void testDoFunctionReverse();
    void testDoFunctionSquare();
    void testDoFunctionReverseZero(); // Обращение нуля
};

#endif // TST_ADTPROC_H
