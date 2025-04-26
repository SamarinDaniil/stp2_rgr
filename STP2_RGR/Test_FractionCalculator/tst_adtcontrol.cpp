#include "tst_adtcontrol.h"
#include <utility>

TestADTControl::TestADTControl() {}
TestADTControl::~TestADTControl() {}

void TestADTControl::testInitAndReset()
{
    ADT_Control control;
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Start);
    QCOMPARE(control.GetMemory().FState(), false);
    // Проверим начальное значение редактора (неявно через Reset)
    QCOMPARE(control.Reset(), QString("0")); // Reset возвращает "0"
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Start); // Состояние после Reset
}

void TestADTControl::testEditorCommands()
{
    ADT_Control control;
    // Ввод числа
    QCOMPARE(control.ExecCommandEditor(1), QString("1"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing);
    QCOMPARE(control.ExecCommandEditor(2), QString("12"));
    QCOMPARE(control.ExecCommandEditor(3), QString("123"));
    // Ввод разделителя
    QCOMPARE(control.ExecCommandEditor(11), QString("123/")); // Команда '/'
    QCOMPARE(control.ExecCommandEditor(4), QString("123/4"));
    QCOMPARE(control.ExecCommandEditor(5), QString("123/45"));
    // Смена знака
    QCOMPARE(control.ExecCommandEditor(10), QString("-41/15")); // Команда +/- и произойдет сокращение
    QCOMPARE(control.ExecCommandEditor(10), QString("41/15"));
    // Backspace
    QCOMPARE(control.ExecCommandEditor(12), QString("41/1")); // Команда BS
    QCOMPARE(control.ExecCommandEditor(12), QString("41"));
    QCOMPARE(control.ExecCommandEditor(12), QString("4"));
    // Clear Entry
    QCOMPARE(control.ExecCommandEditor(13), QString("0")); // Команда CE
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing);

     // Ввод после =
     control.Reset();
     control.ExecCommandEditor(5); // 5
     control.ExecOperation(1); // +
     control.ExecCommandEditor(3); // 3
     control.Calculate(); // = 8/1
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);
     QCOMPARE(control.ExecCommandEditor(2), QString("2")); // Новый ввод после =
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing); // Состояние сбрасывается в Start/Editing
}


void TestADTControl::testSimpleAddition()
{
    ADT_Control control;
    control.ExecCommandEditor(5); // 5
    QCOMPARE(control.ExecOperation(1), QString("5/1")); // + (показывает левый операнд)
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::OpDone);
    control.ExecCommandEditor(3); // 3
    QCOMPARE(control.ExecCommandEditor(11), QString("3/")); // /
    control.ExecCommandEditor(2); // 2 -> Введено "3/2"
    QCOMPARE(control.Calculate(), QString("13/2")); // 5/1 + 3/2 = 10/2 + 3/2 = 13/2
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);
}

void TestADTControl::testSimpleSubtraction()
{
    ADT_Control control;
    control.ExecCommandEditor(1); // 1
    control.ExecCommandEditor(0); // 0 -> 10
    QCOMPARE(control.ExecOperation(2), QString("10/1")); // -
    control.ExecCommandEditor(3); // 3
    QCOMPARE(control.Calculate(), QString("7/1")); // 10 - 3 = 7
}

void TestADTControl::testSimpleMultiplication()
{
     ADT_Control control;
    control.ExecCommandEditor(3); // 3
    control.ExecCommandEditor(11); // /
    control.ExecCommandEditor(4); // 4 -> 3/4
    QCOMPARE(control.ExecOperation(3), QString("3/4")); // *
    control.ExecCommandEditor(2); // 2
    QCOMPARE(control.Calculate(), QString("3/2")); // 3/4 * 2/1 = 6/4 = 3/2
}

void TestADTControl::testSimpleDivision()
{
    ADT_Control control;
    control.ExecCommandEditor(5); // 5
    QCOMPARE(control.ExecOperation(4), QString("5/1")); // /
    control.ExecCommandEditor(2); // 2
    QCOMPARE(control.Calculate(), QString("5/2")); // 5 / 2 = 5/2
}

void TestADTControl::testDivisionByZeroError()
{
    ADT_Control control;
    control.ExecCommandEditor(5); // 5
    control.ExecOperation(4); // /
    control.ExecCommandEditor(0); // 0
    QCOMPARE(control.Calculate(), QString("ERROR")); // 5 / 0
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Error);

    QCOMPARE(control.ExecOperation(1), QString("ERROR")); // +
    QCOMPARE(control.ExecCommandEditor(1), QString("1"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing);
}

