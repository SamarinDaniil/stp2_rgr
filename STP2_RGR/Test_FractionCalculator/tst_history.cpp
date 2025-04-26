#include "tst_history.h"

TestHistory::TestHistory() {}
TestHistory::~TestHistory() {}

void TestHistory::testInit()
{
    History history;
    QCOMPARE(history.count(), 0);
    QVERIFY(history.getRecords().isEmpty());
}

void TestHistory::testAddRecord()
{
    History history;
    QString expr1 = "1/2 + 1/4 = 3/4";
    QString expr2 = "3/4 * 2/1 = 3/2";

    history.addRecord(expr1);
    QCOMPARE(history.count(), 1);
    QCOMPARE(history.record(0).toString(), expr1);

    history.addRecord(expr2);
    QCOMPARE(history.count(), 2);
    // Новые записи добавляются в конец списка
    QCOMPARE(history.record(0).toString(), expr1);
    QCOMPARE(history.record(1).toString(), expr2);
}

void TestHistory::testGetRecord()
{
    History history;
    QString expr1 = "1 = 1";
    QString expr2 = "2 = 2";
    history.addRecord(expr1);
    history.addRecord(expr2);

    QCOMPARE(history.record(0).toString(), expr1);
    QCOMPARE(history.record(1).toString(), expr2);

    // Проверка невалидных индексов
    QCOMPARE(history.record(-1).toString(), QString("")); // Ожидаем пустую строку
    QCOMPARE(history.record(2).toString(), QString(""));
}

void TestHistory::testClear()
{
    History history;
    history.addRecord("1+1=2");
    history.addRecord("2*2=4");
    QCOMPARE(history.count(), 2);

    history.clear();
    QCOMPARE(history.count(), 0);
    QVERIFY(history.getRecords().isEmpty());
}

void TestHistory::testCount()
{
     History history;
     QCOMPARE(history.count(), 0);
     history.addRecord("a=b");
     QCOMPARE(history.count(), 1);
     history.addRecord("c=d");
     QCOMPARE(history.count(), 2);
     history.clear();
     QCOMPARE(history.count(), 0);
}


void TestHistory::testGetRecords()
{
    History history;
    QString expr1 = "1";
    QString expr2 = "2";
    history.addRecord(expr1);
    history.addRecord(expr2);

    const QList<CalcRecord>& records = history.getRecords();
    QCOMPARE(records.size(), 2);
    QCOMPARE(records.at(0).toString(), expr1);
    QCOMPARE(records.at(1).toString(), expr2);
}

void TestHistory::testMaxSize()
{
    History history;

    const int maxSize = 50;

    for (int i = 1; i <= maxSize + 10; ++i) {
        history.addRecord(QString("Rec %1").arg(i));
    }

    QCOMPARE(history.count(), maxSize);

    // Проверяем, что остались последние maxSize записей
    // Первая запись должна быть "Rec 11"
    QCOMPARE(history.record(0).toString(), QString("Rec 11"));
    // Последняя запись должна быть "Rec 60"
    QCOMPARE(history.record(maxSize - 1).toString(), QString("Rec 60"));
}
