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

    inline void setMain(const QString &aName) { main_info->setText(aName); }
    inline void setDescription(const QString aInfo) { description->setText(aInfo); }
    inline void setInfo1(const QString aInfo) { info1->setText(aInfo); }
    inline void setInfo2(const QString aInfo) { info2->setText(aInfo); }

public slots:
  void onNetworkReplay();

private:
    void parseWeatherJson(QJsonDocument &aJsonDoc);
    void parseNetRpl(QNetworkReply *aRpl);
    int get(QUrl &aUrl);
    int get(QString aId);
    int get(QString &aCountry, QString &aCity);

    static const QString mAppId;
    QUrl *mUlr;
    QNetworkAccessManager *mNam;
    QList<QNetworkReply*> mNetRpl;
};

#endif // OPENWEATHER_H
