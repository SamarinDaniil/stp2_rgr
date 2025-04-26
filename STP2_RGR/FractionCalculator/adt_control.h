#ifndef ADT_CONTROL_H
#define ADT_CONTROL_H

#include "teditor.h"
#include "tfrac.h"
#include "history.h"
#include "tmemory.h"
#include "adt_proc.h"

#include <QString>
#include <stdexcept>
#include <utility>
#include <QDebug>
#include <QtGlobal>


using FracEditor = TEditor;
using FracMemory = TMemory<TFrac>;
using FracProc = ADT_Proc<TFrac>;

class ADT_Control {
public:

    enum class State { Start, Editing, FuncDone, ValDone, ExpDone, OpDone, OpChange, Error };

    ADT_Control() :
        currentState(State::Start),
        editor(),
        proc(),
        memory(),
        history()
    {}


    State GetCurrentState() const { return currentState; }
    const FracMemory& GetMemory() const { return memory; }
    const History& GetHistory() const { return history; }
    const FracProc& GetProc() const { return proc; }
    QString GetEditorString() const { return editor.ToString(); }

    QString Reset();
    QString ExecCommandEditor(int command);
    QString ExecOperation(int operation);
    QString ExecFunction(int function);
    QString Calculate();
    std::pair<QString, bool> ExecCommandMemory(int command, const QString& currentDisplayValue);
    void ClearHistory() { history.clear(); }


    State currentState;
    FracEditor editor;
    FracProc proc;
    FracMemory memory;
    History history;
private:
    const QString ERROR_MSG = "ERROR";

};


inline QString ADT_Control::Reset() {
    qDebug() << "[Ctrl::Reset] Resetting calculator state.";
    editor.Clear();
    proc.ResetProc();
    memory.Clear();

    currentState = State::Start;
    return editor.ToString();
}

inline QString ADT_Control::ExecCommandEditor(int command) {
    qDebug() << "[Ctrl::EditCmd] Cmd:" << command << " State Before:" << static_cast<int>(currentState) << " Editor Before:" << editor.ToString();


    if (currentState != State::Editing) {
        proc.ClearLastOperationForEquals();
    }


    if (currentState == State::ExpDone || currentState == State::Error) {
        proc.ResetProc();
        editor.Clear();
        currentState = State::Start;
        qDebug() << "[Ctrl::EditCmd] State was ExpDone/Error, reset to Start.";
    }

    else if (currentState == State::OpDone || currentState == State::OpChange || currentState == State::FuncDone) {
        editor.Clear();
        qDebug() << "[Ctrl::EditCmd] State was Op/FuncDone, cleared editor.";
    }


    currentState = State::Editing;


    QString currentEditorString = editor.Edit(command);
    qDebug() << "[Ctrl::EditCmd] String after editor.Edit:" << currentEditorString;


    try {

        TFrac tempFrac = TFrac(currentEditorString);
        proc.SetRightOperand(tempFrac);
        qDebug() << "[Ctrl::EditCmd] Proc Right Operand updated to:" << tempFrac.ToString();
    } catch (const std::exception& e) {


        qWarning() << "[Ctrl::EditCmd] Parsing error for string" << currentEditorString << ":" << e.what() << "- Proc Right Operand not updated.";
    } catch(...) {

        qWarning() << "[Ctrl::EditCmd] Unknown parsing error for string" << currentEditorString << "- Proc Right Operand not updated.";
    }




    qDebug() << "[Ctrl::EditCmd] Returning editor string:" << currentEditorString;
    return currentEditorString;
}



