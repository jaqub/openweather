#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>
#include <QMovie>

#include "ui_openweathermain.h"

class WeatherListItem;

class OpenWeather : public QWidget,
                    public Ui::OpenWeather
{

public:
    explicit OpenWeather(QWidget *aParent = nullptr, QString aAppId = "");
    ~OpenWeather();

    inline void setMain(const QString &aName) { main->setText(aName); }
    inline void setDescription(const QString aInfo) { description->setText(aInfo); }
    inline void setClock(const QString &time) { clock->setText(time);}

private slots:
  void onWeatherRpl();
  void onForecastRpl();

private:
    void timerEvent(QTimerEvent *event = nullptr);

    void updateTime(void);
    void updateWeather(void);

    void parseWeatherJson(QJsonDocument &aJsonDoc);
    void parseWeatherRpl(QNetworkReply *aRpl);
    int getWeather(QUrl &aUrl);
    int getWeather(const QString &aId);
    int getWeather(QString &aCountry, QString &aCity);

    WeatherListItem *parseForecastJson(QJsonObject &aJObj);
    void parseForecastRpl(QNetworkReply *aRpl);
    int getForecast(const QUrl &aUrl);

    const QString mAppId;
    QString mLang;
    int mClockTimer;
    int mWeatherTimer;
    QUrl *mUlr;
    QNetworkAccessManager *mNam;
};

#endif // OPENWEATHER_H
