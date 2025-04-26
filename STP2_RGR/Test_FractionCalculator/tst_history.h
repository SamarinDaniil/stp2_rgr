#ifndef TST_HISTORY_H
#define TST_HISTORY_H

#include <QObject>
#include <QtTest>
#include <QString>

#include "../FractionCalculator/history.h"

class TestHistory : public QObject
{
    Q_OBJECT

public:
    TestHistory();
    ~TestHistory();

private slots:
    void testInit();       // Проверка начального состояния
    void testAddRecord();  // Добавление записей
    void testGetRecord();  // Получение записей по индексу
    void testClear();      // Очистка истории
    void testCount();      // Проверка количества записей
    void testGetRecords(); // Получение всего списка
    void testMaxSize();    // Проверка ограничения размера (если есть)
};

#endif // TST_HISTORY_H