inline QString ADT_Control::ExecOperation(int operation) {
    if (currentState == State::Error) {
         qWarning() << "[Ctrl::ExecOp] Ignoring operation due to Error state.";
         return ERROR_MSG;
    }

    FracProc::OperationType opType = static_cast<FracProc::OperationType>(operation);
    if (opType == FracProc::OperationType::None) {
        qWarning() << "[Ctrl::ExecOp] Ignoring invalid operation code:" << operation;
        return editor.ToString();
    }

    qDebug() << "[Ctrl::ExecOp] Op:" << operation << " State Before:" << static_cast<int>(currentState)
             << " LeftOp Before:" << proc.GetLeftResultOperand().ToString()
             << " RightOp Before:" << proc.GetRightOperand().ToString();
    proc.ClearLastOperationForEquals();

    QString resultStr = editor.ToString();

    try {

        switch (currentState) {
            case State::Start:
                proc.SetLeftResultOperand(proc.GetRightOperand());
                proc.SetOperation(opType);
                currentState = State::OpDone;
                resultStr = proc.GetLeftResultOperand().ToString();
                break;

            case State::Editing:

                if (proc.GetOperation() != FracProc::OperationType::None) {
                    proc.DoOperation();
                } else {

                    proc.SetLeftResultOperand(proc.GetRightOperand());
                }
                proc.SetOperation(opType);
                currentState = State::OpDone;
                resultStr = proc.GetLeftResultOperand().ToString();
                break;

            case State::FuncDone:
                proc.SetLeftResultOperand(proc.GetRightOperand());
                proc.SetOperation(opType);
                currentState = State::OpDone;
                resultStr = proc.GetLeftResultOperand().ToString();
                break;

            case State::OpDone:
                proc.SetOperation(opType);
                currentState = State::OpChange;
                resultStr = proc.GetLeftResultOperand().ToString();
                break;

            case State::OpChange:
                proc.SetOperation(opType);

                resultStr = proc.GetLeftResultOperand().ToString();
                break;


            case State::ExpDone:
                qDebug() << "[Ctrl::ExecOp] Handling operation after ExpDone state.";
                {
                    TFrac rightOpForRepeat = proc.GetLeftResultOperand();
                    proc.SetRightOperand(rightOpForRepeat);
                    proc.SetOperation(opType);
                    qDebug() << "[Ctrl::ExecOp] Set RightOp to LeftOp:" << rightOpForRepeat.ToString() << "and Op to:" << static_cast<int>(opType);


                    proc.DoOperation();
                    qDebug() << "[Ctrl::ExecOp] Performed operation result OP result. New LeftOp:" << proc.GetLeftResultOperand().ToString();


                    currentState = State::OpDone;
                    resultStr = proc.GetLeftResultOperand().ToString();


                    proc.SetLastOperationForEquals(opType);
                    proc.SetLastRightOperandForEquals(rightOpForRepeat);
                    qDebug() << "[Ctrl::ExecOp] Stored for repeat after ExpDone op: Op=" << static_cast<int>(opType) << " RightOp=" << rightOpForRepeat.ToString();
                }
                break;


            case State::ValDone:
                 break;
            case State::Error:
                 return ERROR_MSG;
        }


         try {

             TFrac resultFrac(resultStr);
             editor.SetEditorFromFrac(resultFrac);
             qDebug() << "[Ctrl::ExecOp] Editor updated after operation to:" << editor.ToString();
         } catch(const std::exception& e) {
             qWarning() << "[Ctrl::ExecOp] Failed to parse result string for editor update:" << resultStr << "-" << e.what();


         } catch(...) {
             qWarning() << "[Ctrl::ExecOp] Unknown error parsing result string for editor update:" << resultStr;
         }

    } catch (const std::exception& e) {
        qCritical() << "[Ctrl::ExecOp] Exception during operation execution:" << e.what();
        Reset();
        currentState = State::Error;
        editor.SetFractionString(ERROR_MSG);
        return ERROR_MSG;
    }

     qDebug() << "[Ctrl::ExecOp] State After:" << static_cast<int>(currentState)
              << " LeftOp After:" << proc.GetLeftResultOperand().ToString()
              << " Returning Display Str:" << resultStr;

    return resultStr;
}



inline QString ADT_Control::ExecFunction(int function) {

    if (currentState == State::Error) {
        qWarning() << "[Ctrl::ExecFunc] Ignoring function due to Error state.";
        return ERROR_MSG;
    }

    qDebug() << "[Ctrl::ExecFunc] Func:" << function << " State Before:" << static_cast<int>(currentState)
             << " RightOp Before:" << proc.GetRightOperand().ToString();


    proc.ClearLastOperationForEquals();

    try {

        if (currentState == State::ExpDone) {
            proc.SetRightOperand(proc.GetLeftResultOperand());
            qDebug() << "[Ctrl::ExecFunc] Function applied to ExpDone result.";
        }


        proc.DoFunction(function);

        currentState = State::FuncDone;

        editor.SetEditorFromFrac(proc.GetRightOperand());

        qDebug() << "[Ctrl::ExecFunc] State After:" << static_cast<int>(currentState)
                 << " RightOp After:" << proc.GetRightOperand().ToString()
                 << " Editor After:" << editor.ToString();

        return editor.ToString();

    } catch (const std::exception& e) {
        qCritical() << "[Ctrl::ExecFunc] Exception during function execution:" << e.what();
        Reset();
        currentState = State::Error;
        editor.SetFractionString(ERROR_MSG);
        return ERROR_MSG;
    }
}



