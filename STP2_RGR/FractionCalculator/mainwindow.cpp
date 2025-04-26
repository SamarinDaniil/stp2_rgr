#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QAction>
#include <QPlainTextEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateDisplay(calculatorController.Reset());
    updateMemoryIndicator(false);

    // Цифры (0-9)
    connect(ui->pushButton_0, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_1, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::digitClicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::digitClicked);

    // Операторы (+, -, *, /)
    connect(ui->pushButton_add, &QPushButton::clicked, this, &MainWindow::operatorClicked);
    connect(ui->pushButton_subtract, &QPushButton::clicked, this, &MainWindow::operatorClicked);
    connect(ui->pushButton_multiply, &QPushButton::clicked, this, &MainWindow::operatorClicked);
    connect(ui->pushButton_divide, &QPushButton::clicked, this, &MainWindow::operatorClicked);

    // Функции (sqr, 1/x)
    connect(ui->pushButton_sqrt, &QPushButton::clicked, this, &MainWindow::functionClicked);
    connect(ui->pushButton_reciprocal, &QPushButton::clicked, this, &MainWindow::functionClicked);

    // Память (MC, MR, MS, M+)
    connect(ui->pushButton_MC, &QPushButton::clicked, this, &MainWindow::memoryClicked);
    connect(ui->pushButton_MR, &QPushButton::clicked, this, &MainWindow::memoryClicked);
    connect(ui->pushButton_MS, &QPushButton::clicked, this, &MainWindow::memoryClicked);
    connect(ui->pushButton_Mplus, &QPushButton::clicked, this, &MainWindow::memoryClicked);

    // Управление и спецсимволы
    connect(ui->pushButton_equals, &QPushButton::clicked, this, &MainWindow::equalsClicked);
    connect(ui->pushButton_sign, &QPushButton::clicked, this, &MainWindow::signClicked);
    connect(ui->pushButton_fraction, &QPushButton::clicked, this, &MainWindow::fractionClicked);
    connect(ui->pushButton_BS, &QPushButton::clicked, this, &MainWindow::backspaceClicked);
    connect(ui->pushButton_CE, &QPushButton::clicked, this, &MainWindow::clearEntryClicked);
    connect(ui->pushButton_C, &QPushButton::clicked, this, &MainWindow::clearAllClicked);

    // Меню
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutClicked);
    connect(ui->actionLookHistory, &QAction::triggered, this, &MainWindow::showHistoryTriggered);
    connect(ui->actionClearHistory, &QAction::triggered, this, &MainWindow::clearHistoryTriggered);

    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateDisplay(const QString& text) {
    if (text == "ERROR") {
         QMessageBox::warning(this, "Ошибка", "Произошла ошибка вычисления!");
         //ui->displayLineEdit->setText("ERROR");
    } else {
        ui->displayLineEdit->setText(text);
    }
}

void MainWindow::updateMemoryIndicator(bool state) {
    ui->memoryIndicatorLabel->setText(state ? "M" : "");
}

int MainWindow::buttonToEditorCommand(QObject* sender) {
    QPushButton* button = qobject_cast<QPushButton*>(sender);
    if (!button) return -1;

    QString text = button->text();
    bool ok;
    int digit = text.toInt(&ok);
    if (ok) return digit;

    if (button == ui->pushButton_sign) return 10;
    if (button == ui->pushButton_fraction) return 11;
    if (button == ui->pushButton_BS) return 12;
    if (button == ui->pushButton_CE) return 13;
    return -1;
}

int MainWindow::buttonToOperationCommand(QObject* sender) {
    if (sender == ui->pushButton_add) return 1;
    if (sender == ui->pushButton_subtract) return 2;
    if (sender == ui->pushButton_multiply) return 3;
    if (sender == ui->pushButton_divide) return 4;
    return 0;
}

int MainWindow::buttonToFunctionCommand(QObject* sender) {
    if (sender == ui->pushButton_reciprocal) return 0;
    if (sender == ui->pushButton_sqrt) return 1;
    return -1;
}

int MainWindow::buttonToMemoryCommand(QObject* sender) {
    if (sender == ui->pushButton_MS) return 0;
    if (sender == ui->pushButton_Mplus) return 1;
    if (sender == ui->pushButton_MC) return 2;
    if (sender == ui->pushButton_MR) return 3;
    return -1;
}

void MainWindow::digitClicked() {
    int command = buttonToEditorCommand(sender());
    if (command >= 0 && command <= 9) {
        updateDisplay(calculatorController.ExecCommandEditor(command));
    }
}

void MainWindow::operatorClicked() {
    int command = buttonToOperationCommand(sender());
    if (command > 0) {
         updateDisplay(calculatorController.ExecOperation(command));
    }
}

