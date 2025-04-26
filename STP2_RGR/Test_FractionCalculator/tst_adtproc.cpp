#include "tst_adtproc.h"

TestADTProc::TestADTProc() {}
TestADTProc::~TestADTProc() {}

void TestADTProc::testInit()
{
    ADT_Proc<TFrac> proc;
    QCOMPARE(proc.GetLeftResultOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetRightOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetOperation(), ADT_Proc<TFrac>::OperationType::None);
}

void TestADTProc::testReset()
{
    ADT_Proc<TFrac> proc;
    proc.SetLeftResultOperand(TFrac("5/6"));
    proc.SetRightOperand(TFrac("1/3"));
    proc.SetOperation(ADT_Proc<TFrac>::OperationType::Add);

    proc.ResetProc();

    QCOMPARE(proc.GetLeftResultOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetRightOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetOperation(), ADT_Proc<TFrac>::OperationType::None);
}


void TestADTProc::testSetOperandsAndOperation()
{
     ADT_Proc<TFrac> proc;
     TFrac left("11/3");
     TFrac right("17/9");

     proc.SetLeftResultOperand(left);
     proc.SetRightOperand(right);
     proc.SetOperation(ADT_Proc<TFrac>::OperationType::Mul); // Умножение (3)

     QCOMPARE(proc.GetLeftResultOperand(), left);
     QCOMPARE(proc.GetRightOperand(), right);
     QCOMPARE(proc.GetOperation(), ADT_Proc<TFrac>::OperationType::Mul);

     proc.SetOperation(2); // Установка через int
     QCOMPARE(proc.GetOperation(), ADT_Proc<TFrac>::OperationType::Sub);
}

void TestADTProc::testDoOperationAdd()
{
    ADT_Proc<TFrac> proc;
    proc.SetLeftResultOperand(TFrac("1/2"));
    proc.SetRightOperand(TFrac("1/2"));
    proc.SetOperation(ADT_Proc<TFrac>::OperationType::Add); // 1
    proc.DoOperation();
    QCOMPARE(proc.GetLeftResultOperand(), TFrac("1/1"));
}

void TestADTProc::testDoOperationSub()
{
     ADT_Proc<TFrac> proc;
    proc.SetLeftResultOperand(TFrac("3/4"));
    proc.SetRightOperand(TFrac("5/6"));
    proc.SetOperation(ADT_Proc<TFrac>::OperationType::Sub); // 2
    proc.DoOperation();
    QCOMPARE(proc.GetLeftResultOperand(), TFrac("-1/12")); // 9/12 - 10/12
}

void TestADTProc::testDoOperationMul()
{
     ADT_Proc<TFrac> proc;
    proc.SetLeftResultOperand(TFrac("12/7"));
    proc.SetRightOperand(TFrac("5/9"));
    proc.SetOperation(ADT_Proc<TFrac>::OperationType::Mul); // 3
    proc.DoOperation();
    QCOMPARE(proc.GetLeftResultOperand(), TFrac("20/21")); // (12*5)/(7*9) = 60/63
}

void TestADTProc::testDoOperationDiv()
{
     ADT_Proc<TFrac> proc;
    proc.SetLeftResultOperand(TFrac("56/7")); // 8/1
    proc.SetRightOperand(TFrac("-22/3"));
    proc.SetOperation(ADT_Proc<TFrac>::OperationType::Div); // 4
    proc.DoOperation();
    QCOMPARE(proc.GetLeftResultOperand(), TFrac("-12/11")); // (8/1) * (-3/22) = -24/22
}

void TestADTProc::testDoOperationNone()
{
    ADT_Proc<TFrac> proc;
    proc.SetLeftResultOperand(TFrac("1/2"));
    proc.SetRightOperand(TFrac("3/4"));
    proc.SetOperation(ADT_Proc<TFrac>::OperationType::None); // 0
    proc.DoOperation();
    QCOMPARE(proc.GetLeftResultOperand(), TFrac("3/4"));
}

void TestADTProc::testDoOperationDivByZero()
{
    ADT_Proc<TFrac> proc;
    proc.SetLeftResultOperand(TFrac("1/2"));
    proc.SetRightOperand(TFrac("0/1"));
    proc.SetOperation(ADT_Proc<TFrac>::OperationType::Div); // 4

    QVERIFY_EXCEPTION_THROWN(proc.DoOperation(), std::runtime_error);

    QCOMPARE(proc.GetLeftResultOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetRightOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetOperation(), ADT_Proc<TFrac>::OperationType::None);
}


void TestADTProc::testDoFunctionReverse()
{
    ADT_Proc<TFrac> proc;
    proc.SetRightOperand(TFrac("-22/3"));
    proc.DoFunction(0); // ReverseFunc
    QCOMPARE(proc.GetRightOperand(), TFrac("-3/22"));
    // Левый операнд не должен меняться
    QCOMPARE(proc.GetLeftResultOperand(), TFrac("0/1"));
}

void TestADTProc::testDoFunctionSquare()
{
     ADT_Proc<TFrac> proc;
     proc.SetRightOperand(TFrac("-22/3"));
     proc.DoFunction(1); // SquareFunc
     QCOMPARE(proc.GetRightOperand(), TFrac("484/9")); // (-22)^2 / 3^2
      // Левый операнд не должен меняться
     QCOMPARE(proc.GetLeftResultOperand(), TFrac("0/1"));
}

void TestADTProc::testDoFunctionReverseZero()
{
    ADT_Proc<TFrac> proc;
    proc.SetRightOperand(TFrac("0/1"));

    QVERIFY_EXCEPTION_THROWN(proc.DoFunction(0), std::runtime_error);

    QCOMPARE(proc.GetLeftResultOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetRightOperand(), TFrac(0, 1));
    QCOMPARE(proc.GetOperation(), ADT_Proc<TFrac>::OperationType::None);
}