void TestADTControl::testOperationChain()
{
    ADT_Control control; // 5 + 3 * 2 =
    control.ExecCommandEditor(5);
    control.ExecOperation(1); // + (op=Add, left=5/1, state=OpDone)
    control.ExecCommandEditor(3);
    control.ExecOperation(3);
    QCOMPARE(control.GetProc().GetLeftResultOperand().ToString(), QString("8/1"));
    control.ExecCommandEditor(2);
    QString result = control.Calculate(); // = (do 8*2=16/1)
    QCOMPARE(result, QString("16/1"));
    QCOMPARE(control.GetEditorString(), QString("16/1"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);
}

void TestADTControl::testFunctionSquare()
{
    ADT_Control control;
    control.ExecCommandEditor(3);
    control.ExecCommandEditor(10); // +/- -> -3
    QString result = control.ExecFunction(1); // sqr (command 1)
    QCOMPARE(result, QString("9/1"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::FuncDone);
    QCOMPARE(control.GetEditorString(), QString("9/1"));
    QCOMPARE(control.GetProc().GetRightOperand().ToString(), QString("9/1"));
}

void TestADTControl::testFunctionReverse()
{
    ADT_Control control;
    control.ExecCommandEditor(4); // 4
    QString result = control.ExecFunction(0); // 1/x (command 0)
    QCOMPARE(result, QString("1/4"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::FuncDone);
    // Проверим через геттеры
    QCOMPARE(control.GetEditorString(), QString("1/4"));
    QCOMPARE(control.GetProc().GetRightOperand().ToString(), QString("1/4"));
}

void TestADTControl::testFunctionReverseZeroError()
{
    ADT_Control control;
    control.ExecCommandEditor(0); // 0
    QCOMPARE(control.ExecFunction(0), QString("ERROR")); // 1/x от нуля
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Error);
}


void TestADTControl::testCalculateAfterNumber()
{
    ADT_Control control;
    control.ExecCommandEditor(5); // 5
    control.ExecCommandEditor(11); // /
    control.ExecCommandEditor(2); // 2 -> 5/2
    QString result = control.Calculate(); // =
    QCOMPARE(result, QString("5/2"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);
    // Проверим редактор через геттер
    QCOMPARE(control.GetEditorString(), QString("5/2"));
}

void TestADTControl::testCalculateAfterOperation()
{
    ADT_Control control;
    control.ExecCommandEditor(5);
    control.ExecOperation(1);
     control.ExecCommandEditor(3);
     control.ExecOperation(1);
     QCOMPARE(control.Calculate(), QString("11/1"));
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);

     control.Calculate();
     QCOMPARE(control.editor.ToString(), QString("14/1"));
}


void TestADTControl::testCalculateAfterFunction()
{
    ADT_Control control;
    control.ExecCommandEditor(3);
    control.ExecCommandEditor(11);
    control.ExecCommandEditor(2);
    control.ExecFunction(1);
    QCOMPARE(control.editor.ToString(), QString("9/4"));
    control.ExecOperation(1);
    control.ExecCommandEditor(1);
    control.ExecCommandEditor(11);
    control.ExecCommandEditor(4);
    QCOMPARE(control.Calculate(), QString("5/2"));
}

void TestADTControl::testCalculateAfterEquals()
{
    ADT_Control control;
    control.ExecCommandEditor(5);
    control.ExecOperation(1);
    control.ExecCommandEditor(3);
    QString result1 = control.Calculate();
    QCOMPARE(result1, QString("8/1"));
    QCOMPARE(control.GetEditorString(), QString("8/1"));

    QString result2 = control.Calculate();
    QCOMPARE(result2, QString("11/1"));
    QCOMPARE(control.GetEditorString(), QString("11/1"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);
}

void TestADTControl::testOperationChange()
{
    ADT_Control control;
    control.ExecCommandEditor(5);
    control.ExecOperation(1);
    QCOMPARE(control.proc.GetOperation(), ADT_Proc<TFrac>::OperationType::Add);
    control.ExecOperation(3);
    QCOMPARE(control.proc.GetOperation(), ADT_Proc<TFrac>::OperationType::Mul);
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::OpChange);
    control.ExecCommandEditor(3);
    QCOMPARE(control.Calculate(), QString("15/1"));
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);
}


void TestADTControl::testMemoryStoreAndRecall()
{
    ADT_Control control;
    control.ExecCommandEditor(7);
    control.ExecCommandEditor(11);
    control.ExecCommandEditor(8);
    QString currentEditorValue = control.GetEditorString();
    QCOMPARE(currentEditorValue, QString("7/8"));

    std::pair<QString, bool> msResult = control.ExecCommandMemory(0, currentEditorValue);
    QCOMPARE(msResult.second, true);
    QVERIFY(msResult.first.isEmpty());
    QCOMPARE(control.GetMemory().FState(), true);
    QCOMPARE(control.GetMemory().FNumber().ToString(), QString("7/8"));

    QString ceResult = control.ExecCommandEditor(13); // CE
    QCOMPARE(ceResult, QString("0"));
    QCOMPARE(control.GetEditorString(), QString("0")); // Проверяем редактор после CE
    std::pair<QString, bool> mrResult = control.ExecCommandMemory(3, control.GetEditorString()); // command 3 = MR
    QCOMPARE(mrResult.second, true); // Состояние памяти не изменилось
    QCOMPARE(mrResult.first, QString("7/8"));
    QCOMPARE(control.GetEditorString(), QString("7/8"));
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing); // Состояние после MR
}

