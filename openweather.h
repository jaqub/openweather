#ifndef OPENWEATHER_H
#define OPENWEATHER_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>
#include <QMovie>
#include <QList>

#include "ui_openweathermain.h"

class OpenWeather : public QWidget,
                    public Ui::OpenWeather
{
    Q_OBJECT
public:
    explicit OpenWeather(QWidget *aParent = nullptr);
    ~OpenWeather();

    inline void setMain(const QString &aName) { main->setText(aName); }
    inline void setDescription(const QString aInfo) { description->setText(aInfo); }
    inline void setClock(const QString &time) { clock->setText(time);}

public slots:
  void onWeatherRpl();

private:
    void timerEvent(QTimerEvent *event = nullptr);
    void parseWeatherJson(QJsonDocument &aJsonDoc);
    void parseWeatherRpl(QNetworkReply *aRpl);
    int getWeather(QUrl &aUrl);
    int getWeather(const QString &aId);
    int getWeather(QString &aCountry, QString &aCity);

    static const QString mAppId;
    int mTimer;
    QUrl *mUlr;
    QNetworkAccessManager *mNam;
    QList<QNetworkReply*> mNetRpl;
};

#endif // OPENWEATHER_H
