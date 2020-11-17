#include "calculator.h"
#include "button.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QtMath>
#include <vector>
#include <string>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), memory(nullptr), waitingForOperand(true)
{

    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    QFont font = display->font();
    font.setPointSize(font.pointSize() + 8);
    display->setFont(font);

    for (int i = 0; i < NumDigitButtons; ++i)
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

    Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
    Button *seperatorButton = createButton(tr(";"), SLOT(seperatorButton()));
    Button *backspaceButton = createButton(tr("Backspace"), SLOT(backspaceClicked()));
    Button *clearButton = createButton(tr("Clear"), SLOT(clear()));
    Button *clearAllButton = createButton(tr("Clear All"), SLOT(clearAll()));
    Button *euclidButton = createButton(tr("Euclid"), SLOT(euclidButtonClicked()));
    Button *dotProduct = createButton(tr("dotProduct"), SLOT(dotProductButtonClicked()));
    Button *maxNorm = createButton(tr("maxNrom"), SLOT(maxNormClicked()));

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display, 0, 0, 1, 6);
    mainLayout->addWidget(backspaceButton, 1, 0, 1, 2);
    mainLayout->addWidget(clearButton, 1, 2, 1, 2);
    mainLayout->addWidget(clearAllButton, 1, 4, 1, 2);

    for (int i = 1; i < NumDigitButtons; ++i) {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        mainLayout->addWidget(digitButtons[i], row, column);
    }

    mainLayout->addWidget(digitButtons[0], 5, 1);
    mainLayout->addWidget(pointButton, 5, 2);
    mainLayout->addWidget(seperatorButton, 5, 3);
    mainLayout->addWidget(euclidButton, 2, 4);
    mainLayout->addWidget(dotProduct, 3, 4);
    mainLayout->addWidget(maxNorm, 4, 4);

    setLayout(mainLayout);

    setWindowTitle(tr("Calculator"));
}

void Calculator::seperatorButton()
{
    display->setText(display->text() + tr(";"));

}

double* Calculator::display2array()
{
    QStringList test = display->text().split(';');
    int size = test.size();
    double vectorA[size];
    for (int i = 0; i < size; i++) {
        vectorA[i] = test[i].toDouble();
    }
    return vectorA;
}

void Calculator::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();
    if (display->text() == "0" && digitValue == 0.0)
        return;

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }
    display->setText(display->text() + QString::number(digitValue));
}

void Calculator::euclidButtonClicked()
{
//    display->setText(/*fucntionhere(display2array)*/);
}

void Calculator::dotProductButtonClicked()
{
    if(memory == nullptr)
        memory = display2array();
        display->clear();
//    else
//        display->setText(function(memory, display2array()));
}

void Calculator::maxNormClicked()
{
//    display->setText(/*fucntionhere(display2array)*/);
}

void Calculator::pointClicked()
{
    if (waitingForOperand)
        display->setText("0");
    if (!display->text().contains('.'))
        display->setText(display->text() + tr("."));
    waitingForOperand = false;
}


void Calculator::backspaceClicked()
{
    if (waitingForOperand)
        return;

    QString text = display->text();
    text.chop(1);
    if (text.isEmpty()) {
        text = "0";
        waitingForOperand = true;
    }
    display->setText(text);
}


void Calculator::clear()
{
    if (waitingForOperand)
        return;

    display->setText("0");
    waitingForOperand = true;
}


void Calculator::clearAll()
{
    memory = nullptr;
    display->setText("0");
    waitingForOperand = true;
}


Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
