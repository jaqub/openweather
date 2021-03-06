#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>
#include <QMovie>

#include "ui_openweathermain.h"
#include "udev.h"
#include "device.h"

class WeatherListItem;
class BacklightDev;

class OpenWeather : public QWidget,
                    public Ui::OpenWeather
{

public:
    explicit OpenWeather(QString aAppId = "", QString aId = "", QWidget *aParent = nullptr);
    ~OpenWeather();

    inline void setMain(const QString &aName) { main->setText(aName); }
    inline void setDescription(const QString aInfo) { description->setText(aInfo); }
    inline void setClock(const QString &time) { clock->setText(time);}

private slots:
  void onWeatherRpl();
  void onForecastRpl();

  void onBacklightTimer();

private:
    bool eventFilter(QObject *watched, QEvent *event);

    void timerEvent(QTimerEvent *event = nullptr);

    void updateTime(void);
    void updateWeather(void);

    void parseWeatherJson(QJsonDocument &aJsonDoc);
    void parseWeatherRpl(QNetworkReply *aRpl);
    int getWeather(QUrl &aUrl);
    int getWeather(const QString &aId);

    WeatherListItem *parseForecastJson(QJsonObject &aJObj);
    void parseForecastRpl(QNetworkReply *aRpl);
    int getForecast(const QString &aId);
    int getForecast(const QUrl &aUrl);

    void setupBrightness();

    const QString mAppId;
    const QString mId;
    QString mLang;
    int mClockTimer;
    int mWeatherTimer;

    BacklightDev *mBacklightDev;
    QTimer *mBacklightTimer;

    QUrl *mUrl;
    QNetworkAccessManager *mNam;
};

#endif // OPENWEATHER_H
