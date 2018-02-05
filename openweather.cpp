#include "openweather.h"

OpenWeather::OpenWeather(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    clock->showTime();
}
