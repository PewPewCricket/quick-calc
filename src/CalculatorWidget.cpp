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

// Your apply_operator function (no change needed here)
float apply_operator(const float a, const float b, const char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b; // 'x' should be converted to '*' before this
    if (op == '/') return a / b;
    return 0; // Handle error or unsupported operator
}

float CalculatorWidget::solve(const std::string& equation) {
    std::stack<float> operands;
    std::stringstream ss(equation);
    std::string token;

    float num1 = 0.0f;
    float num2 = 0.0f;
    char op = ' ';
    bool num1_found = false;
    bool op_found = false;

    while (ss >> token) {
        if (isdigit(token[0]) || (token.length() > 1 && token[0] == '-' && isdigit(token[1]))) {
            if (!num1_found) {
                num1 = std::stof(token);
                num1_found = true;
            } else if (op_found) {
                num2 = std::stof(token);
                return apply_operator(num1, num2, op);
            }
        } else { // It's an operator
            if (token.length() == 1) {
                op = token[0];
                if (op == 'x') op = '*';
                op_found = true;
            } else
                return 0.0f;
        }
    }

    return num1;
}

static std::array<std::string, 16> buttonTypes = {
    "1", "2", "3", "+",
    "4", "5", "6", "-",
    "7", "8", "9", "x",
    "CE", "0", "=", "/"
};

void CalculatorWidget::appendDisplay(const QString& appendText) const {
    QString currentText = this->display->text();
    QString processedAppendText = appendText;

    if (appendText == "+" || appendText == "-" || appendText == "x" || appendText == "/") {
        processedAppendText = " " + appendText + " ";
    }

    currentText.append(processedAppendText);
    this->display->setText(currentText);
}

void CalculatorWidget::numCallback() const {
    const QString buttonText = qobject_cast<QPushButton*>(sender())->text();
    this->appendDisplay(buttonText);
}

void CalculatorWidget::opCallback() const {
    const QString buttonText = qobject_cast<QPushButton*>(sender())->text();
    this->appendDisplay(buttonText);
}

void CalculatorWidget::eqCallback() const {
    const QString equation = this->display->text();
    std::string equationStd = equation.toStdString();

    const size_t first = equationStd.find_first_not_of(' ');
    if (const size_t last = equationStd.find_last_not_of(' '); std::string::npos == first || std::string::npos == last)
        equationStd = "";
    else
        equationStd = equationStd.substr(first, (last - first + 1));
    const float result = CalculatorWidget::solve(equationStd);
    auto d_result = static_cast<double>(result);
    QString formattedResult = QString::number(d_result, 'f', 6);

    while (formattedResult.contains('.') && formattedResult.endsWith('0'))
        formattedResult.chop(1);
    if (formattedResult.endsWith('.'))
        formattedResult.chop(1);

    this->display->setText(formattedResult);
}

void CalculatorWidget::ceCallback() const {
    this->display->setText("");
}

CalculatorWidget::CalculatorWidget(QWidget *parent = nullptr) : QWidget(parent)
{
    this->layout = new QGridLayout(this);
    for (int i = 0; i < 16; i++) {
        this->buttons[i] = new QPushButton(QString::fromStdString(buttonTypes[i]), this);
        this->layout->addWidget(this->buttons[i], i / 4 + 1, i % 4, 1, 1);
        if (buttonTypes[i] == "CE")
            connect(this->buttons[i], &QPushButton::clicked, this, &CalculatorWidget::ceCallback);
        else if (buttonTypes[i] == "=")
            connect(this->buttons[i], &QPushButton::clicked, this, &CalculatorWidget::eqCallback);
        else if (
            buttonTypes[i] == "+" || buttonTypes[i] == "-" ||
            buttonTypes[i] == "x" || buttonTypes[i] == "/")
            connect(this->buttons[i], &QPushButton::clicked, this, &CalculatorWidget::opCallback);
        else {
            connect(this->buttons[i], &QPushButton::clicked, this, &CalculatorWidget::numCallback);
        }
    }
    this->display = new QLabel("", this);
    this->layout->addWidget(this->display, 0, 0, 1, 4);
}

CalculatorWidget::~CalculatorWidget()
{
    delete this->display;
    for (int i = 0; i < 16; i++) {
        delete this->buttons[i];
    }
    delete this->layout;
}