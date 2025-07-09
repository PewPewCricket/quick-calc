//
// Created by pewpewcricket on 7/1/25.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <iostream>
#include <string>
#include <stdfloat>

class CalculatorWidget final : public QWidget {
    Q_OBJECT
public:
    explicit CalculatorWidget(QWidget *parent);
    ~CalculatorWidget() override;

private:
    QLabel *display;
    std::array<QPushButton *, 16> buttons{};
    QGridLayout *layout;

    void appendDisplay(const QString& appendText) const;
    static float solve(const std::string& equation);

private slots:
    void numCallback() const;
    void eqCallback() const;
    void ceCallback() const;
    void opCallback() const;
};

#endif //MAINWINDOW_H
