#include "openweather.h"
#include <QDateTime>
#include <QLocale>
#include <QListWidgetItem>
#include <QListWidget>

#include "weatheritemdelegate.h"
#include "weatherlistitem.h"

const QString OpenWeather::mAppId = "";

OpenWeather::OpenWeather(QWidget *aParent) : QWidget(aParent)
{
    setupUi(this);
    forecastList->setItemDelegate(new WeatherItemDelegate(forecastList));

    mUlr = new QUrl();
    Q_CHECK_PTR(mUlr);

    mNam = new QNetworkAccessManager(this);
    Q_CHECK_PTR(mNam);

    mUlr->setScheme("http");
    mUlr->setHost("api.openweathermap.org");

    timerEvent();
    mTimer = startTimer(1000);
    getWeather(QString("3081368"));
}

OpenWeather::~OpenWeather()
{
    delete mUlr;
}

void OpenWeather::timerEvent(QTimerEvent *event)
{
    QTime currentTime = QTime::currentTime();
    QString time = currentTime.toString("hh:mm");
    if (currentTime.second() % 2)
        time[2] = ' ';
    setClock(time);
}

void OpenWeather::parseWeatherJson(QJsonDocument &aJsonDoc)
{
    QJsonObject jsonObj;
    QString main;
    QString description;

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
            main.append(QString::number(jMainObj["temp"].toDouble(), 'f', 1) + QChar(0x2103) + "\t");
        }
    }

    if (jsonObj.contains("weather") && jsonObj["weather"].isArray()) {
        QJsonArray jWeatherArr = jsonObj["weather"].toArray();
        for (int i = 0; i < jWeatherArr.size(); i++) {
            QJsonObject jWeatherObj = jWeatherArr.at(i).toObject();

            if (jWeatherObj.contains("description") && jWeatherObj["description"].isString())
                description.append(jWeatherObj["description"].toString());

            if (jWeatherObj.contains("icon") && jWeatherObj["icon"].isString()) {
                QString url = QString("http://openweathermap.org/img/w/%1.png").arg(jWeatherObj["icon"].toString());
                QNetworkReply *rpl = mNam->get(QNetworkRequest(QUrl(url)));
                connect(rpl, &QNetworkReply::finished, [=] () {
                    QPixmap pixmap;
                    pixmap.loadFromData(rpl->readAll());
                    icon->setPixmap(pixmap);
                    rpl->deleteLater();
                });
            }
        }
    }

    setMain(main);
    setDescription(description);
}

void OpenWeather::parseWeatherRpl(QNetworkReply *aRpl)
{
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(aRpl->readAll(), &jsonErr);

    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse network replay:" << jsonErr.errorString();
        setMain("Json Error");
        setDescription(jsonErr.errorString());
        return;
    }

    parseWeatherJson(jsonDoc);

    return;
}

void OpenWeather::onWeatherRpl()
{
    QNetworkReply *netRpl = qobject_cast<QNetworkReply*>(sender());

    if (netRpl->isFinished()) {
        if (netRpl->error()) {
            qWarning() <<  "Request finished with error:" << netRpl->errorString();
            setMain("Host request error");
            setDescription(netRpl->errorString());
        } else {
            qDebug() << "Got response from server";
            parseWeatherRpl(netRpl);
        }

        netRpl->deleteLater();
    }
}

int OpenWeather::getWeather(const QString &aId)
{
    mUlr->setPath("/data/2.5/weather");

    QUrlQuery urlQ;
    urlQ.addQueryItem("appid", mAppId);
    urlQ.addQueryItem("id", aId);
    urlQ.addQueryItem("units", "metric");
    urlQ.addQueryItem("lang", QLocale().bcp47Name());

    mUlr->setQuery(urlQ);

    return getWeather(*mUlr);
}

int OpenWeather::getWeather(QUrl &aUrl)
{

    QNetworkReply *netRpl;
    netRpl = mNam->get(QNetworkRequest(aUrl));
    Q_CHECK_PTR(netRpl);

    connect(netRpl, &QNetworkReply::finished, this, &OpenWeather::onWeatherRpl);

    return 0;
}

WeatherListItem *OpenWeather::parseForecastJson(QJsonObject &aJObj)
{
    WeatherListItem *listItem = new WeatherListItem();
    QString weather;

    if (aJObj.contains("dt") && aJObj["dt"].isDouble()) {
        QDateTime dateTime;
        dateTime.setSecsSinceEpoch(aJObj["dt"].toInt());
        weather.append(dateTime.toString("ddd hh ").toUpper());
    }

    if (aJObj.contains("main") && aJObj["main"].isObject()) {
        QJsonObject jMainObj = aJObj["main"].toObject();

        if (jMainObj.contains("temp") && jMainObj["temp"].toDouble()) {
            weather.append(QString::number(jMainObj["temp"].toDouble(), 'f', 1) + QChar(0x2103) + " ");
        }
    }

    if (aJObj.contains("weather") && aJObj["weather"].isArray()) {
        QJsonArray jWeatherArr = aJObj["weather"].toArray();

        for (int i = 0; i < jWeatherArr.size(); i++) {
           QJsonObject jWeatherObj = jWeatherArr.at(i).toObject();

           if (jWeatherObj.contains("description") && jWeatherObj["description"].isString()) {
               weather.append(jWeatherObj["description"].toString());
           }

           if (jWeatherObj.contains("icon") && jWeatherObj["icon"].isString()) {
               QString url = QString("http://openweathermap.org/img/w/%1.png").arg(jWeatherObj["icon"].toString());
               QNetworkReply *rpl = mNam->get(QNetworkRequest(QUrl(url)));
               connect(rpl, &QNetworkReply::finished, listItem, &WeatherListItem::onIconDownload);
           }
        }
    }

    qDebug() << weather;
    listItem->setText(weather);
    return listItem;
}

void OpenWeather::parseForecastRpl(QNetworkReply *aRpl)
{
    QJsonParseError jsonErr;
    QJsonObject jObj;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(aRpl->readAll(), &jsonErr);

    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse network replay:" << jsonErr.errorString();
        return;
    }

    jObj = jsonDoc.object();

    if (jObj.contains("list") && jObj["list"].isArray()) {
        QJsonArray jListArr = jObj["list"].toArray();

        for (int i = 0; i < jListArr.size(); i++) {
            QJsonObject jListObj = jListArr.at(i).toObject();

            WeatherListItem *item = parseForecastJson(jListObj);
            if (item)
              forecastList->addItem(item);
         }
    } else {
        parseForecastJson(jObj);
    }

    return;
}

void OpenWeather::onForecastRpl()
{
    QNetworkReply *netRpl = qobject_cast<QNetworkReply*>(sender());

    if (netRpl->error()) {
        qDebug() << "Request finished with error:" << netRpl->errorString();
    } else {
        parseForecastRpl(netRpl);
    }

    netRpl->deleteLater();
}

int OpenWeather::getForecast(const QUrl &aUrl)
{
    QNetworkReply *netRpl;
    netRpl = mNam->get(QNetworkRequest(aUrl));
    Q_CHECK_PTR(netRpl);

    connect(netRpl, &QNetworkReply::finished, this, &OpenWeather::onForecastRpl);

    return 0;
}