void MainWindow::functionClicked() {
    int command = buttonToFunctionCommand(sender());
     if (command >= 0) {
         updateDisplay(calculatorController.ExecFunction(command));
     }
}

void MainWindow::memoryClicked() {
     int command = buttonToMemoryCommand(sender());
     if (command >= 0) {
         std::pair<QString, bool> result = calculatorController.ExecCommandMemory(command, ui->displayLineEdit->text());
         if (command == 3 && !result.first.isEmpty()) {
              updateDisplay(result.first);
         }
         updateMemoryIndicator(result.second);
     }
}

void MainWindow::equalsClicked() {
    updateDisplay(calculatorController.Calculate());
}

void MainWindow::signClicked() {
     updateDisplay(calculatorController.ExecCommandEditor(10));
}

void MainWindow::fractionClicked() {
     updateDisplay(calculatorController.ExecCommandEditor(11));
}

void MainWindow::backspaceClicked() {
     updateDisplay(calculatorController.ExecCommandEditor(12));
}

void MainWindow::clearEntryClicked() {
     updateDisplay(calculatorController.ExecCommandEditor(13));
}

void MainWindow::clearAllClicked() {
     updateDisplay(calculatorController.Reset());
     updateMemoryIndicator(false);
}

void MainWindow::aboutClicked() {
     QMessageBox::about(this, "О программе",
                        "Калькулятор простых дробей (C++/Qt)\n"
                        "Версия: 1.0.0\n\n"
                        "Выполнил студент Самарин Д.А. группа ИП-115.\n"
                        "(c) 2025");
}

void MainWindow::showHistoryTriggered() {
    const History& history = calculatorController.GetHistory();
    const QList<CalcRecord>& records = history.getRecords();


    QString historyText;
    if (records.isEmpty()) {
        historyText = "(История пуста)";
    } else {
        for (int i = records.count() - 1; i >= 0; --i) {
            historyText += records.at(i).toString() + "\n";
        }
    }

    QPlainTextEdit *historyView = new QPlainTextEdit();
    historyView->setPlainText(historyText);
    historyView->setReadOnly(true);
    historyView->setMinimumSize(400, 300);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    QDialog *historyDialog = new QDialog(this);
    historyDialog->setWindowTitle("История вычислений");

    QVBoxLayout *layout = new QVBoxLayout(historyDialog);
    layout->addWidget(historyView);
    layout->addWidget(buttonBox);
    historyDialog->setLayout(layout);

    connect(buttonBox, &QDialogButtonBox::accepted, historyDialog, &QDialog::accept);

    historyDialog->exec();


    historyDialog->deleteLater();
}

void MainWindow::clearHistoryTriggered() {
    // Подтверждение очистки
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Очистка истории", "Вы уверены, что хотите очистить историю вычислений?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        calculatorController.ClearHistory();
        qDebug() << "История очищена.";

        QMessageBox::information(this, "История", "История вычислений очищена.");
    }
    showHistoryTriggered();
}

// --- Обработка клавиатуры ---
void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    QString text = event->text();

    qDebug() << "Key pressed:" << key << "Text:" << text; // Отладка

    // Цифры (0-9)
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        int digit = text.toInt();
        updateDisplay(calculatorController.ExecCommandEditor(digit));
        return;
    }

    if (text == "+") {
        updateDisplay(calculatorController.ExecOperation(1));
        return;
    }
    if (text == "-") {
        updateDisplay(calculatorController.ExecOperation(2));
        return;
    }
    if (text == "*") {
        updateDisplay(calculatorController.ExecOperation(3));
        return;
    }

    // Ввод дроби (/)
     if (text == "/" || text == "\\") {
         updateDisplay(calculatorController.ExecCommandEditor(11));
         return;
     }

    // Равно (= или Enter)
    if (key == Qt::Key_Equal || key == Qt::Key_Return || key == Qt::Key_Enter) {
        updateDisplay(calculatorController.Calculate());
        return;
    }

    // Backspace
    if (key == Qt::Key_Backspace) {
        updateDisplay(calculatorController.ExecCommandEditor(12));
        return;
    }

    // Escape (используем как C - Clear All)
    if (key == Qt::Key_Escape) {
         updateDisplay(calculatorController.Reset());
         updateMemoryIndicator(false);
        return;
    }

    // Delete (используем как CE - Clear Entry)
    if (key == Qt::Key_Delete) {
        updateDisplay(calculatorController.ExecCommandEditor(13));
        return;
    }


     if (key == Qt::Key_F9) {
         updateDisplay(calculatorController.ExecCommandEditor(10));
         return;
     }

    QMainWindow::keyPressEvent(event);
}
