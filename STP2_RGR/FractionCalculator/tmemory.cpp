#ifndef ADT_PROC_H
#define ADT_PROC_H

#include <stdexcept> // Для исключений

template <typename T> // T должен иметь методы Add, Sub, Mul, Div, Reverse, Square
class ADT_Proc {
public:
    enum OperationType { None = 0, Add = 1, Sub = 2, Mul = 3, Div = 4 };
    enum FunctionType { ReverseFunc = 0, SquareFunc = 1 };

private:
    T left_result_operand; // Левый операнд и результат
    T right_operand;       // Правый операнд (вводимый)
    OperationType operation; // Текущая операция

public:
    ADT_Proc() : left_result_operand(), right_operand(), operation(OperationType::None) {}

    // Геттеры и Сеттеры
    const T& GetLeftResultOperand() const { return left_result_operand; }
    void SetLeftResultOperand(const T& value) { left_result_operand = value; }

    const T& GetRightOperand() const { return right_operand; }
    void SetRightOperand(const T& value) { right_operand = value; }

    OperationType GetOperation() const { return operation; }
    void SetOperation(OperationType op) { operation = op; }
    void SetOperation(int op) { // Для совместимости с командами из C#
        if (op >= OperationType::None && op <= OperationType::Div) {
            operation = static_cast<OperationType>(op);
        } else {
            operation = OperationType::None;
        }
    }


    // Сброс процессора
    void ResetProc() {
        left_result_operand = T();
        right_operand = T();
        operation = OperationType::None;
    }

    // Выполнение бинарной операции
    void DoOperation() {
        // Если операция не задана, результат = правый операнд (как в C# Calculate при старте)
        if (operation == OperationType::None) {
            left_result_operand = right_operand;
            return;
        }

        try {
            switch (operation) {
                case OperationType::Add:
                    left_result_operand = left_result_operand.Add(right_operand);
                    break;
                case OperationType::Sub:
                    left_result_operand = left_result_operand.Sub(right_operand);
                    break;
                case OperationType::Mul:
                    left_result_operand = left_result_operand.Mul(right_operand);
                    break;
                case OperationType::Div:
                    left_result_operand = left_result_operand.Div(right_operand);
                    break;
                case OperationType::None:
                default:
                    // Ничего не делаем или присваиваем правый операнд?
                    // В C# логика Calculate обрабатывала это до вызова DoOperation
                    // Оставим как есть: если сюда дошли с None, ничего не меняется
                    break;
            }
            // После успешной операции можно сбросить тип операции,
            // если `=` не является продолжением цепочки
            // operation = OperationType::None; // Пока не сбрасываем здесь
        } catch (const std::exception& e) {
            // Перехватываем исключения (например, деление на ноль из TFrac::Div)
            // и пробрасываем их дальше или обрабатываем
             ResetProc(); // Сброс состояния процессора при ошибке
            throw; // Пробросить исключение выше (в ADT_Control)
        }
         // Сброс правого операнда после операции? Зависит от логики контроллера.
         // Пока не сбрасываем здесь.
    }

    // Выполнение унарной функции над правым операндом
    void DoFunction(int func) {
         FunctionType function = static_cast<FunctionType>(func);
         try {
             switch (function) {
                 case FunctionType::ReverseFunc: // 1/x
                     right_operand = right_operand.Reverse();
                     break;
                 case FunctionType::SquareFunc: // sqr
                     right_operand = right_operand.Square();
                     break;
                 default:
                     break; // Неизвестная функция
             }
         } catch (const std::exception& e) {
             ResetProc(); // Сброс при ошибке
             throw; // Пробросить выше
         }
    }
};

#endif // ADT_PROC_H
