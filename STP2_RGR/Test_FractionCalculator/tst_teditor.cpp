#include "tst_teditor.h"

TestTEditor::TestTEditor() {}
TestTEditor::~TestTEditor() {}

// --- Данные для тестов ---

void TestTEditor::testInit_data()
{
    QTest::addColumn<QString>("inputString");
    QTest::addColumn<QString>("expectedEditorString");

    // Используем SetFractionString, который нормализует через TFrac
    QTest::newRow("Simple") << QString("3/4") << QString("3/4");
    QTest::newRow("Negative") << QString("-16/3") << QString("-16/3");
    QTest::newRow("Simplify") << QString("0/8") << QString("0/1");
    QTest::newRow("NoSimplify") << QString("-17/4") << QString("-17/4");
    QTest::newRow("Zero") << QString("0/1") << QString("0/1");
    QTest::newRow("Simplify2") << QString("666/6666") << QString("111/1111");
    QTest::newRow("Invalid") << QString("aaaa") << QString("0/1"); // TFrac вернет 0/1
    QTest::newRow("ZeroDenominator") << QString("16/0") << QString("0/1");
    QTest::newRow("Integer") << QString("5") << QString("5/1"); // TFrac -> "5/1"
}

void TestTEditor::testIsZero_data()
{
     QTest::addColumn<QString>("editorString"); // Подаем строку как она есть в редакторе
     QTest::addColumn<bool>("expectedIsZero");

     QTest::newRow("ZeroNum") << QString("0") << true;
     QTest::newRow("ZeroFrac") << QString("0/1") << true;
     QTest::newRow("NegZeroNum") << QString("-0") << true;
     QTest::newRow("NegZeroFrac") << QString("-0/1") << true; // TFrac нормализует к 0/1, но IsZero должно работать
     QTest::newRow("NonZero") << QString("14/3") << false;
     QTest::newRow("NonZeroNeg") << QString("-1/2") << false;
     QTest::newRow("AfterClear") << QString("0") << true; // После Clear()
}

void TestTEditor::testToggleMinus_data()
{
    QTest::addColumn<QString>("initialString");
    QTest::addColumn<QString>("expectedAfterToggle");

    QTest::newRow("PositiveFrac") << QString("14/3") << QString("-14/3");
    QTest::newRow("NegativeFrac") << QString("-14/3") << QString("14/3");
    QTest::newRow("PositiveInt") << QString("5") << QString("-5/1");
    QTest::newRow("NegativeInt") << QString("-5") << QString("5/1");
    QTest::newRow("Zero") << QString("0") << QString("0/1");
    QTest::newRow("NegZero") << QString("-0") << QString("0/1");
    QTest::newRow("ZeroFrac") << QString("0/1") << QString("0/1");
    QTest::newRow("Slash") << QString("5/") << QString("0/1");
    QTest::newRow("NegSlash") << QString("-5/") << QString("0/1");
}

// --- Тестовые методы ---

void TestTEditor::testInit()
{
    QFETCH(QString, inputString);
    QFETCH(QString, expectedEditorString);

    TEditor editor;
    editor.SetFractionString(inputString); // Используем метод установки
    QCOMPARE(editor.GetFractionString(), expectedEditorString);
}

void TestTEditor::testIsZero()
{
    QFETCH(QString, editorString);
    QFETCH(bool, expectedIsZero);

    TEditor editor;
    editor.SetFractionString(editorString); // Устанавливаем строку напрямую
    QCOMPARE(editor.IsZero(), expectedIsZero);
}

void TestTEditor::testToggleMinus()
{
    QFETCH(QString, initialString);
    QFETCH(QString, expectedAfterToggle);

    TEditor editor;
    editor.SetFractionString(initialString); // Устанавливаем начальное значение
    editor.ToggleMinus();
    QCOMPARE(editor.GetFractionString(), expectedAfterToggle);
}

void TestTEditor::testAddNumber()
{
    TEditor editor; // Начинает с "0"
    QCOMPARE(editor.AddNumber(1), QString("1"));
    QCOMPARE(editor.AddNumber(2), QString("12"));
    QCOMPARE(editor.AddNumber(3), QString("123"));
    QCOMPARE(editor.AddSeparator(), QString("123/")); // Добавим разделитель
    QCOMPARE(editor.AddNumber(4), QString("123/4"));
    QCOMPARE(editor.AddNumber(5), QString("123/45"));
    QCOMPARE(editor.AddNumber(6), QString("123/456"));

    // Тест на не добавление нуля в начало (кроме самого нуля)
    editor.Clear();
    QCOMPARE(editor.AddNumber(0), QString("0")); // Ноль остается нулем
    QCOMPARE(editor.AddNumber(5), QString("5")); // Теперь 5
    QCOMPARE(editor.AddSeparator(), QString("5/"));
    QCOMPARE(editor.AddNumber(0), QString("5/0")); // Ноль после / можно
    QCOMPARE(editor.AddNumber(1), QString("5/01"));
}

