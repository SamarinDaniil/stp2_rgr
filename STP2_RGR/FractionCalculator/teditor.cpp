#include "teditor.h"
#include <QString>

TEditor::TEditor() : fractionString("0") {}

QString TEditor::GetFractionString() const {
    return fractionString;
}

void TEditor::SetFractionString(const QString& s) {
    // Можно добавить валидацию, но пока просто присваиваем
    fractionString = s;
    // При установке строки извне (MR), полезно нормализовать ее через TFrac
     TFrac temp(fractionString);
     fractionString = temp.ToString(); // Сохраняем в каноническом виде
}


void TEditor::SetEditorFromFrac(const TFrac& frac) {
    fractionString = frac.ToString();
}

bool TEditor::IsZero() const {
    return fractionString == "0" || fractionString == "-0" || fractionString == "0/1" || fractionString == "-0/1";
}

QString TEditor::ToggleMinus() {
    if (fractionString.startsWith('-')) {
        fractionString.remove(0, 1);
    } else {
        if (fractionString != "0" && fractionString != "0/1") {
            fractionString.prepend('-');
        } else if (fractionString == "0"){
            fractionString = "-0";
        } else if (fractionString == "-0") {
             fractionString = "0";
        }
    }
    if (fractionString.contains(Separator)) {
        TFrac temp(fractionString);
        fractionString = temp.ToString();
    }
    return fractionString;
}

QString TEditor::AddNumber(int number) {
    if (number < 0 || number > 9) return fractionString;

    QString numStr = QString::number(number);

    int separatorPos = fractionString.indexOf(Separator);
    if (separatorPos == -1) {
        QString currentNum = fractionString;
        if (currentNum.startsWith('-')) currentNum.remove(0,1);
        if (currentNum.length() >= max_numerator_length && currentNum != "0") return fractionString;

        if (fractionString == "0") {
            fractionString = numStr;
        } else if (fractionString == "-0") {
             fractionString = "-" + numStr;
        } else {
            fractionString += numStr;
        }
    } else {
        if (fractionString.length() >= max_denominator_length) return fractionString;
         if (fractionString.endsWith(Separator) && numStr == "0") {
             fractionString += numStr;
         }
         else if (!fractionString.endsWith(Separator)) {
             fractionString += numStr;
         } else if (numStr != "0") {
              fractionString += numStr;
         }
    }

    return fractionString;
}

QString TEditor::AddZero() {
    int separatorPos = fractionString.indexOf(Separator);
    if (separatorPos == -1) {
        QString currentNum = fractionString;
        if (currentNum.startsWith('-')) currentNum.remove(0,1);
        if (currentNum.length() >= max_numerator_length && currentNum != "0") return fractionString;

        if (fractionString != "0" && fractionString != "-0") {
             fractionString += "0";
        }
    } else {
        if (fractionString.length() >= max_denominator_length) return fractionString;
        if (!fractionString.endsWith(Separator)) {
            fractionString += "0";
        } else {
             fractionString += "0";
        }
    }
    return fractionString;
}


QString TEditor::RemoveSymbol() {
    if (fractionString.length() > 1) {
        if (fractionString.length() == 2 && fractionString.startsWith('-')) {
            fractionString = "-0";
        } else {
            fractionString.chop(1);
            if (fractionString == "-") {
                fractionString = "-0";
            }

             if (fractionString.endsWith(Separator)) {
                 fractionString.chop(1);
             }
        }
    } else if (fractionString.length() == 1 && fractionString != "0") {

        fractionString = "0";
    }
    return fractionString;
}

QString TEditor::Clear() {
    fractionString = "0";
    return fractionString;
}

QString TEditor::AddSeparator() {

    if (!fractionString.contains(Separator) && fractionString != "-" && fractionString != "") {

         if (fractionString != "0" && fractionString != "-0") {
              fractionString += Separator;
         }
    }
    return fractionString;
}

TFrac TEditor::ToFrac() const {
    return TFrac(fractionString);
}

QString TEditor::ToString() const {
    return GetFractionString();
}

QString TEditor::Edit(int command) {
    switch (command) {
        case 0: return AddZero();
        case 1: return AddNumber(1);
        case 2: return AddNumber(2);
        case 3: return AddNumber(3);
        case 4: return AddNumber(4);
        case 5: return AddNumber(5);
        case 6: return AddNumber(6);
        case 7: return AddNumber(7);
        case 8: return AddNumber(8);
        case 9: return AddNumber(9);
        case 10: return ToggleMinus(); // +/- в C# было 10
        case 11: return AddSeparator(); // '/' (дробь) в C# было 11
        case 12: return RemoveSymbol(); // Backspace (BS) в C# было 12
        case 13: return Clear();      // Clear Entry (CE) в C# было 13
        default: break; // Неизвестная команда
    }
    return fractionString;
}
