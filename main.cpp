#include <QApplication>

#include "openweather.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    OpenWeather *openWeather = new OpenWeather();

    openWeather->showFullScreen();

    return a.exec();
}