void TestADTControl::testMemoryClear()
{
     ADT_Control control;
     control.ExecCommandEditor(1); // 1
     control.ExecCommandMemory(0, control.editor.ToString()); // MS
     QVERIFY(control.GetMemory().FState());

     std::pair<QString, bool> mcResult = control.ExecCommandMemory(2, control.editor.ToString()); // MC (command 2)
     QCOMPARE(mcResult.second, false); // Состояние памяти = false
     QVERIFY(mcResult.first.isEmpty());
     QCOMPARE(control.GetMemory().FState(), false);
     QCOMPARE(control.GetMemory().FNumber(), TFrac(0,1));
}

void TestADTControl::testMemoryAdd()
{
    ADT_Control control;
    control.ExecCommandEditor(1);
    control.ExecCommandEditor(11);
    control.ExecCommandEditor(2);
    std::pair<QString, bool> mplusResult1 = control.ExecCommandMemory(1, control.editor.ToString());
    QCOMPARE(mplusResult1.second, true);
    QCOMPARE(control.GetMemory().FNumber().ToString(), QString("1/2"));
}

void TestADTControl::testMemoryRecallUpdatesEditor()
{
    ADT_Control control;

    QString valueToStore = "5/9";
    qDebug() << "[TestMR] Attempting MS with:" << valueToStore;
    std::pair<QString, bool> msResult = control.ExecCommandMemory(0, valueToStore); // MS "5/9"

    QVERIFY2(msResult.second, "MS should set memory state to true");
    QCOMPARE(control.GetMemory().FState(), true);
    QCOMPARE(control.GetMemory().FNumber().ToString(), valueToStore);

    QString editResult = control.ExecCommandEditor(1); // В редакторе "1"
    QCOMPARE(editResult, QString("1"));
    QCOMPARE(control.GetEditorString(), QString("1"));

    qDebug() << "[TestMR] Attempting MR. Editor before:" << control.GetEditorString();
    std::pair<QString, bool> mrResult = control.ExecCommandMemory(3, control.GetEditorString()); // MR
    QCOMPARE(mrResult.first, valueToStore);
    QVERIFY2(mrResult.second, "Memory state should remain true after MR");

    QCOMPARE(control.GetEditorString(), valueToStore);
    QCOMPARE(control.GetProc().GetRightOperand().ToString(), valueToStore);

     QCOMPARE(control.GetMemory().FNumber().ToString(), valueToStore);
     QCOMPARE(control.GetMemory().FState(), true);


}

void TestADTControl::testStateTransitions()
{
    ADT_Control control;
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Start);

    control.ExecCommandEditor(1); // 1
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing);

    control.ExecOperation(1); // +
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::OpDone);

    control.ExecOperation(3); // * (смена операции)
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::OpChange);

    control.ExecCommandEditor(2); // 2
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing);

    control.Calculate(); // =
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::ExpDone);

    control.ExecOperation(1); // + (после =)
    QCOMPARE(control.GetCurrentState(), ADT_Control::State::OpDone);

     control.ExecCommandEditor(3); // 3
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::Editing);

     control.ExecFunction(1); // sqr
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::FuncDone);

     control.ExecOperation(2); // - (после функции)
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::OpDone);

     // Reset
     control.Reset();
     QCOMPARE(control.GetCurrentState(), ADT_Control::State::Start);
}
