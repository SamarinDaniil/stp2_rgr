#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "adt_control.h"
#include "history.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Переопределяем обработчик событий клавиатуры
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // Слоты для кнопок
    void digitClicked();       // Для кнопок 0-9
    void operatorClicked();    // Для +, -, *, /
    void functionClicked();    // Для sqr, 1/x
    void memoryClicked();      // Для MC, MR, MS, M+
    void equalsClicked();      // Для =
    void signClicked();        // Для +/-
    void fractionClicked();    // Для ввода /
    void backspaceClicked();   // Для BS (←)
    void clearEntryClicked();  // Для CE
    void clearAllClicked();    // Для C
    void aboutClicked();       // Для меню "О программе"
    void showHistoryTriggered(); // Слот для actionLookHistory
    void clearHistoryTriggered(); // Слот для actionClearHistory

private:
    Ui::MainWindow *ui;
    ADT_Control calculatorController;

    void updateDisplay(const QString& text); // Обновление дисплея
    void updateMemoryIndicator(bool state); // Обновление индикатора "M"

    // Вспомогательные функции для преобразования кнопок в команды
    int buttonToEditorCommand(QObject* sender);
    int buttonToOperationCommand(QObject* sender);
    int buttonToFunctionCommand(QObject* sender);
    int buttonToMemoryCommand(QObject* sender);
};
#endif // MAINWINDOW_H
