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
            main.append(QString::number(jMainObj["temp"].toDouble()) + QChar(0x2103) + " ");
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

        delete netRpl;
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

void OpenWeather::onForecastRpl()
{
    QNetworkReply *rpl = qobject_cast<QNetworkReply*>(sender());

    delete rpl;
}

int OpenWeather::getForecast(const QUrl &aUrl)
{
    QNetworkReply *netRpl;
    netRpl = mNam->get(QNetworkRequest(aUrl));
    Q_CHECK_PTR(netRpl);

    connect(netRpl, &QNetworkReply::finished, this, &OpenWeather::onForecastRpl);

    return 0;
}
