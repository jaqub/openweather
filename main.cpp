#include <QApplication>
#include <QPushButton>

#include "ui_openweathermain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;

    Ui::main ui;
    ui.setupUi(&w);
    w.showFullScreen();

    return a.exec();
}
