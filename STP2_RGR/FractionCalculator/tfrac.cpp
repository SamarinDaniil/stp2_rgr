#include "tfrac.h"
#include <QStringList>
#include <QRegularExpression>
#include <QtMath>


// Статическая функция GCD
long long TFrac::GCD(long long a, long long b) {
    a = qAbs(a);
    b = qAbs(b);
    while (b > 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

// Вспомогательная функция для сокращения
void TFrac::simplify() {
    if (denominator == 0) {
        numerator = 0;
        denominator = 1;
        return;
    }

    if (numerator == 0) {
        denominator = 1;
        return;
    }

    if (denominator < 0) {
        numerator *= -1;
        denominator *= -1;
    }

    // Сокращение
    long long common = GCD(numerator, denominator);
     if (common > 1) {
         numerator /= common;
         denominator /= common;
     } else if (common < -1) {
         numerator /= -common;
         denominator /= -common;
     }
}

TFrac::TFrac() : numerator(0), denominator(1) {}

TFrac::TFrac(long long num, long long den) : numerator(num), denominator(den) {
    simplify();
}

TFrac::TFrac(const QString& fracStr) : numerator(0), denominator(1) {
    setString(fracStr);
}

long long TFrac::getNumerator() const { return numerator; }
long long TFrac::getDenominator() const { return denominator; }
QString TFrac::getNumeratorString() const { return QString::number(numerator); }
QString TFrac::getDenominatorString() const { return QString::number(denominator); }

void TFrac::setNumerator(long long num) {
    numerator = num;
    simplify();
}

void TFrac::setDenominator(long long den) {
    if (den == 0) {
        numerator = 0;
        denominator = 1;
    } else {
        denominator = den;
        simplify();
    }
}

void TFrac::setString(const QString& str) {
    QString trimmedStr = str.trimmed();
    QRegularExpression fracRegex(R"(^(-?\d+)\s*\/\s*(\d+)$)");
    QRegularExpression intRegex(R"(^-?\d+$)");

    QRegularExpressionMatch fracMatch = fracRegex.match(trimmedStr);
    QRegularExpressionMatch intMatch = intRegex.match(trimmedStr);

    bool okNum, okDen;

    if (fracMatch.hasMatch()) {
        long long num = fracMatch.captured(1).toLongLong(&okNum);
        long long den = fracMatch.captured(2).toLongLong(&okDen);

        if (okNum && okDen) {
             if (den == 0) {
                numerator = 0;
                denominator = 1;
            } else {
                numerator = num;
                denominator = den;
                simplify();
            }
        } else {
            numerator = 0;
            denominator = 1;
        }
    } else if (intMatch.hasMatch()) {
        long long num = trimmedStr.toLongLong(&okNum);
        if (okNum) {
            numerator = num;
            denominator = 1;
        } else {
            numerator = 0;
            denominator = 1;
        }
    } else {
        numerator = 0;
        denominator = 1;
    }
}

// Арифметические операции
TFrac TFrac::Add(const TFrac& other) const {
    long long newNum = numerator * other.denominator + denominator * other.numerator;
    long long newDen = denominator * other.denominator;
    return TFrac(newNum, newDen);
}

TFrac TFrac::Sub(const TFrac& other) const {
    long long newNum = numerator * other.denominator - denominator * other.numerator;
    long long newDen = denominator * other.denominator;
    return TFrac(newNum, newDen);
}

TFrac TFrac::Mul(const TFrac& other) const {
    long long newNum = numerator * other.numerator;
    long long newDen = denominator * other.denominator;
    return TFrac(newNum, newDen);
}

TFrac TFrac::Div(const TFrac& other) const {
    if (other.numerator == 0) {
        throw std::runtime_error("Division by zero fraction.");
    }
    long long newNum = numerator * other.denominator;
    long long newDen = denominator * other.numerator;
    return TFrac(newNum, newDen);
}

// Унарные операции
TFrac TFrac::Square() const {
    long long newNum = numerator * numerator;
    long long newDen = denominator * denominator;

    return TFrac(newNum, newDen);
}

TFrac TFrac::Reverse() const {
    if (numerator == 0) {

        throw std::runtime_error("Cannot reverse zero fraction (division by zero).");
        return TFrac(0, 1);
    }
    return TFrac(denominator, numerator);
}

TFrac TFrac::Minus() const {
    return TFrac(-numerator, denominator);
}

// Сравнение
bool TFrac::Equal(const TFrac& other) const {
    return numerator == other.numerator && denominator == other.denominator;
}

bool TFrac::operator==(const TFrac& other) const {
    return Equal(other);
}



bool TFrac::operator>(const TFrac& other) const {
    if (denominator == 0 || other.denominator == 0) return false;
    return numerator * other.denominator > other.numerator * denominator;
    // return (static_cast<double>(numerator) / denominator) > (static_cast<double>(other.numerator) / other.denominator);
}

bool TFrac::operator<(const TFrac& other) const {
    if (denominator == 0 || other.denominator == 0) return false;
    return numerator * other.denominator < other.numerator * denominator;
    // return (static_cast<double>(numerator) / denominator) < (static_cast<double>(other.numerator) / other.denominator);
}


// Преобразование в строку
QString TFrac::ToString() const {
    return QString::number(numerator) + "/" + QString::number(denominator);
}
