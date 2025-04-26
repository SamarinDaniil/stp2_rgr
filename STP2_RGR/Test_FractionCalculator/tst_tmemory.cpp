#include "tst_tmemory.h"

TestTMemory::TestTMemory() {}
TestTMemory::~TestTMemory() {}

void TestTMemory::testInitAndRecall_data()
{
    QTest::addColumn<QString>("initialFrac");
    QTest::addColumn<QString>("expectedRecall");
    QTest::addColumn<bool>("expectedInitialState");

    QTest::newRow("Simple") << QString("22/33") << QString("2/3") << true; // Store устанавливает state
    QTest::newRow("Zero") << QString("0/1") << QString("0/1") << true;
    QTest::newRow("Negative") << QString("-1/5") << QString("-1/5") << true;
}

void TestTMemory::testAdd_data()
{
    QTest::addColumn<QString>("initialFrac");
    QTest::addColumn<QString>("addFrac");
    QTest::addColumn<QString>("expectedResult");
    QTest::addColumn<bool>("expectedStateAfterAdd");

    QTest::newRow("Add1") << QString("-1/5") << QString("1/2") << QString("3/10") << true;
    QTest::newRow("Add2") << QString("8/9") << QString("-16/3") << QString("-40/9") << true;
    QTest::newRow("AddToZero") << QString("0/1") << QString("1/3") << QString("1/3") << true;
}

void TestTMemory::testInitAndRecall()
{
    QFETCH(QString, initialFrac);
    QFETCH(QString, expectedRecall);
    QFETCH(bool, expectedInitialState);

    TFrac frac(initialFrac);
    TMemory<TFrac> memory;

    // Проверяем начальное состояние (пустое)
    QVERIFY(!memory.FState());

    // Используем Store (или Edit 0) для инициализации
    memory.Store(frac);

    QCOMPARE(memory.FNumber().ToString(), expectedRecall); // Проверяем сохраненное значение
    QCOMPARE(memory.FState(), expectedInitialState); // Проверяем состояние после Store
    QCOMPARE(memory.Recall().ToString(), expectedRecall); // Проверяем Recall
}


void TestTMemory::testAdd()
{
    QFETCH(QString, initialFrac);
    QFETCH(QString, addFrac);
    QFETCH(QString, expectedResult);
    QFETCH(bool, expectedStateAfterAdd);

    TMemory<TFrac> memory;
    memory.Store(TFrac(initialFrac)); // Инициализируем память

    TFrac adder(addFrac);
    memory.Add(adder); // Выполняем M+

    QCOMPARE(memory.FNumber().ToString(), expectedResult);
    QCOMPARE(memory.FState(), expectedStateAfterAdd);

    // Проверим добавление к пустой памяти
    TMemory<TFrac> emptyMemory;
    QVERIFY(!emptyMemory.FState());
    emptyMemory.Add(TFrac("1/3")); // Должно сработать как Store
    QCOMPARE(emptyMemory.FNumber().ToString(), QString("1/3"));
    QVERIFY(emptyMemory.FState());
}

void TestTMemory::testClear()
{
    TMemory<TFrac> memory;
    memory.Store(TFrac("5/7")); // Заполняем память

    QVERIFY(memory.FState()); // Убеждаемся, что не пусто

    memory.Clear(); // MC

    QCOMPARE(memory.FNumber().ToString(), QString("0/1")); // Значение сброшено
    QVERIFY(!memory.FState()); // Состояние false
}

void TestTMemory::testState()
{
    TMemory<TFrac> memory;
    // Изначально состояние false
    QVERIFY(!memory.FState());

    // После Store - true
    memory.Store(TFrac("1/1"));
    QVERIFY(memory.FState());

    // После Add к непустой - true
    memory.Add(TFrac("1/1"));
    QVERIFY(memory.FState());

    // После Clear - false
    memory.Clear();
    QVERIFY(!memory.FState());

     // После Add к пустой - true
    memory.Add(TFrac("1/1"));
    QVERIFY(memory.FState());

    // Чтение FNumber не должно менять состояние (в отличие от C# кода)
    memory.FNumber();
    QVERIFY(memory.FState());

    // Recall не должен менять состояние
    memory.Recall();
    QVERIFY(memory.FState());
}

void TestTMemory::testEditStore()
{
    TMemory<TFrac> memory;
    TFrac fracToStore("9/8");
    std::pair<TFrac, bool> result = memory.Edit(0, fracToStore); // MS (command 0)

    QCOMPARE(result.first.ToString(), QString("9/8")); // Возвращенное значение
    QCOMPARE(result.second, true);                  // Возвращенное состояние
    QCOMPARE(memory.FNumber().ToString(), QString("9/8")); // Внутреннее значение
    QCOMPARE(memory.FState(), true);                   // Внутреннее состояние
}

void TestTMemory::testEditAdd()
{
    TMemory<TFrac> memory;
    memory.Store(TFrac("1/8")); // Начальное значение

    TFrac fracToAdd("3/8");
    std::pair<TFrac, bool> result = memory.Edit(1, fracToAdd); // M+ (command 1)

    QCOMPARE(result.first.ToString(), QString("1/2")); // Возвращенное значение (1/8 + 3/8 = 4/8 = 1/2)
    QCOMPARE(result.second, true);                  // Возвращенное состояние
    QCOMPARE(memory.FNumber().ToString(), QString("1/2")); // Внутреннее значение
    QCOMPARE(memory.FState(), true);                   // Внутреннее состояние

    // Тест M+ на пустой памяти
    TMemory<TFrac> emptyMemory;
     result = emptyMemory.Edit(1, TFrac("5/6"));
     QCOMPARE(result.first.ToString(), QString("5/6"));
     QCOMPARE(result.second, true);
     QCOMPARE(emptyMemory.FNumber().ToString(), QString("5/6"));
     QCOMPARE(emptyMemory.FState(), true);
}

void TestTMemory::testEditClear()
{
     TMemory<TFrac> memory;
     memory.Store(TFrac("1/1"));

     std::pair<TFrac, bool> result = memory.Edit(2, TFrac()); // MC (command 2), передаваемая дробь игнорируется

     QCOMPARE(result.first.ToString(), QString("0/1")); // Возвращенное значение
     QCOMPARE(result.second, false);                 // Возвращенное состояние
     QCOMPARE(memory.FNumber().ToString(), QString("0/1")); // Внутреннее значение
     QCOMPARE(memory.FState(), false);                  // Внутреннее состояние
}

void TestTMemory::testEditRecall()
{

    TMemory<TFrac> memory;
    memory.Store(TFrac("7/11"));
    TFrac beforeVal = memory.FNumber();
    bool beforeState = memory.FState();

    std::pair<TFrac, bool> result = memory.Edit(3, TFrac("1/1")); // Вызываем Edit с командой 3

    // Проверяем, что Edit вернул текущее состояние, не изменив его
    QCOMPARE(result.first.ToString(), beforeVal.ToString());
    QCOMPARE(result.second, beforeState);
    // Проверяем, что внутреннее состояние не изменилось
    QCOMPARE(memory.FNumber().ToString(), beforeVal.ToString());
    QCOMPARE(memory.FState(), beforeState);

}
