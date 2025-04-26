#ifndef TFRAC_H
#define TFRAC_H

#include <QString>
#include <stdexcept>
#include <numeric>

class TFrac {
private:
    long long numerator;
    long long denominator;

    void simplify();

public:
    // Конструкторы
    TFrac();
    TFrac(long long num, long long den);
    explicit TFrac(const QString& fracStr);
    // Методы доступа
    long long getNumerator() const;
    long long getDenominator() const;
    QString getNumeratorString() const;
    QString getDenominatorString() const;

    void setNumerator(long long num);
    void setDenominator(long long den);
    void setString(const QString& str);

    // Арифметические операции (возвращают новый объект)
    TFrac Add(const TFrac& other) const;
    TFrac Sub(const TFrac& other) const;
    TFrac Mul(const TFrac& other) const;
    TFrac Div(const TFrac& other) const;

    // Унарные операции (возвращают новый объект)
    TFrac Square() const;
    TFrac Reverse() const;
    TFrac Minus() const;

    // Сравнение
    bool Equal(const TFrac& other) const;
    bool operator>(const TFrac& other) const;
    bool operator<(const TFrac& other) const;
    bool operator==(const TFrac& other) const;

    // Преобразование в строку
    QString ToString() const;

    static long long GCD(long long a, long long b);
};


#endif // TFRAC_H
