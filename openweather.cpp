#include "openweather.h"

OpenWeather::OpenWeather(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    netConf = new QNetworkConfigurationManager(this);
    movie = new QMovie(":/ui/main/icons/loading.gif");

    movie->setScaledSize(QSize(75,75));
    status->setMovie(movie);
    movie->start();

    if (netConf->isOnline())
        status->setPixmap(QPixmap(":/ui/main/icons/Wifi-1-icon.png"));

    connect(netConf, &QNetworkConfigurationManager::onlineStateChanged,
            this, &OpenWeather::onNetworkStatusChange);
}

void OpenWeather::onNetworkStatusChange(bool isOnline)
{
    if (isOnline) {
        status->setPixmap(QPixmap(":/ui/main/icons/Wifi-1-icon.png"));
    } else {
        status->setMovie(movie);
        movie->start();
    }
}
