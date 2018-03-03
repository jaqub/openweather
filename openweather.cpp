#include "openweather.h"
#include <QDateTime>
#include <QLocale>
#include <QListWidgetItem>
#include <QListWidget>
#include <QEvent>

#include "weatheritemdelegate.h"
#include "weatherlistitem.h"

OpenWeather::OpenWeather(QString aAppId, QString aId, QWidget *aParent) : QWidget(aParent),
    mAppId(aAppId), mId(aId)
{
    setupUi(this);
    forecastList->setItemDelegate(new WeatherItemDelegate(forecastList));
    forecastList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mNam = new QNetworkAccessManager(this);
    Q_CHECK_PTR(mNam);

    mUrl = new QUrl();
    Q_CHECK_PTR(mUrl);

    mUrl->setScheme("http");
    mUrl->setHost("api.openweathermap.org");

    mLang = locale().bcp47Name();
    updateTime();

    mClockTimer = startTimer(1000);
    mWeatherTimer = startTimer(300000);

    mDevices = (UdevSingleton::get())->getDevice("backlight");
    qDebug() << "Got: " << mDevices.size() << "devices";

    getWeather(mId);
    getForecast(mId);
}

OpenWeather::~OpenWeather()
{
    // release devices if any
    if (!mDevices.isEmpty()) {
        for (QVector<Device *>::iterator it = mDevices.begin(); it < mDevices.end(); it++) {
            delete *it;
        }
    }

    delete mUrl;
}

void OpenWeather::updateTime(void)
{
    QTime currentTime = QTime::currentTime();
    QString time = currentTime.toString("hh:mm");
    if (currentTime.second() % 2)
        time[2] = ' ';
    setClock(time);
}

void OpenWeather::updateWeather(void)
{
    getWeather(mId);
    getForecast(mId);
}

void OpenWeather::timerEvent(QTimerEvent *event)
{
    int timerId = event->timerId();
    if (timerId == mClockTimer)
        updateTime();
    else if (timerId == mWeatherTimer)
        updateWeather();
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
            main.append(QString::number(jMainObj["temp"].toDouble(), 'f', 1) + QChar(0x2103));
        }

        if (jMainObj.contains("pressure") && jMainObj["pressure"].isDouble()) {
            main.append(QString("/%1hpa").arg(jMainObj["pressure"].toDouble()));
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
    mUrl->setPath("/data/2.5/weather");

    QUrlQuery urlQ;
    urlQ.addQueryItem("appid", mAppId);
    urlQ.addQueryItem("id", aId);
    urlQ.addQueryItem("units", "metric");
    urlQ.addQueryItem("lang", mLang);

    mUrl->setQuery(urlQ);

    return getWeather(*mUrl);
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

        forecastList->clear();

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

int OpenWeather::getForecast(const QString &aId)
{
    mUrl->setPath("/data/2.5/forecast");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("appid", mAppId);
    urlQuery.addQueryItem("lang", mLang);
    urlQuery.addQueryItem("units", "metric");
    urlQuery.addQueryItem("id", aId);

    return getForecast(*mUrl);
}

int OpenWeather::getForecast(const QUrl &aUrl)
{
    QNetworkReply *netRpl;
    netRpl = mNam->get(QNetworkRequest(aUrl));
    Q_CHECK_PTR(netRpl);

    connect(netRpl, &QNetworkReply::finished, this, &OpenWeather::onForecastRpl);

    return 0;
}
