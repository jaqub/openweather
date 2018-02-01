#include "openweather.h"

OpenWeather::OpenWeather(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    netConf = new QNetworkConfigurationManager(this);
    status->setPixmap(QPixmap(":/ui/main/icons/Wifi-1-icon.png"));

    if (netConf->isOnline())
        status->setVisible(true);

    connect(netConf, &QNetworkConfigurationManager::onlineStateChanged, this, &OpenWeather::onNetStatusChange);
}

void OpenWeather::onNetStatusChange(bool isOnline)
{
    status->setVisible(isOnline);
}
