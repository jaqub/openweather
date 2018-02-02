#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>
#include <QMovie>

#include "ui_openweathermain.h"

class OpenWeather : public QWidget,
                    public Ui::OpenWeather
{
    Q_OBJECT
public:
    explicit OpenWeather(QWidget *parent = nullptr);

signals:

public slots:
    void onNetworkStatusChange(bool isOnline);

private:
    QNetworkConfigurationManager *netConf;

    QMovie *movie;
};

#endif // OPENWEATHER_H