void TestTEditor::testAddSeparator()
{
    TEditor editor;
    QCOMPARE(editor.AddNumber(1), QString("1"));
    QCOMPARE(editor.AddSeparator(), QString("1/"));
    QCOMPARE(editor.AddSeparator(), QString("1/")); // Повторно не добавляет
    QCOMPARE(editor.AddNumber(2), QString("1/2"));
    QCOMPARE(editor.AddSeparator(), QString("1/2")); // Не добавляет после цифр знаменателя

    editor.Clear();
    QCOMPARE(editor.AddSeparator(), QString("0")); // Добавляет к нулю
    editor.Clear();
    QCOMPARE(editor.ToggleMinus(), QString("-0"));
    QCOMPARE(editor.AddSeparator(), QString("-0")); // Добавляет к -0
}

void TestTEditor::testAddZero()
{
    TEditor editor;
    QCOMPARE(editor.AddZero(), QString("0"));
    QCOMPARE(editor.AddNumber(1), QString("1"));
    QCOMPARE(editor.AddZero(), QString("10"));
    QCOMPARE(editor.AddZero(), QString("100"));
    QCOMPARE(editor.AddSeparator(), QString("100/"));
    QCOMPARE(editor.AddNumber(5), QString("100/5"));
    QCOMPARE(editor.AddZero(), QString("100/50"));
    QCOMPARE(editor.AddZero(), QString("100/500"));
}


void TestTEditor::testRemoveSymbol()
{
    TEditor editor;
    editor.Edit(1); editor.Edit(2); editor.Edit(3); // "123"
    editor.Edit(11); // "123/"
    editor.Edit(4); editor.Edit(5); editor.Edit(6); // "123/456"

    QCOMPARE(editor.GetFractionString(), QString("123/456"));

    QString result1 = editor.RemoveSymbol();
    QCOMPARE(result1, QString("123/45"));
    QCOMPARE(editor.GetFractionString(), QString("123/45"));

    QString result2 = editor.RemoveSymbol();
    QCOMPARE(result2, QString("123/4"));
    QCOMPARE(editor.GetFractionString(), QString("123/4"));

    QString result3 = editor.RemoveSymbol();
    QCOMPARE(result3, QString("123"));
    QCOMPARE(editor.GetFractionString(), QString("123"));

    QString result4 = editor.RemoveSymbol();
    QCOMPARE(result4, QString("12"));
    QCOMPARE(editor.GetFractionString(), QString("12"));

    QCOMPARE(editor.RemoveSymbol(), QString("1"));
    QCOMPARE(editor.RemoveSymbol(), QString("0")); // Становится нулем
    QCOMPARE(editor.RemoveSymbol(), QString("0")); // Дальше не удаляет
}

void TestTEditor::testClear()
{
    TEditor editor;
    editor.SetFractionString("-123/456");
    QCOMPARE(editor.Clear(), QString("0")); // Команда CE
    QCOMPARE(editor.GetFractionString(), QString("0")); // Проверка состояния
}

void TestTEditor::testEditSequence()
{
    TEditor editor;
    editor.SetFractionString("123/123");
    QCOMPARE(editor.Edit(0), QString("1/10"));
    QCOMPARE(editor.Edit(1), QString("1/101"));
    QCOMPARE(editor.Edit(3), QString("1/1013"));
    QCOMPARE(editor.Edit(11), QString("1/1013"));
    QCOMPARE(editor.Edit(10), QString("-1/1013"));
    QCOMPARE(editor.ToString(), QString("-1/1013"));

    editor.SetEditorFromFrac(TFrac(123,123));
    editor.Edit(12);
    editor.Edit(12);
    editor.Edit(12);
    editor.Edit(1); QCOMPARE(editor.GetFractionString(), QString("1"));
    editor.Edit(2); QCOMPARE(editor.GetFractionString(), QString("12"));
    editor.Edit(3); QCOMPARE(editor.GetFractionString(), QString("123"));
    editor.Edit(4); QCOMPARE(editor.GetFractionString(), QString("1234"));
    editor.Edit(5); QCOMPARE(editor.GetFractionString(), QString("12345"));
    editor.Edit(11); QCOMPARE(editor.GetFractionString(), QString("12345/"));
    editor.Edit(1); QCOMPARE(editor.GetFractionString(), QString("12345/1"));
    editor.Edit(1); QCOMPARE(editor.GetFractionString(), QString("12345/11"));
    editor.Edit(1); QCOMPARE(editor.GetFractionString(), QString("12345/111"));
    editor.Edit(1); QCOMPARE(editor.GetFractionString(), QString("12345/1111"));
    QCOMPARE(editor.ToString(), QString("12345/1111"));

    editor.SetFractionString("1234567/12345678");
    for (int i = 0; i < 100; ++i) editor.Edit(12);
    QCOMPARE(editor.GetFractionString(), QString("0"));
    for (int i = 0; i < 100; ++i) editor.Edit(11);
    QCOMPARE(editor.GetFractionString(), QString("0"));
    editor.Edit(1); QCOMPARE(editor.GetFractionString(), QString("1"));

    editor.Edit(13);
    QCOMPARE(editor.GetFractionString(), QString("0"));
    for (int i = 1; i <= 9; ++i) editor.Edit(i);
    QCOMPARE(editor.GetFractionString(), QString("123456789"));

}
