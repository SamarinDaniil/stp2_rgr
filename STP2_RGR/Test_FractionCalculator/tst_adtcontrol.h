#ifndef TST_ADTCONTROL_H
#define TST_ADTCONTROL_H

#include <QObject>
#include <QtTest>
#include <QString>

#include "../FractionCalculator/adt_control.h"

class TestADTControl : public QObject
{
    Q_OBJECT

public:
    TestADTControl();
    ~TestADTControl();

private slots:
    // Инициализация и сброс
    void testInitAndReset();

    // Тестирование ввода цифр и знаков (ExecCommandEditor)
    void testEditorCommands();

    // Тестирование бинарных операций (ExecOperation)
    void testSimpleAddition(); // 5 + 3 =
    void testSimpleSubtraction();
    void testSimpleMultiplication();
    void testSimpleDivision();
    void testDivisionByZeroError();

    // Тестирование цепочек операций
    void testOperationChain(); // 5 + 3 * 2 =

    // Тестирование унарных функций (ExecFunction)
    void testFunctionSquare(); // 5 sqr
    void testFunctionReverse(); // 4 Reverse
    void testFunctionReverseZeroError();

    // Тестирование = (Calculate)
    void testCalculateAfterNumber(); // 5 =
    void testCalculateAfterOperation(); // 5 + = (поведение?) -> должно дать 5+5? (нет в текущей реализации)
    void testCalculateAfterFunction(); // 5 sqr =
    void testCalculateAfterEquals(); // 5 + 3 = = (поведение?)

    // Тестирование смены операции
    void testOperationChange(); // 5 + * 3 =

    // Тестирование команд памяти (ExecCommandMemory)
    void testMemoryStoreAndRecall(); // MS, MR
    void testMemoryClear(); // MC
    void testMemoryAdd(); // M+
    void testMemoryRecallUpdatesEditor();

    // Тестирование состояний
    void testStateTransitions();
};

#endif // TST_ADTCONTROL_H
