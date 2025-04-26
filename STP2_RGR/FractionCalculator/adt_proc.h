#ifndef ADT_PROC_H
#define ADT_PROC_H

#include <stdexcept>
#include <QDebug>

template <typename T>
class ADT_Proc {
public:
    enum OperationType { None = 0, Add = 1, Sub = 2, Mul = 3, Div = 4 };
    enum FunctionType { ReverseFunc = 0, SquareFunc = 1 };

private:
    T left_result_operand;
    T right_operand;
    OperationType operation;

    OperationType lastOperationForEquals;
    T lastRightOperandForEquals;

public:
    ADT_Proc() :
        left_result_operand(),
        right_operand(),
        operation(OperationType::None),
        lastOperationForEquals(OperationType::None),
        lastRightOperandForEquals()
    {}


    const T& GetLeftResultOperand() const { return left_result_operand; }
    void SetLeftResultOperand(const T& value) { left_result_operand = value; }

    const T& GetRightOperand() const { return right_operand; }
    void SetRightOperand(const T& value) { right_operand = value; }

    OperationType GetOperation() const { return operation; }
    void SetOperation(OperationType op) { operation = op; }
    void SetOperation(int op) {
        if (op >= static_cast<int>(OperationType::None) && op <= static_cast<int>(OperationType::Div)) {
            operation = static_cast<OperationType>(op);
        } else {
            operation = OperationType::None;
        }
    }

    void SetLastOperationForEquals(OperationType op) {
        lastOperationForEquals = op;
        qDebug() << "[Proc] SetLastOperationForEquals to:" << static_cast<int>(op);
    }
    void SetLastRightOperandForEquals(const T& operand) {
        lastRightOperandForEquals = operand;
        qDebug() << "[Proc] SetLastRightOperandForEquals to:" << operand.ToString();
    }
    OperationType GetLastOperationForEquals() const {
        return lastOperationForEquals;
    }
    const T& GetLastRightOperandForEquals() const {
        return lastRightOperandForEquals;
    }
    void ClearLastOperationForEquals() {
        lastOperationForEquals = OperationType::None;
        lastRightOperandForEquals = T();
        qDebug() << "[Proc] Cleared last operation for equals.";
    }

    void ResetProc() {
        left_result_operand = T();
        right_operand = T();
        operation = OperationType::None;
        ClearLastOperationForEquals();
        qDebug() << "[Proc] ResetProc called.";
    }

    void DoOperation() {
        qDebug() << "[Proc::DoOp] Executing Op:" << static_cast<int>(operation)
                 << " Left:" << left_result_operand.ToString()
                 << " Right:" << right_operand.ToString();

        if (operation == OperationType::None) {
            left_result_operand = right_operand;
            qDebug() << "[Proc::DoOp] Operation was None. Left set to Right.";
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
                default:
                    qWarning() << "[Proc::DoOp] Unknown operation type:" << static_cast<int>(operation);
                    break;
            }
            qDebug() << "[Proc::DoOp] Result Left:" << left_result_operand.ToString();

        } catch (const std::exception& e) {
            qCritical() << "[Proc::DoOp] Exception caught:" << e.what();
            ResetProc();
            throw;
        }
   }

    void DoFunction(int funcCode) {
        FunctionType function = static_cast<FunctionType>(funcCode);
        qDebug() << "[Proc::DoFunc] Executing Func:" << funcCode
                 << " on RightOp:" << right_operand.ToString();

         try {
             switch (function) {
                 case FunctionType::ReverseFunc:
                     right_operand = right_operand.Reverse();
                     break;
                 case FunctionType::SquareFunc:
                     right_operand = right_operand.Square();
                     break;
                 default:
                     qWarning() << "[Proc::DoFunc] Unknown function code:" << funcCode;
                     break;
             }
             qDebug() << "[Proc::DoFunc] Result RightOp:" << right_operand.ToString();

         } catch (const std::exception& e) {
             qCritical() << "[Proc::DoFunc] Exception caught:" << e.what();
             ResetProc();
             throw;
         }
    }
};

#endif // ADT_PROC_H