inline QString ADT_Control::Calculate() {

    if (currentState == State::Error) {
        qWarning() << "[Ctrl::Calculate] Ignoring calculation due to Error state.";
        return ERROR_MSG;
    }

    qDebug() << "[Ctrl::Calculate] State Before:" << static_cast<int>(currentState)
             << " Left:" << proc.GetLeftResultOperand().ToString()
             << " Right:" << proc.GetRightOperand().ToString()
             << " Op:" << static_cast<int>(proc.GetOperation())
             << " LastOpEq:" << static_cast<int>(proc.GetLastOperationForEquals())
             << " LastRightEq:" << proc.GetLastRightOperandForEquals().ToString();

    QString resultDisplayString;
    QString historyEntryString;
    bool recordHistory = true;
    bool operationJustPerformed = false;

    try {
        TFrac leftBeforeCalc = proc.GetLeftResultOperand();
        TFrac rightOperandForHistory = proc.GetRightOperand();
        FracProc::OperationType opForHistory = proc.GetOperation();


        if (currentState == State::ExpDone) {
            qDebug() << "[Ctrl::Calculate] Handling REPEATED equals.";
            FracProc::OperationType lastOp = proc.GetLastOperationForEquals();

            if (lastOp != FracProc::OperationType::None) {
                TFrac lastRight = proc.GetLastRightOperandForEquals();
                qDebug() << "[Ctrl::Calculate] Repeating Op:" << static_cast<int>(lastOp) << " with Right:" << lastRight.ToString();


                proc.SetRightOperand(lastRight);
                proc.SetOperation(lastOp);


                leftBeforeCalc = proc.GetLeftResultOperand();
                rightOperandForHistory = lastRight;
                opForHistory = lastOp;


                proc.DoOperation();
                operationJustPerformed = true;
                recordHistory = true;

            } else {

                qDebug() << "[Ctrl::Calculate] No last operation stored to repeat.";
                recordHistory = false;
            }
        }

        else if (currentState == State::Start || currentState == State::Editing ||
                 currentState == State::FuncDone || currentState == State::OpDone ||
                 currentState == State::OpChange)
        {
             qDebug() << "[Ctrl::Calculate] Handling FIRST equals (or after op/func).";
             opForHistory = proc.GetOperation();
             rightOperandForHistory = proc.GetRightOperand();


             if (opForHistory != FracProc::OperationType::None) {
                 qDebug() << "[Ctrl::Calculate] Pending operation found:" << static_cast<int>(opForHistory);

                 proc.SetLastOperationForEquals(opForHistory);
                 proc.SetLastRightOperandForEquals(rightOperandForHistory);
                 qDebug() << "[Ctrl::Calculate] Stored for repeat: Op=" << static_cast<int>(opForHistory) << " RightOp=" << rightOperandForHistory.ToString();


                 leftBeforeCalc = proc.GetLeftResultOperand();

                 proc.DoOperation();
                 operationJustPerformed = true;
                 recordHistory = true;
             } else {

                 qDebug() << "[Ctrl::Calculate] No pending operation.";
                 proc.SetLeftResultOperand(proc.GetRightOperand());
                 proc.ClearLastOperationForEquals();
                 recordHistory = false;
             }
        }

        else {
             qWarning() << "[Ctrl::Calculate] Unexpected state (" << static_cast<int>(currentState) << "), clearing last op info.";
             proc.ClearLastOperationForEquals();
             recordHistory = false;
        }


        TFrac finalResult = proc.GetLeftResultOperand();
        currentState = State::ExpDone;
        editor.SetEditorFromFrac(finalResult);
        resultDisplayString = editor.ToString();

        qDebug() << "[Ctrl::Calculate] State After:" << static_cast<int>(currentState)
                 << " Final Result:" << finalResult.ToString()
                 << " Editor Updated To:" << resultDisplayString;


        if (recordHistory && operationJustPerformed) {
            QString opSymbol = "?";
            if (opForHistory != FracProc::OperationType::None) {
                 switch (opForHistory) {
                     case FracProc::OperationType::Add: opSymbol = " + "; break;
                     case FracProc::OperationType::Sub: opSymbol = " - "; break;
                     case FracProc::OperationType::Mul: opSymbol = " * "; break;
                     case FracProc::OperationType::Div: opSymbol = " / "; break;
                     default: break;
                 }

                 historyEntryString = leftBeforeCalc.ToString() + opSymbol + rightOperandForHistory.ToString() + " = " + finalResult.ToString();
                 history.addRecord(historyEntryString);
                 qDebug() << "[Ctrl::Calculate] Added to history:" << historyEntryString;
            } else {
                 qDebug() << "[Ctrl::Calculate] No operation performed, not adding to history.";
            }
        } else {
             qDebug() << "[Ctrl::Calculate] Not adding to history (recordHistory=" << recordHistory << ", operationJustPerformed=" << operationJustPerformed << ")";
        }

    } catch (const std::exception& e) {
        qCritical() << "[Ctrl::Calculate] Exception during calculation:" << e.what();
        proc.ClearLastOperationForEquals();
        Reset();
        currentState = State::Error;
        editor.SetFractionString(ERROR_MSG);
        return ERROR_MSG;
    }


    return resultDisplayString;
}



