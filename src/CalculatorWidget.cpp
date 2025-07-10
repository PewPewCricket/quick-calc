//
// Created by pewpewcricket on 7/1/25.
//

#include "CalculatorWidget.h"

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <array>
#include <sstream>
#include <stack>
#include <stdfloat>

#include "mathSolver.h"

static std::array<std::string, 20> buttonTypes =
{
    "1", "2", "3", "+", "(",
    "4", "5", "6", "-", ")",
    "7", "8", "9", "*", "^",
    "CE", "0", "=", "/", "u-"
};

static std::string trimZeros(std::string s) {
    auto pos = s.find('.');
    if (pos != std::string::npos) {
        while (!s.empty() && s.back() == '0')
            s.pop_back();

        if (!s.empty() && s.back() == '.')
            s.pop_back();
    }
    return s;
}

void CalculatorWidget::appendDisplay(const QString& text) const
{
    QString cur = display->text();
    static const QSet<QString> ops = { "+", "-", "*", "/", "^", "(", ")" };

    if (text == "u-") {
        if (!cur.isEmpty() && !cur.endsWith(' '))
            cur += ' ';
        cur += "u-";
    }
    else if (cur.endsWith("u-") && text.size() == 1 && text[0].isDigit()) {
        cur += text;
    }
    else if (text.size() == 1 && text[0].isDigit() && !cur.isEmpty() && cur.back().isDigit()) {
        cur += text;
    }
    else {
        QString in = ops.contains(text) ? QString(" %1 ").arg(text) : text;
        QString trimmed = cur.trimmed();
        if (!trimmed.isEmpty() && !trimmed.endsWith(' '))
            trimmed += ' ';
        trimmed += in.trimmed();
        cur = trimmed;
    }

    display->setText(cur);
}

// =========================
// Button Callbacks
// =========================

void CalculatorWidget::eqCallback() const
{
    const std::vector<std::string> problem = toPostfix(display->text().toStdString());
    std::string solution = std::to_string(solvePostfix(problem));
    if (solution[0] == '-') {
        solution.erase(0,1);
        solution = "u-" + solution;
    }

    solution = trimZeros(solution);
    display->setText(QString::fromStdString(solution));
}

void CalculatorWidget::numCallback() const
{
    const QString buttonText = qobject_cast<QPushButton*>(sender())->text();
    appendDisplay(buttonText);
}

void CalculatorWidget::opCallback() const
{
    const QString buttonText = qobject_cast<QPushButton*>(sender())->text();
    appendDisplay(buttonText);
}

void CalculatorWidget::ceCallback() const
{
    display->setText("");
}

// =========================
// Constructors & Destructors
// =========================

CalculatorWidget::CalculatorWidget(QWidget *parent = nullptr) : QWidget(parent)
{
    layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    for (int i = 0; i < 20; i++) {
        buttons[i] = new QPushButton(QString::fromStdString(buttonTypes[i]), this);
        layout->addWidget(buttons[i], i / 5 + 1, i % 5, 1, 1);
        buttons[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        if (buttonTypes[i] == "CE")
            connect(buttons[i], &QPushButton::clicked, this, &CalculatorWidget::ceCallback);
        else if (buttonTypes[i] == "=")
            connect(buttons[i], &QPushButton::clicked, this, &CalculatorWidget::eqCallback);
        else if (
            buttonTypes[i] == "+" || buttonTypes[i] == "-" ||
            buttonTypes[i] == "*" || buttonTypes[i] == "/" ||
            buttonTypes[i] == "^" || buttonTypes[i] == "(" ||
            buttonTypes[i] == ")")
            connect(buttons[i], &QPushButton::clicked, this, &CalculatorWidget::opCallback);
        else {
            connect(buttons[i], &QPushButton::clicked, this, &CalculatorWidget::numCallback);
        }
    }
    display = new QLabel("", this);
    font = new QFont();
    font->setPointSize(24);
    display->setFont(*font);
    display->setWordWrap(true);
    display->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addWidget(display, 0, 0, 1, 5);
}

CalculatorWidget::~CalculatorWidget()
{
    delete display;
    for (int i = 0; i < 20; i++) {
        delete buttons[i];
    }
    delete layout;
}