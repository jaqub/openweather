#include <QApplication>

#include "openweather.h"

int main(int argc, char *argv[])
{
    int ret;

    QApplication a(argc, argv);

    OpenWeather *openWeather = new OpenWeather();

    openWeather->showFullScreen();

    ret = a.exec();

    delete openWeather;

    return ret;
}
