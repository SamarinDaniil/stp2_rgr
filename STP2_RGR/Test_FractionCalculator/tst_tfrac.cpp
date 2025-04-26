#include "tst_tfrac.h"

TestTFrac::TestTFrac() {}
TestTFrac::~TestTFrac() {}

void TestTFrac::testInitFromString_data()
{
    QTest::addColumn<QString>("inputString");
    QTest::addColumn<QString>("expectedString");

    QTest::newRow("Simple") << QString("1/2") << QString("1/2");
    QTest::newRow("Longer") << QString("111/2") << QString("111/2");
    QTest::newRow("NegativeSimplify") << QString("-100/60") << QString("-5/3");
    QTest::newRow("LeadingZeros") << QString("00000003/000004") << QString("3/4");
    QTest::newRow("NegativeLeadingZeros") << QString("-00000003/000004") << QString("-3/4");
    QTest::newRow("ZeroDenominator") << QString("5/0") << QString("0/1"); // TFrac обрабатывает это как 0/1
    QTest::newRow("Integer") << QString("5") << QString("5/1");
    QTest::newRow("NegativeInteger") << QString("-10") << QString("-10/1");
    QTest::newRow("Invalid") << QString("abc") << QString("0/1");
    QTest::newRow("MixedInvalid") << QString("5/abc") << QString("0/1");
    QTest::newRow("SpaceAroundSlash") << QString(" 3 / 4 ") << QString("3/4");
}

void TestTFrac::testInitFromNumber_data()
{
    QTest::addColumn<long long>("num");
    QTest::addColumn<long long>("den");
    QTest::addColumn<QString>("expectedString");

    QTest::newRow("Simple") << 1LL << 2LL << QString("1/2");
    QTest::newRow("Simplify") << 100LL << 100LL << QString("1/1");
    QTest::newRow("DoubleNegative") << -100LL << -99LL << QString("100/99");
    QTest::newRow("ZeroZero") << 0LL << 0LL << QString("0/1"); // TFrac обрабатывает это как 0/1
    QTest::newRow("ZeroDenominator") << 5LL << 0LL << QString("0/1");
    QTest::newRow("NegativeDenominator") << 50LL << -5LL << QString("-10/1");
    QTest::newRow("ZeroNumerator") << 0LL << 5LL << QString("0/1");
}

void TestTFrac::testAddition_data()
{
    QTest::addColumn<QString>("frac1");
    QTest::addColumn<QString>("frac2");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Add1") << QString("1/4") << QString("-3/4") << QString("-1/2");
    QTest::newRow("Add2") << QString("-1/2") << QString("-1/2") << QString("-1/1");
    QTest::newRow("Add3") << QString("-6/2") << QString("6/2") << QString("0/1"); // -3 + 3 = 0
    QTest::newRow("Add4") << QString("50/3") << QString("0/1") << QString("50/3");
    QTest::newRow("Add5") << QString("0/1") << QString("0/1") << QString("0/1");
}

void TestTFrac::testSubtraction_data()
{
    QTest::addColumn<QString>("frac1");
    QTest::addColumn<QString>("frac2");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Sub1") << QString("0/1") << QString("1/1") << QString("-1/1");
    QTest::newRow("Sub2") << QString("5/1") << QString("1/1") << QString("4/1");
    QTest::newRow("Sub3") << QString("1/2") << QString("1/2") << QString("0/1");
    QTest::newRow("Sub4") << QString("-1/6") << QString("-1/6") << QString("0/1");
    QTest::newRow("Sub5") << QString("-1/6") << QString("2/6") << QString("-1/2"); // -1/6 - 1/3 = -1/6 - 2/6 = -3/6
}

void TestTFrac::testMultiplication_data()
{
     QTest::addColumn<QString>("frac1");
    QTest::addColumn<QString>("frac2");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Mul1") << QString("-1/2") << QString("-1/2") << QString("1/4");
    QTest::newRow("Mul2") << QString("1/6") << QString("0/1") << QString("0/1");
    QTest::newRow("Mul3") << QString("1/6") << QString("1/6") << QString("1/36");
    QTest::newRow("Mul4") << QString("-1/6") << QString("12/1") << QString("-2/1");
    // Mul5 is duplicate of Mul4 in C#
}

void TestTFrac::testDivision_data()
{
    QTest::addColumn<QString>("frac1");
    QTest::addColumn<QString>("frac2");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Div1") << QString("5/6") << QString("1/1") << QString("5/6");
    QTest::newRow("Div2") << QString("1/1") << QString("5/6") << QString("6/5");
    QTest::newRow("Div3") << QString("0/1") << QString("5/6") << QString("0/1");
    QTest::newRow("Div4") << QString("2/3") << QString("7/4") << QString("8/21");
    QTest::newRow("Div5") << QString("2/3") << QString("2/3") << QString("1/1");
}

void TestTFrac::testSquare_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Square1") << QString("2/3") << QString("4/9");
    QTest::newRow("Square2") << QString("0/1") << QString("0/1");
    QTest::newRow("Square3") << QString("-2/3") << QString("4/9");
}

void TestTFrac::testReverse_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Reverse1") << QString("-2/3") << QString("-3/2");
    // Reverse2 (0/1) handled separately as it should throw
    QTest::newRow("Reverse3") << QString("5/6") << QString("6/5");
    QTest::newRow("ReverseInteger") << QString("5/1") << QString("1/5");
}


