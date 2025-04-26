#ifndef HISTORY_H
#define HISTORY_H

#include <QString>
#include <QList>
//#include <QAction>

struct CalcRecord {
    QString expression; // Строка вида "5/2 + 1/3 = 17/6"

    // Конструктор для удобства
    explicit CalcRecord(const QString& expr) : expression(expr) {}

    // Метод для получения строки
    QString toString() const {
        return expression;
    }
};

// Класс для управления историей вычислений
class History {
public:
    History();

    // Добавляем запись в историю
    void addRecord(const QString& expressionResult);

    // Получаем запись по индексу
    CalcRecord record(int index) const;

    // Очищаем всю историю
    void clear();

    // Получаем количество записей
    int count() const;

    // Получаем все записи
    const QList<CalcRecord>& getRecords() const;

private:
    QList<CalcRecord> records;
    const int maxHistorySize = 50;
};

#endif // HISTORY_H
