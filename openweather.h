#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>

#include "ui_openweathermain.h"

class OpenWeather : public QWidget, public Ui::OpenWeather
{
    Q_OBJECT
public:
    explicit OpenWeather(QWidget *parent = nullptr);

signals:

public slots:
    void onNetStatusChange(bool isOnline);

private:
    QNetworkConfigurationManager *netConf;
};

#endif // OPENWEATHER_H
