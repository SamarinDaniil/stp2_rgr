#ifndef TEDITOR_H
#define TEDITOR_H

#include "tfrac.h"
#include <QString>

class TEditor {
private:
    QString fractionString;

    const QString Separator = "/";
    const QString ZeroFractionNumStr = "0";
    const int max_numerator_length = 14;
    const int max_denominator_length = 22;

public:
    TEditor();

    QString GetFractionString() const;
    void SetFractionString(const QString& s);
    void SetEditorFromFrac(const TFrac& frac);

    bool IsZero() const;
    QString ToggleMinus();
    QString AddNumber(int number); // Принимаем int 0-9
    QString AddZero();
    QString RemoveSymbol(); // Backspace
    QString Clear(); // CE (Clear Entry)
    QString AddSeparator(); // Добавить '/'

    // Основной метод редактирования по команде
    QString Edit(int command);

    // Метод для получения TFrac из текущей строки редактора
    TFrac ToFrac() const;

    QString ToString() const;
};

#endif // TEDITOR_H