void TestTFrac::testEquality_data() {
    QTest::addColumn<QString>("frac1");
    QTest::addColumn<QString>("frac2");
    QTest::addColumn<bool>("expectedEqual");

    QTest::newRow("Equal1") << QString("1/3") << QString("1/3") << true;
    QTest::newRow("Equal2") << QString("0/6") << QString("1/6") << false; // 0/1 != 1/6
    QTest::newRow("Equal3") << QString("-1/6") << QString("-1/6") << true;
    QTest::newRow("Equal4") << QString("-1/7") << QString("1/7") << false;
    QTest::newRow("Equal5") << QString("1/6") << QString("0/1") << false;
    QTest::newRow("EqualSimplify") << QString("2/4") << QString("1/2") << true;
    QTest::newRow("EqualSimplifyNeg") << QString("-4/6") << QString("-2/3") << true;
}

void TestTFrac::testGreaterThan_data() {
    QTest::addColumn<QString>("frac1");
    QTest::addColumn<QString>("frac2");
    QTest::addColumn<bool>("expectedGreater");

    QTest::newRow("Greater1") << QString("1/6") << QString("0/1") << true;
    QTest::newRow("Greater2") << QString("0/1") << QString("0/1") << false;
    QTest::newRow("Greater3") << QString("-1/6") << QString("0/1") << false;
    QTest::newRow("Greater4") << QString("17/3") << QString("16/3") << true;
    QTest::newRow("Greater5") << QString("-2/3") << QString("-1/3") << false;
    QTest::newRow("GreaterEqual") << QString("1/2") << QString("1/2") << false;
    QTest::newRow("GreaterSimplify") << QString("3/4") << QString("1/2") << true; // 3/4 > 2/4
}

void TestTFrac::testInitFromString()
{
    QFETCH(QString, inputString);
    QFETCH(QString, expectedString);

    TFrac frac(inputString);
    QCOMPARE(frac.ToString(), expectedString);
}

void TestTFrac::testInitFromNumber()
{
    QFETCH(long long, num);
    QFETCH(long long, den);
    QFETCH(QString, expectedString);

    TFrac frac(num, den);
    QCOMPARE(frac.ToString(), expectedString);
}


void TestTFrac::testAddition()
{
    QFETCH(QString, frac1);
    QFETCH(QString, frac2);
    QFETCH(QString, expectedResult);

    TFrac f1(frac1);
    TFrac f2(frac2);
    TFrac result = f1.Add(f2);
    QCOMPARE(result.ToString(), expectedResult);
}

void TestTFrac::testSubtraction()
{
    QFETCH(QString, frac1);
    QFETCH(QString, frac2);
    QFETCH(QString, expectedResult);

    TFrac f1(frac1);
    TFrac f2(frac2);
    TFrac result = f1.Sub(f2);
    QCOMPARE(result.ToString(), expectedResult);
}

void TestTFrac::testMultiplication()
{
     QFETCH(QString, frac1);
    QFETCH(QString, frac2);
    QFETCH(QString, expectedResult);

    TFrac f1(frac1);
    TFrac f2(frac2);
    TFrac result = f1.Mul(f2);
    QCOMPARE(result.ToString(), expectedResult);
}

void TestTFrac::testDivision()
{
     QFETCH(QString, frac1);
    QFETCH(QString, frac2);
    QFETCH(QString, expectedResult);

    TFrac f1(frac1);
    TFrac f2(frac2);
    // Деление на ноль проверим отдельно
    if (TFrac(frac2) == TFrac(0,1)) return;

    TFrac result = f1.Div(f2);
    QCOMPARE(result.ToString(), expectedResult);
}

void TestTFrac::testSquare()
{
    QFETCH(QString, input);
    QFETCH(QString, expectedResult);

    TFrac f(input);
    TFrac result = f.Square();
    QCOMPARE(result.ToString(), expectedResult);
}

void TestTFrac::testReverse()
{
     QFETCH(QString, input);
    QFETCH(QString, expectedResult);

    TFrac f(input);
    if (f == TFrac(0,1)) return;

    TFrac result = f.Reverse();
    QCOMPARE(result.ToString(), expectedResult);
}

void TestTFrac::testEquality() {
    QFETCH(QString, frac1);
    QFETCH(QString, frac2);
    QFETCH(bool, expectedEqual);

    TFrac f1(frac1);
    TFrac f2(frac2);

    QCOMPARE(f1.Equal(f2), expectedEqual);
    QCOMPARE(f1 == f2, expectedEqual); // Проверим и оператор ==
}

void TestTFrac::testGreaterThan() {
    QFETCH(QString, frac1);
    QFETCH(QString, frac2);
    QFETCH(bool, expectedGreater);

    TFrac f1(frac1);
    TFrac f2(frac2);

    QCOMPARE(f1 > f2, expectedGreater);
}


void TestTFrac::testDivisionByZero()
{
    TFrac f1("5/6");
    TFrac f0("0/1");
    // TFrac::Div должен бросать исключение std::runtime_error
    QVERIFY_EXCEPTION_THROWN(f1.Div(f0), std::runtime_error);

    // Также проверим деление на дробь, которая станет нулем после парсинга
    TFrac f0_str("0/5");
    QVERIFY_EXCEPTION_THROWN(f1.Div(f0_str), std::runtime_error);
}

void TestTFrac::testReverseZero()
{
    TFrac f0("0/1");
    // TFrac::Reverse должен бросать исключение std::runtime_error для нуля
    QVERIFY_EXCEPTION_THROWN(f0.Reverse(), std::runtime_error);
}
