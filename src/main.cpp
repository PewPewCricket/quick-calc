#include <QMainWindow>
#include <QApplication>
#include <QWidget>

#include "CalculatorWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CalculatorWidget window(nullptr);
    window.setWindowTitle("Calculator");
    window.show();
    return QApplication::exec();
}