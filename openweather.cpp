#include "openweather.h"
#include <QDateTime>

const QString OpenWeather::mAppId = "";

OpenWeather::OpenWeather(QWidget *aParent) : QWidget(aParent)
{
    setupUi(this);
    mUlr = new QUrl();
    Q_CHECK_PTR(mUlr);

    mNam = new QNetworkAccessManager(this);
    Q_CHECK_PTR(mNam);

    mUlr->setScheme("http");
    mUlr->setHost("api.openweathermap.org");
    mUlr->setPath("/data/2.5/weather");

    clock->showTime();

    get("3081368");
}

OpenWeather::~OpenWeather()
{
    delete mUlr;
}

void OpenWeather::parseWeatherJson(QJsonDocument &aJsonDoc)
{
    QJsonObject jsonObj;
    QString main;
    QString description;
    QString info1;
    QString info2;

    if (!aJsonDoc.isObject()) {
        qDebug() << "JsonObject didn't found in JsonDoc";
        return;
    }

    jsonObj = aJsonDoc.object();

    if (jsonObj.contains("name") && jsonObj["name"].isString()) {
       main.append(jsonObj["name"].toString());
       main.append(" ");

    }

    if (jsonObj.contains("main") && jsonObj["main"].isObject()) {
        QJsonObject jMainObj = jsonObj["main"].toObject();

        if (jMainObj.contains("temp") && jMainObj["temp"].isDouble()) {
            main.append(QString::number(jMainObj["temp"].toDouble()) + QChar(0x2103) + " ");
        }

        if (jMainObj.contains("pressure") && jMainObj["pressure"].isDouble()) {
            info1.append("pressure:");
            info1.append(QString::number(jMainObj["pressure"].toDouble()));
            info1.append("hpa ");
        }

        if (jMainObj.contains("humidity") && jMainObj["humidity"].isDouble()) {
            info1.append("humidity:");
            info1.append(QString::number(jMainObj["humidity"].toDouble()));
            info1.append("% ");
        }
    }

    if (jsonObj.contains("weather") && jsonObj["weather"].isArray()) {
        QJsonArray jWeatherArr = jsonObj["weather"].toArray();
        for (int i = 0; i < jWeatherArr.size(); i++) {
            QJsonObject jWeatherObj = jWeatherArr.at(i).toObject();

            if (jWeatherObj.contains("main") && jWeatherObj["main"].isString())
                main.append(jWeatherObj["main"].toString());

            if (jWeatherObj.contains("description") && jWeatherObj["description"].isString())
                description.append(jWeatherObj["description"].toString());
        }
    }


    if (jsonObj.contains("clouds") && jsonObj["clouds"].isObject()) {
        QJsonObject jCloudsObj = jsonObj["clouds"].toObject();

        if (jCloudsObj.contains("all") && jCloudsObj["all"].isDouble()) {
          info1.append("cloudness:");
          info1.append(QString::number(jCloudsObj["all"].toDouble()));
          info1.append("% ");
        }
    }

    if (jsonObj.contains("rain") && jsonObj["rain"].isObject()) {
        QJsonObject jRainObj = jsonObj["rain"].toObject();

        if (jRainObj.contains("3h") && jRainObj["rain"].isDouble()) {
            info1.append("Rain: ");
            info1.append(QString::number(jRainObj["3h"].toDouble()));
        }
    }

    if (jsonObj.contains("snow") && jsonObj["snow"].isObject()) {
        QJsonObject jSnowObj = jsonObj["snow"].toObject();

        if (jSnowObj.contains("3h") && jSnowObj["3h"].isDouble()) {
            info1.append("Snow: ");
            info1.append(QString::number(jSnowObj["3h"].toDouble()));
        }
    }

    if (jsonObj.contains("sys") && jsonObj["sys"].isObject()) {
        QJsonObject jSysObj = jsonObj["sys"].toObject();
        QDateTime dateTime;
        QTime time;

        if (jSysObj.contains("sunrise") && jSysObj["sunrise"].isDouble()) {
            dateTime.setSecsSinceEpoch(jSysObj["sunrise"].toInt());
            time = dateTime.time();
            info2.append("Sunrise: ");
            info2.append(time.toString("hh:mm:ss"));
        }

        if (jSysObj.contains("sunset") && jSysObj["sunset"].isDouble()) {
            dateTime.setSecsSinceEpoch(jSysObj["sunset"].toInt());
            time = dateTime.time();
            info2.append(" Sunset: ");
            info2.append(time.toString("hh:mm:ss"));
        }
    }

    setMain(main);
    setDescription(description);
    setInfo1(info1);
    setInfo2(info2);
}

void OpenWeather::parseNetRpl(QNetworkReply *aRpl)
{
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(aRpl->readAll(), &jsonErr);

    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse network replay:" << jsonErr.errorString();
        return;
    }

    parseWeatherJson(jsonDoc);

    return;
}

void OpenWeather::onNetworkReplay()
{
    if (mNetRpl.isEmpty())
        return;

    for (int i = 0; i < mNetRpl.size(); i++) {
        QNetworkReply *netRpl = mNetRpl.at(i);

        if (netRpl->isFinished()) {
            mNetRpl.removeAt(i);

            if (netRpl->error()) {
                qWarning() <<  "Request finished with error:" << netRpl->errorString();
            } else {
                qDebug() << "Got response from server";
                parseNetRpl(netRpl);
            }

            delete netRpl;
        }
    }
}

int OpenWeather::get(QString aId)
{
    QUrlQuery urlQ;
    urlQ.addQueryItem("appid", mAppId);
    urlQ.addQueryItem("id", aId);
    urlQ.addQueryItem("units", "metric");

    mUlr->setQuery(urlQ);

    return get(*mUlr);
}

int OpenWeather::get(QUrl &aUrl)
{

    QNetworkReply *netRpl;
    netRpl = mNam->get(QNetworkRequest(aUrl));
    Q_CHECK_PTR(netRpl);

    connect(netRpl, &QNetworkReply::finished, this, &OpenWeather::onNetworkReplay);

    mNetRpl.append(netRpl);
    return 0;
}