inline std::pair<QString, bool> ADT_Control::ExecCommandMemory(int command, const QString& currentDisplayValue) {

    Q_UNUSED(currentDisplayValue);

    qDebug() << "[Ctrl::ExecMem] Cmd:" << command << " State Before:" << static_cast<int>(currentState)
             << " Input Display Value:" << currentDisplayValue;


    proc.ClearLastOperationForEquals();


    if (currentState == State::Error && command != 2 ) {
        qWarning() << "[Ctrl::ExecMem] Ignoring command due to Error state.";
        return {"", memory.FState()};
    }

    TFrac fracForMemory;


    if (command == 0 || command == 1) {
        try {

            TFrac parsedValue(currentDisplayValue);
            fracForMemory = parsedValue;
            qDebug() << "[Ctrl::ExecMem] Parsed input for MS/M+:" << fracForMemory.ToString();
        } catch (const std::exception &e) {
            qWarning() << "[Ctrl::ExecMem] Error parsing display value for MS/M+:" << currentDisplayValue << "-" << e.what();

            return {"", memory.FState()};
        } catch(...) {
             qWarning() << "[Ctrl::ExecMem] Unknown error parsing display value for MS/M+:" << currentDisplayValue;
             return {"", memory.FState()};
        }
    }


    QString displayResult = "";
    bool memoryState = memory.FState();

    try {

        if (command == 3) {
            if (memory.FState()) {
                TFrac recalledValue = memory.Recall();
                editor.SetEditorFromFrac(recalledValue);
                proc.SetRightOperand(recalledValue);
                displayResult = editor.ToString();
                currentState = State::Editing;
                qDebug() << "[Ctrl::ExecMem] MR executed. Recalled:" << displayResult << " New State:" << static_cast<int>(currentState);
            } else {

                 qDebug() << "[Ctrl::ExecMem] MR ignored, memory empty.";

            }

        } else if (command == 0 || command == 1 || command == 2) {


            auto memResult = memory.Edit(command, fracForMemory);

            memoryState = memResult.second;
            qDebug() << "[Ctrl::ExecMem] Memory Edit executed. Command:" << command
                     << " Result Value:" << memResult.first.ToString()
                     << " Result State:" << memoryState;
        } else {

             qWarning() << "[Ctrl::ExecMem] Unknown memory command:" << command;
        }
    } catch (const std::exception& e) {

        qCritical() << "[Ctrl::ExecMem] Exception during memory operation:" << e.what();



        return {"", memory.FState()};
    }


    qDebug() << "[Ctrl::ExecMem] Finished. Returning Display:" << displayResult << " Mem State:" << memoryState;
    return {displayResult, memoryState};
}


#endif // ADT_CONTROL_H
