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

            if (jWeatherObj.contains("main") && jWeatherObj["main"].isString())
                main.append(jWeatherObj["main"].toString());

            if (jWeatherObj.contains("description") && jWeatherObj["description"].isString())
                description.append(jWeatherObj["description"].toString());

            if (jWeatherObj.contains("icon") && jWeatherObj["icon"].isString()) {
                QUrl urlQ;
                urlQ.setScheme("http");
                urlQ.setHost("openweathermap.org");
                urlQ.setPath("/img/w/" + jWeatherObj["icon"].toString() + ".png");
                QNetworkReply *rpl = mNam->get(QNetworkRequest(urlQ));
                connect(rpl, &QNetworkReply::finished, [=] () {
                    QPixmap pixmap;
                    pixmap.loadFromData(rpl->readAll());
                    icon->setPixmap(pixmap);
                    delete rpl;
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

void OpenWeather::parseForecastJson(QJsonObject &aJObj)
{
    QString weather;

    if (aJObj.contains("dt") && aJObj["dt"].isDouble()) {
        QDateTime dateTime;
        dateTime.setSecsSinceEpoch(aJObj["dt"].toInt());
        if (dateTime.time().hour() != 13)
            return;

        weather.append(dateTime.toString("ddd\t").toUpper());
    }

    if (aJObj.contains("main") && aJObj["main"].isObject()) {
        QJsonObject jMainObj = aJObj["main"].toObject();

        if (jMainObj.contains("temp") && jMainObj["temp"].toDouble()) {
            weather.append(QString::number(jMainObj["temp"].toDouble(), 'f', 1) + QChar(0x2103) + "\t");
        }
    }

    if (aJObj.contains("weather") && aJObj["weather"].isArray()) {
        QJsonArray jWeatherArr = aJObj["weather"].toArray();

        for (int i = 0; i < jWeatherArr.size(); i++) {
           QJsonObject jWeatherObj = jWeatherArr.at(i).toObject();

           if (jWeatherObj.contains("description") && jWeatherObj["description"].isString()) {
               weather.append(jWeatherObj["description"].toString());
           }
        }
    }

    qDebug() << weather;
    QListWidgetItem *listItem = new QListWidgetItem();
    listItem->setText(weather);
    forecastLst->addItem(listItem);
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

            parseForecastJson(jListObj);
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
