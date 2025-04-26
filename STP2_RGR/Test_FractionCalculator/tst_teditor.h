#ifndef TST_TEDITOR_H
#define TST_TEDITOR_H

#include <QObject>
#include <QtTest>
#include <QString>

#include "../FractionCalculator/teditor.h" // Включаем тестируемый класс

class TestTEditor : public QObject
{
    Q_OBJECT

public:
    TestTEditor();
    ~TestTEditor();

private slots:
    // Тесты на инициализацию и парсинг (через SetFractionString/SetEditorFromFrac)
    void testInit();
    void testInit_data();

    // Тесты на IsZero
    void testIsZero();
    void testIsZero_data();

    // Тесты на ToggleMinus
    void testToggleMinus();
    void testToggleMinus_data();

    // Тесты на добавление символов
    void testAddNumber();
    void testAddSeparator();
    void testAddZero();

    // Тест на удаление символа
    void testRemoveSymbol();

    // Тест на очистку
    void testClear();

    // Комплексные тесты на метод Edit
    void testEditSequence();
};

#endif // TST_TEDITOR_H
